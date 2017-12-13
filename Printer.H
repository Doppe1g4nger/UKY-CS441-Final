#ifndef PRINTER_HEADER
#define PRINTER_HEADER

#include "Absyn.H"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/* Certain applications may improve performance by changing the buffer size */
#define BUFFER_INITIAL 2000
/* You may wish to change _L_PAREN or _R_PAREN */
#define _L_PAREN '('
#define _R_PAREN ')'

class PrintAbsyn : public Visitor
{
 protected:
  int _n_, _i_;
  /* The following are simple heuristics for rendering terminals */
  /* You may wish to change them */
  void render(Char c);
  void render(String s);
void render(char* s);
  void indent(void);
  void backup(void);
 public:
  PrintAbsyn(void);
  ~PrintAbsyn(void);
  char* print(Visitable* v);

  void visitProgram(Program *p); /* abstract class */
  void visitProg(Prog *p);
  void visitFunction(Function *p); /* abstract class */
  void visitFun(Fun *p);
  void visitDecl(Decl *p); /* abstract class */
  void visitDecA(DecA *p);
  void visitListVar(ListVar* p);
  void visitListFunction(ListFunction* p);
  void visitListStm(ListStm* p);
  void visitListDecl(ListDecl* p);
  void visitListIdent(ListIdent* p);
  void visitStm(Stm *p); /* abstract class */
  void visitSDecl(SDecl *p);
  void visitSExp(SExp *p);
  void visitSBlock(SBlock *p);
  void visitSWhile(SWhile *p);
  void visitSRepeat(SRepeat *p);
  void visitSFor(SFor *p);
  void visitSFor3(SFor3 *p);
  void visitSIf(SIf *p);
  void visitSIfElse(SIfElse *p);
  void visitSReturn(SReturn *p);
  void visitVar(Var *p); /* abstract class */
  void visitVarAss(VarAss *p);
  void visitVarDec(VarDec *p);
  void visitExp(Exp *p); /* abstract class */
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
  void visitListExp(ListExp* p);
  void visitType(Type *p); /* abstract class */
  void visitTInt(TInt *p);
  void visitTDouble(TDouble *p);

  void visitInteger(Integer i);
  void visitDouble(Double d);
  void visitChar(Char c);
  void visitString(String s);
  void visitIdent(String s);
 protected:
  void inline bufAppend(const char* s)
  {
    int len = strlen(s);
    while (cur_ + len > buf_size)
    {
      buf_size *= 2; /* Double the buffer size */
      resizeBuffer();
    }
    for(int n = 0; n < len; n++)
    {
      buf_[cur_ + n] = s[n];
    }
    cur_ += len;
    buf_[cur_] = 0;
  }
  void inline bufAppend(const char c)
  {
    if (cur_ == buf_size)
    {
      buf_size *= 2; /* Double the buffer size */
      resizeBuffer();
    }
    buf_[cur_] = c;
    cur_++;
    buf_[cur_] = 0;
  }
  void inline bufAppend(String str)
  {
    const char* s = str.c_str();
    bufAppend(s);
  }
  void inline bufReset(void)
  {
    cur_ = 0;
    buf_size = BUFFER_INITIAL;
    resizeBuffer();
    memset(buf_, 0, buf_size);
  }
  void inline resizeBuffer(void)
  {
    char* temp = (char*) malloc(buf_size);
    if (!temp)
    {
      fprintf(stderr, "Error: Out of memory while attempting to grow buffer!\n");
      exit(1);
    }
    if (buf_)
    {
      strcpy(temp, buf_);
      free(buf_);
    }
    buf_ = temp;
  }
  char *buf_;
  int cur_, buf_size;
};



class ShowAbsyn : public Visitor
{
 public:
  ShowAbsyn(void);
  ~ShowAbsyn(void);
  char* show(Visitable* v);

  void visitProgram(Program *p); /* abstract class */
  void visitProg(Prog *p);
  void visitFunction(Function *p); /* abstract class */
  void visitFun(Fun *p);
  void visitDecl(Decl *p); /* abstract class */
  void visitDecA(DecA *p);
  void visitListVar(ListVar* p);
  void visitListFunction(ListFunction* p);
  void visitListStm(ListStm* p);
  void visitListDecl(ListDecl* p);
  void visitListIdent(ListIdent* p);
  void visitStm(Stm *p); /* abstract class */
  void visitSDecl(SDecl *p);
  void visitSExp(SExp *p);
  void visitSBlock(SBlock *p);
  void visitSWhile(SWhile *p);
  void visitSRepeat(SRepeat *p);
  void visitSFor(SFor *p);
  void visitSFor3(SFor3 *p);
  void visitSIf(SIf *p);
  void visitSIfElse(SIfElse *p);
  void visitSReturn(SReturn *p);
  void visitVar(Var *p); /* abstract class */
  void visitVarAss(VarAss *p);
  void visitVarDec(VarDec *p);
  void visitExp(Exp *p); /* abstract class */
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
  void visitListExp(ListExp* p);
  void visitType(Type *p); /* abstract class */
  void visitTInt(TInt *p);
  void visitTDouble(TDouble *p);

  void visitInteger(Integer i);
  void visitDouble(Double d);
  void visitChar(Char c);
  void visitString(String s);
  void visitIdent(String s);
 protected:
  void inline bufAppend(const char* s)
  {
    int len = strlen(s);
    while (cur_ + len > buf_size)
    {
      buf_size *= 2; /* Double the buffer size */
      resizeBuffer();
    }
    for(int n = 0; n < len; n++)
    {
      buf_[cur_ + n] = s[n];
    }
    cur_ += len;
    buf_[cur_] = 0;
  }
  void inline bufAppend(const char c)
  {
    if (cur_ == buf_size)
    {
      buf_size *= 2; /* Double the buffer size */
      resizeBuffer();
    }
    buf_[cur_] = c;
    cur_++;
    buf_[cur_] = 0;
  }
  void inline bufAppend(String str)
  {
    const char* s = str.c_str();
    bufAppend(s);
  }
  void inline bufReset(void)
  {
    cur_ = 0;
    buf_size = BUFFER_INITIAL;
    resizeBuffer();
    memset(buf_, 0, buf_size);
  }
  void inline resizeBuffer(void)
  {
    char* temp = (char*) malloc(buf_size);
    if (!temp)
    {
      fprintf(stderr, "Error: Out of memory while attempting to grow buffer!\n");
      exit(1);
    }
    if (buf_)
    {
      strcpy(temp, buf_);
      free(buf_);
    }
    buf_ = temp;
  }
  char *buf_;
  int cur_, buf_size;
};




#endif

