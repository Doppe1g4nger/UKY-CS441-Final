#ifndef CODEGEN_H_INCLUDED
#define CODEGEN_H_INCLUDED

#include "Absyn.H"
#include "pstcode.h"
#include <stdexcept>
#include <string>

class UnknownVar : public std::logic_error
{
public:
    UnknownVar(const std::string &name)
        : logic_error("Unknown variable \"" + name + "\"")
    {}
};

class UnknownFunc : public std::logic_error
{
public:
    UnknownFunc(const std::string &name)
        : logic_error("Unknown function \"" + name + "\"")
    {}
};

class Redeclared : public std::logic_error
{
public:
    Redeclared(const std::string &name)
        : logic_error("Symbol \"" + name + "\" redeclared")
    {}
};

class Unimplemented : public std::logic_error
{
public:
    Unimplemented(const std::string &what) : logic_error(what) {}
};

class ArgError : public std::logic_error
{
public:
    ArgError(const std::string &what) : logic_error(what) {}
};

class TypeError : public std::logic_error
{
public:
    TypeError(const std::string &what) 
        : logic_error("Error: Type of" + what + "Does not match") 
        {}
};

class CodeGen : public Visitor
{
private:
    Ident currid;        // identifier from last visitIdent
    type_t currtype;     // type from last visitT{Int,Double}
    PstackCode code;     // buffer to hold generated code
    SymbolTable symbols; // symbol table
    int funargs;
    type_t fun_type;
                         // number of parameters in current function.
public:
    CodeGen()
        : currid(""), currtype(TY_BAD), code(), symbols(), funargs(-1), fun_type(TY_BAD)
    {}
    PstackCode generate(Visitable *vis);

    // These will never actually be called; instead, node->accept(this) will
    // call the method for the concrete class (visitProg rather than
    // visitProgram, visitTInt or visitTDouble rather than visitType, etc.)
    void visitProgram(Program *) {}
    void visitFunction(Function *) {}
    void visitDecl(Decl *) {}
    void visitStm(Stm *) {}
    void visitExp(Exp *) {}
    void visitType(Type *) {}
    void visitVar(Var *) {}

    void visitVarAss(VarAss *p);
    void visitVarDec(VarDec *p);
    void visitProg(Prog *p);
    void visitGlobal(Global *p);
    void visitFun(Fun *p);
    //void visitDec(Dec *p);
    void visitDecA(DecA *p);
    void visitListFunction(ListFunction* p);
    void visitListExp(ListExp* p);
    void visitListStm(ListStm* p);
    void visitListDecl(ListDecl* p);
    void visitListIdent(ListIdent* p);
    void visitListVar(ListVar* p);

    // Statements
    void visitSDecl(SDecl *p);
    void visitSExp(SExp *p);
    void visitSBlock(SBlock *p);
    void visitSWhile(SWhile *p);
    void visitSRepeat(SRepeat *p);
    void visitSIf(SIf *p);
    void visitSFor(SFor *p);
    void visitSFor3(SFor3 *p);
    void visitSIfElse(SIfElse *p);
    void visitSReturn(SReturn *p);

    // Expressions
    void visitEAss(EAss *p);
    void visitEEq(EEq *p);
    void visitEAnd(EAnd *p);
    void visitEOr(EOr *p);
    void visitENot(ENot *p);
    void visitENEq(ENEq *p);
    void visitELt(ELt *p);
    void visitEEqLt(EEqLt *p);
    void visitEGt(EGt *p);
    void visitEEqGt(EEqGt *p);
    void visitEAdd(EAdd *p);
    void visitESub(ESub *p);
    void visitEMul(EMul *p);
    void visitCall(Call *p);
    void visitEVar(EVar *p);
    void visitEStr(EStr *p);
    void visitEInt(EInt *p);
    void visitEDouble(EDouble *p);


    // Types
    void visitTInt(TInt *p);
    void visitTDouble(TDouble *p);

    // Literals and identifiers.
    void visitInteger(Integer i);
    void visitDouble(Double d);
    void visitChar(Char c);
    void visitString(String s);
    void visitIdent(String s);
};

#endif

