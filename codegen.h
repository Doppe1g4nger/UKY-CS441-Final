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

class CodeGen : public Visitor
{
private:
    Ident currid;        // identifier from last visitIdent
    type_t currtype;     // type from last visitT{Int,Double}
    PstackCode code;     // buffer to hold generated code
    SymbolTable symbols; // symbol table
    int funargs;         // number of parameters in current function.
public:
    CodeGen()
        : currid(""), currtype(TY_BAD), code(), symbols(), funargs(-1)
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

    void visitProg(Prog *p);
    void visitFun(Fun *p);
    void visitDec(Dec *p);
    void visitListFunction(ListFunction* p);
    void visitListExp(ListExp* p);
    void visitListStm(ListStm* p);
    void visitListDecl(ListDecl* p);
    void visitListIdent(ListIdent* p);

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
    void visitELt(ELt *p);
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

