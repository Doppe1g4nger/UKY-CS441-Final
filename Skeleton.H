#ifndef SKELETON_HEADER
#define SKELETON_HEADER
/* You might want to change the above name. */

#include "Absyn.H"


class Skeleton : public Visitor
{
public:
  void visitProgram(Program* p);
  void visitFunction(Function* p);
  void visitDecl(Decl* p);
  void visitStm(Stm* p);
  void visitVar(Var* p);
  void visitExp(Exp* p);
  void visitType(Type* p);
  void visitProg(Prog* p);
  void visitFun(Fun* p);
  void visitGlobal(Global* p);
  void visitDecA(DecA* p);
  void visitSDecl(SDecl* p);
  void visitSExp(SExp* p);
  void visitSBlock(SBlock* p);
  void visitSWhile(SWhile* p);
  void visitSRepeat(SRepeat* p);
  void visitSFor(SFor* p);
  void visitSFor3(SFor3* p);
  void visitSIf(SIf* p);
  void visitSIfElse(SIfElse* p);
  void visitSReturn(SReturn* p);
  void visitVarAss(VarAss* p);
  void visitVarDec(VarDec* p);
  void visitEAss(EAss* p);
<<<<<<< HEAD
  void visitEAnd(EAnd* p);
  void visitEOr(EOr* p);
  void visitENot(ENot* p);
=======
>>>>>>> 1be0693fca677c0da5896a2c68f9642a1a33355a
  void visitEEq(EEq* p);
  void visitENEq(ENEq* p);
  void visitEEqLt(EEqLt* p);
  void visitELt(ELt* p);
  void visitEEqGt(EEqGt* p);
  void visitEGt(EGt* p);
  void visitEAdd(EAdd* p);
  void visitESub(ESub* p);
  void visitEMul(EMul* p);
  void visitCall(Call* p);
  void visitEVar(EVar* p);
  void visitEStr(EStr* p);
  void visitEInt(EInt* p);
  void visitEDouble(EDouble* p);
  void visitTInt(TInt* p);
  void visitTDouble(TDouble* p);
  void visitListVar(ListVar* p);
  void visitListFunction(ListFunction* p);
  void visitListStm(ListStm* p);
  void visitListDecl(ListDecl* p);
  void visitListIdent(ListIdent* p);
  void visitListExp(ListExp* p);

  void visit"Integer"("Integer" x);
  void visit"Char"("Char" x);
  void visit"Double"("Double" x);
  void visit"String"("String" x);
  void visit"Ident"("Ident" x);

};


#endif
