#include <string>
#include "pstcode.h"
#include "codegen.h"
using namespace std;

PstackCode CodeGen::generate(Visitable *vis)
{
    vis->accept(this);
    return code;
}

void CodeGen::visitProg(Prog *prog)
{
    code.begin_prog();
    code.prolog(symbols);

    // Insert call to main(), to be patched up later.
    code.add(I_CALL);
    int patchloc = code.pos();
    code.add(0);
    code.add(0);
    code.add(I_ENDPROG);

    // Generate code for the functions.
    prog->listfunction_->accept(this);

    // Now look up the address of main, and throw if it wasn't defined.
    if (!symbols.exists("main"))
        throw UnknownFunc("main");
    int level = symbols.levelof("main");
    int addr = symbols["main"]->address();

    // Patch up the address of main.
    code.at(patchloc) = level;
    code.at(patchloc + 1) = addr;

    code.end_prog();
}

void CodeGen::visitGlobal(Global *global)
{
    global->type_->accept(this); // sets currtype
    visitIdent(global->ident_);
    Ident glob_name = currid;

    if (symbols.exists(glob_name))//check that the var name does not already exist
        throw Redeclared(glob_name);

    symbols.insert(Symbol(glob_name, currtype, -1, code.pos(), TY_BAD)); //insert into symbol table

    code.add(I_PROG);	//I_PROG allocates global memory
    int patchloc = code.pos(); // to be filled with number of global variables.
    code.add(0);
    code.add(code.pos() + 1);

    //fill in number of variables
    int startvar = symbols.numvars();
    code.at(patchloc) = symbols.numvars() - startvar;

    //return, popping off parameters
    code.add(I_ENDPPROC);
    code.add(funargs);
}


void CodeGen::visitFun(Fun *fun)
{
    fun->type_->accept(this); //sets currtype
    fun_type = currtype;


    // return type in currtype, but currently ignored (always int)
    visitIdent(fun->ident_);
    Ident fun_name = currid;
    currfun = currid;
    
    if (symbols.exists(fun_name))
        throw Redeclared(fun_name);

    symbols.insert(Symbol(fun_name, TY_FUNC, funargs, code.pos(), TY_BAD));


    code.add(I_PROC);
    int patchloc = code.pos(); // to be filled with number of local variables.
    code.add(0);
    code.add(code.pos() + 1); // function code starts next

    symbols.enter(); // since parameters are local to the function

    // Adds entries to symbol table, sets funargs
    fun->listdecl_->accept(this);
    int startvar = symbols.numvars();

    // Generate code for function body.
    fun->liststm_->accept(this);

    // Fill in number of local variables.
    code.at(patchloc) = symbols.numvars() - startvar;
    symbols[fun_name]->numargs()=funargs;
    symbols[fun_name]->setargs(funargs);
    symbols.leave();

    // Return, popping off our parameters.
    code.add(I_ENDPPROC);
    code.add(funargs);
}

void CodeGen::visitDecA(DecA *deca)
{
    deca->type_->accept(this);
    deca->listvar_->accept(this);
}

void CodeGen::visitSDecl(SDecl *sdecl)
{
    sdecl->decl_->accept(this); // visitDec
}

void CodeGen::visitSExp(SExp *sexp)
{
    sexp->exp_->accept(this);

    // Pop and discard the expression's value.  pstack doesn't have a
    // POP instruction, but a conditional jump to the next instruction
    // (PC + 2) will do the trick.
    code.add(I_JR_IF_TRUE);
    code.add(2);
}

void CodeGen::visitSBlock(SBlock *sblock)
{
    sblock->liststm_->accept(this);
}

void CodeGen::visitSWhile(SWhile *swhile)
{
    int looploc = code.pos(); // Beginning of test
    swhile->exp_->accept(this);
    code.add(I_JR_IF_FALSE);  // Jump past the body.
    code.add(0);
    int patchloc = code.pos() - 1;

    swhile->stm_->accept(this); // Body.
    code.add(I_JR);
    code.add(looploc - (code.pos() - 1)); // offset to looploc
    code.at(patchloc) = code.pos() - (patchloc - 1);
}

void CodeGen::visitSRepeat(SRepeat *srep)
{
    int looploc = code.pos(); //Store location of Body start
    srep->stm_->accept(this);
    srep->exp_->accept(this); // Evaluate expression
    code.add(I_JR_IF_FALSE); // Jump to statement start if expression False
    code.add(looploc - (code.pos() - 1));
}

void CodeGen::visitSFor(SFor *sfor)
{
    int looploc = code.pos(); // Just like While loop with extra statement
    sfor->exp_1->accept(this); //Place test code
    code.add(I_JR_IF_FALSE); //Jump past the body if test fails
    code.add(0);
    int patchloc = code.pos() - 1;
    sfor->stm_->accept(this); // Run body of code
    sfor->exp_2->accept(this); // Run expression to get closer to end of for loop
    code.add(I_JR);
    code.add(looploc - (code.pos() - 1)); // offset to looploc
    code.at(patchloc) = code.pos() - (patchloc - 1);
}

void CodeGen::visitSFor3(SFor3 *sfor)
{
    /*code.add(I_CALL);
    int callloc = code.pos();
    code.add(0);
    code.add(callloc+2);

    code.add(I_PROC);
    int startloc = code.pos(); // to be filled with number of local variables.
    code.add(0);
    code.add(code.pos() + 1); */
    symbols.enter();
    //int startvar = symbols.numvars();

    sfor->stm_1->accept(this);

    // Patch up the address of main.
    //code.at(callloc) = symbols.levelof(currid) + 1;

    int looploc = code.pos();
    sfor->exp_1->accept(this);
    code.add(I_JR_IF_FALSE);
    code.add(0);
    int patchloc = code.pos() - 1;
    sfor->stm_2->accept(this);
    sfor->exp_2->accept(this);
    code.add(I_JR);
    code.add(looploc - (code.pos() - 1));
    code.at(patchloc) = code.pos() - (patchloc - 1);

    // Fill in number of local variables.
    //code.at(startloc) = symbols.numvars() - startvar;
    symbols.leave();

    // Return, popping off our parameters.
    //code.add(I_ENDPPROC);
    //code.add(0);
}

void CodeGen::visitSIf(SIf *sif)
{
    sif->exp_->accept(this);
    code.add(I_JR_IF_FALSE);  // Jump past the body.
    code.add(0);
    int patchloc = code.pos() - 1;

    sif->stm_->accept(this); // Body.
    code.at(patchloc) = code.pos() - (patchloc - 1);
}

void CodeGen::visitSIfElse(SIfElse *sifelse)
{
    sifelse->exp_->accept(this); // Put expression code
    code.add(I_JR_IF_FALSE); // Jump past body if expression false
    code.add(0);
    int patchloc = code.pos() - 1;

    sifelse->stm_1->accept(this); //Body
    code.add(I_JR); // Jump past else
    code.at(patchloc) = code.pos() - (patchloc - 2); //Jump to statement after Jump
    code.add(0);
    patchloc = code.pos() - 1;
    sifelse->stm_2->accept(this); // Else body
    code.at(patchloc) = code.pos() - (patchloc - 1);
}

void CodeGen::visitSReturn(SReturn *sreturn)
{
    // Could avoid the I_SWAP later if we generate code for the
    // return expression after pushing the return value address.
    // sreturn->exp_->accept(this);

    // Store the top of stack (return value) at (bp-funargs)
    code.add(I_VARIABLE);
    code.add(0);
    code.add(-(funargs+1));
    //code.add(I_SWAP);
    sreturn->exp_->accept(this); // Evaluate expression after variable assignment to avoid swap
    if (fun_type != currtype)
    {
        throw TypeError(currid);
    }
    if (fun_type != currtype)
    {
        throw TypeError(currid);
    }

    code.add(I_ASSIGN);
    code.add(1);

    // And return, popping off our parameters.
    code.add(I_ENDPPROC);
    code.add(funargs);
}

void CodeGen::visitEAss(EAss *eass)
{
    visitIdent(eass->ident_); // sets currid
    if (!symbols.exists(currid))
        throw UnknownVar(currid);

    // Compute the address.
    code.add(I_VARIABLE);
    code.add(symbols.levelof(currid));
    code.add(symbols[currid]->address());

    // One copy of the address for the assignment, one for the result.
    code.add_dup();

    // Generate code for the value of the RHS.
    eass->exp_->accept(this);

    // Store the value at the computed address.
    code.add(I_ASSIGN);
    code.add(1);

    // Dereference the address and return its value.
    code.add(I_VALUE);
}

void CodeGen::visitEAnd(EAnd *eand)
{
    eand->exp_1->accept(this);
    eand->exp_2->accept(this);
    code.add(I_AND);
}

void CodeGen::visitEOr(EOr *eor)
{
    eor->exp_1->accept(this);
    eor->exp_2->accept(this);
    code.add(I_OR);
}

void CodeGen::visitENot(ENot *enot)
{
    enot->exp_->accept(this);
    code.add(I_NOT);
}

void CodeGen::visitEEq(EEq *eeq)
{
    eeq->exp_1->accept(this);
    eeq->exp_2->accept(this);
    code.add(I_EQUAL);
}

void CodeGen::visitENEq(ENEq *eneq)
{
    eneq->exp_1->accept(this);
    eneq->exp_2->accept(this);
    code.add(I_EQUAL);
    code.add(I_NOT);
}

void CodeGen::visitVarAss(VarAss *varass)
{
    visitIdent(varass->ident_);
    if (symbols.exists(currid))
    {
        throw Redeclared(currid);
    }
    symbols.insert(Symbol(currid, currtype, 3 + symbols.numvars() - funargs));

    code.add(I_VARIABLE);
    code.add(symbols.levelof(currid));
    code.add(symbols[currid]->address());

    // One copy of the address for the assignment, one for the result.
    code.add_dup();

    // Generate code for the value of the RHS.
    varass->exp_->accept(this);

    // Store the value at the computed address.
    code.add(I_ASSIGN);
    code.add(1);

    // Dereference the address and return its value.
    code.add(I_VALUE);
}

void CodeGen::visitELt(ELt *elt)
{
    elt->exp_1->accept(this);
    elt->exp_2->accept(this);
    code.add(I_LESS);
}

void CodeGen::visitEEqLt(EEqLt *eeqlt)
{
    eeqlt->exp_1->accept(this);
    eeqlt->exp_2->accept(this);
    code.add(I_GREATER);
    code.add(I_NOT);
}

void CodeGen::visitEGt(EGt *egt)
{
    egt->exp_1->accept(this);
    egt->exp_2->accept(this);
    code.add(I_GREATER);
}

void CodeGen::visitEEqGt(EEqGt *eeqgt)
{
    eeqgt->exp_1->accept(this);
    eeqgt->exp_2->accept(this);
    code.add(I_LESS);
    code.add(I_NOT);
}


void CodeGen::visitEAdd(EAdd *eadd)
{
    eadd->exp_1->accept(this);
    eadd->exp_2->accept(this);
    code.add(I_ADD);
}

void CodeGen::visitESub(ESub *esub)
{
    esub->exp_1->accept(this);
    esub->exp_2->accept(this);
    code.add(I_SUBTRACT);
}

void CodeGen::visitEMul(EMul *emul)
{
    emul->exp_1->accept(this);
    emul->exp_2->accept(this);
    code.add(I_MULTIPLY);
}

void CodeGen::visitCall(Call *call)
{
    visitIdent(call->ident_);
    if (!symbols.exists(currid))
        throw UnknownFunc(currid);

    currfun = currid;

    //printf("Symbol name: %s\n", symbols[currid]->name());

    int level = symbols.levelof(currid);
    int addr = symbols[currid]->address();

    // Make room on the stack for the return value.  Assumes all functions
    // will return some value.
    code.add(I_CONSTANT);
    code.add(0);

    // Generate code for the expressions (which leaves their values on the
    // stack when executed).
    call->listexp_->accept(this);

    if(symbols[currid]->type()==TY_FUNC && symbols[currid]->numargs()!=-1){
   	if (symbols[currid]->numargs()!=call->listexp_->size()){
    		throw ArgError("A function does not have the appropriate number of arguments!\n");
		}
	}


    code.add(I_CALL);
    code.add(level);
    code.add(addr);
    
    // The result, if any, is left on the stack.
}

void CodeGen::visitEVar(EVar *evar)
{
    visitIdent(evar->ident_); // sets currid
    if (!symbols.exists(currid))
        throw UnknownVar(currid);

	currtype=symbols[currid]->type();

    // Compute the address.
    code.add(I_VARIABLE);
    code.add(symbols.levelof(currid));
    code.add(symbols[currid]->address());
    // Dereference it.
    code.add(I_VALUE);
}

void CodeGen::visitEStr(EStr *estr)
{
    code.add(I_CONSTANT);
    code.add(0); // must be patched for string address
    visitString(estr->string_);
}

void CodeGen::visitEInt(EInt *eint)
{
    visitInteger(eint->integer_);
}

void CodeGen::visitEDouble(EDouble *edouble)
{
    visitDouble(edouble->double_);
}

void CodeGen::visitTInt(TInt *)
{
    currtype = TY_INT;
}

void CodeGen::visitTDouble(TDouble *)
{
    currtype = TY_DOUBLE;
}

void CodeGen::visitListFunction(ListFunction* listfunction)
{
    // Generate code for each function in turn.
    for (ListFunction::iterator i = listfunction->begin() ; i != listfunction->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void CodeGen::visitListStm(ListStm* liststm)
{
    // Generate code for each statement in turn.
    for (ListStm::iterator i = liststm->begin() ; i != liststm->end() ; ++i)
    {
        (*i)->accept(this);
    }
}

void CodeGen::visitListDecl(ListDecl* listdecl)
{
    // ListDecl is a function parameter list, so we can compute funargs here.
    funargs = listdecl->size();

    int currarg = 0;
    for (ListDecl::iterator i = listdecl->begin() ; i != listdecl->end() ; ++i)
    {
        (*i)->accept(this); // visitDec

        // The first argument (currarg = 0) has address -nargs; the last
        // (currarg = nargs - 1) has address -1.
        symbols[currid]->address() = currarg - funargs;
	symbols[currfun]->argtype() = currtype;
    }
}

void CodeGen::visitListIdent(ListIdent* listident)
{
    // Add all the identifiers to the symbol table.  Assumes currtype is
    // already set.
    for (ListIdent::iterator i = listident->begin(); i != listident->end(); ++i)
    {
        visitIdent(*i); // sets currid
        if (symbols.exists(currid))
        {
            throw Redeclared(currid);
        }
        // First local variable (numvars = funargs) has address 3, etc.
        // If this ListIdent is actually part of a parameter list, these
        // addresses will be fixed up by visitListDecl.
        symbols.insert(Symbol(currid, currtype, -1, 3 + symbols.numvars() - funargs, TY_BAD));
    }
}

void CodeGen::visitVarDec(VarDec* vardec)
{
    visitIdent(vardec->ident_);
    if (symbols.exists(currid))
    {
        throw Redeclared(currid);
    }
    symbols.insert(Symbol(currid, currtype, -1, 3 + symbols.numvars() - funargs, TY_BAD));
}

void CodeGen::visitListExp(ListExp* listexp)
{
    // Evaluate each expression in turn, leaving all the values on the stack.
    for (ListExp::iterator i = listexp->begin() ; i != listexp->end() ; ++i)
    {
        (*i)->accept(this);

    if(symbols[currfun]->type()==TY_FUNC && symbols[currfun]->numargs()!=-1){

//        if(symbols[currfun]->argtype()==TY_BAD)
  //            printf("THE CURRFUN TYPE IS BAD \n");
    //    if(symbols[currfun]->argtype()==TY_INT)
      //        printf("THE CURRFUN TYPE IS INT \n");
        //if(symbols[currfun]->argtype()==TY_DOUBLE)
          //    printf("THE CURRFUN TYPE IS DOUBLE \n");

        //if(currtype==TY_BAD)
          //    printf("THE expression TYPE IS BAD \n");
        //if(currtype==TY_GLOB)
          //    printf("THE expression TYPE IS GLOBAL \n");
        //if(currtype==TY_FUNC)
          //    printf("THE expression TYPE IS FUNC \n");
        //if(currtype==TY_INT)
          //    printf("THE expression TYPE IS INT \n");
        //if(currtype==TY_DOUBLE)
          //    printf("THE expression TYPE IS DOUBLE \n");


	 if(symbols[currfun]->argtype()!=currtype){
    		throw ArgError("A function does not have the appropriate type of arguments!\n");
		}
	}

    }
}

void CodeGen::visitListVar(ListVar* listvar)
{
    for (ListVar::iterator i = listvar->begin() ; i != listvar->end() ; ++i)
    {
        (*i)->accept(this);
    }
}


void CodeGen::visitInteger(Integer x)
{
    code.add(I_CONSTANT);
	currtype=TY_INT;
    code.add(x);
}

void CodeGen::visitChar(Char x)
{
    code.add(I_CONSTANT);
    code.add(x);
}

void CodeGen::visitDouble(Double x)
{
    code.add(R_CONSTANT);
	currtype=TY_DOUBLE;
    code.add(x);
}

void CodeGen::visitString(String x)
{
    code.add_string(x, code.pos() - 1);
}

void CodeGen::visitIdent(Ident x)
{
    currid = x;
}
