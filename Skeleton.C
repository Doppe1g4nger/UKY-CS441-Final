/*** BNFC-Generated Visitor Design Pattern Skeleton. ***/
/* This implements the common visitor design pattern.
   Note that this method uses Visitor-traversal of lists, so
   List->accept() does NOT traverse the list. This allows different
   algorithms to use context information differently. */

#include "Skeleton.H"



void Skeleton::visitProgram(Program* t) {} //abstract class
void Skeleton::visitFunction(Function* t) {} //abstract class
void Skeleton::visitDecl(Decl* t) {} //abstract class
void Skeleton::visitStm(Stm* t) {} //abstract class
void Skeleton::visitVar(Var* t) {} //abstract class
void Skeleton::visitExp(Exp* t) {} //abstract class
void Skeleton::visitType(Type* t) {} //abstract class

void Skeleton::visitProg(Prog *prog)
{
  /* Code For Prog Goes Here */

  prog->listfunction_->accept(this);

}

void Skeleton::visitFun(Fun *fun)
{
  /* Code For Fun Goes Here */

  fun->type_->accept(this);
  visitIdent(fun->ident_);
  fun->listdecl_->accept(this);
  fun->liststm_->accept(this);

}

void Skeleton::visitGlobal(Global *global)
{
  /* Code For Global Goes Here */

  global->type_->accept(this);
  visitIdent(global->ident_);

}

void Skeleton::visitDecA(DecA *deca)
{
  /* Code For DecA Goes Here */

  deca->type_->accept(this);
  deca->listvar_->accept(this);

}

void Skeleton::visitSDecl(SDecl *sdecl)
{
  /* Code For SDecl Goes Here */

  sdecl->decl_->accept(this);

}

void Skeleton::visitSExp(SExp *sexp)
{
  /* Code For SExp Goes Here */

  sexp->exp_->accept(this);

}

void Skeleton::visitSBlock(SBlock *sblock)
{
  /* Code For SBlock Goes Here */

  sblock->liststm_->accept(this);

}

void Skeleton::visitSWhile(SWhile *swhile)
{
  /* Code For SWhile Goes Here */

  swhile->exp_->accept(this);
  swhile->stm_->accept(this);

}

void Skeleton::visitSRepeat(SRepeat *srepeat)
{
  /* Code For SRepeat Goes Here */

  srepeat->stm_->accept(this);
  srepeat->exp_->accept(this);

}

void Skeleton::visitSFor(SFor *sfor)
{
  /* Code For SFor Goes Here */

  sfor->exp_1->accept(this);
  sfor->exp_2->accept(this);
  sfor->stm_->accept(this);

}

void Skeleton::visitSFor3(SFor3 *sfor3)
{
  /* Code For SFor3 Goes Here */

  sfor3->stm_1->accept(this);
  sfor3->exp_1->accept(this);
  sfor3->exp_2->accept(this);
  sfor3->stm_2->accept(this);

}

void Skeleton::visitSIf(SIf *sif)
{
  /* Code For SIf Goes Here */

  sif->exp_->accept(this);
  sif->stm_->accept(this);

}

void Skeleton::visitSIfElse(SIfElse *sifelse)
{
  /* Code For SIfElse Goes Here */

  sifelse->exp_->accept(this);
  sifelse->stm_1->accept(this);
  sifelse->stm_2->accept(this);

}

void Skeleton::visitSReturn(SReturn *sreturn)
{
  /* Code For SReturn Goes Here */

  sreturn->exp_->accept(this);

}

void Skeleton::visitVarAss(VarAss *varass)
{
  /* Code For VarAss Goes Here */

  visitIdent(varass->ident_);
  varass->exp_->accept(this);

}

void Skeleton::visitVarDec(VarDec *vardec)
{
  /* Code For VarDec Goes Here */

  visitIdent(vardec->ident_);

}

void Skeleton::visitEAss(EAss *eass)
{
  /* Code For EAss Goes Here */

  visitIdent(eass->ident_);
  eass->exp_->accept(this);

}

void Skeleton::visitEEq(EEq *eeq)
{
  /* Code For EEq Goes Here */

  eeq->exp_1->accept(this);
  eeq->exp_2->accept(this);

}

void Skeleton::visitENEq(ENEq *eneq)
{
  /* Code For ENEq Goes Here */

  eneq->exp_1->accept(this);
  eneq->exp_2->accept(this);

}

void Skeleton::visitEEqLt(EEqLt *eeqlt)
{
  /* Code For EEqLt Goes Here */

  eeqlt->exp_1->accept(this);
  eeqlt->exp_2->accept(this);

}

void Skeleton::visitELt(ELt *elt)
{
  /* Code For ELt Goes Here */

  elt->exp_1->accept(this);
  elt->exp_2->accept(this);

}

void Skeleton::visitEEqGt(EEqGt *eeqgt)
{
  /* Code For EEqGt Goes Here */

  eeqgt->exp_1->accept(this);
  eeqgt->exp_2->accept(this);

}

void Skeleton::visitEGt(EGt *egt)
{
  /* Code For EGt Goes Here */

  egt->exp_1->accept(this);
  egt->exp_2->accept(this);

}

void Skeleton::visitEAdd(EAdd *eadd)
{
  /* Code For EAdd Goes Here */

  eadd->exp_1->accept(this);
  eadd->exp_2->accept(this);

}

void Skeleton::visitESub(ESub *esub)
{
  /* Code For ESub Goes Here */

  esub->exp_1->accept(this);
  esub->exp_2->accept(this);

}

void Skeleton::visitEMul(EMul *emul)
{
  /* Code For EMul Goes Here */

  emul->exp_1->accept(this);
  emul->exp_2->accept(this);

}

void Skeleton::visitCall(Call *call)
{
  /* Code For Call Goes Here */

  visitIdent(call->ident_);
  call->listexp_->accept(this);

}

void Skeleton::visitEVar(EVar *evar)
{
  /* Code For EVar Goes Here */

  visitIdent(evar->ident_);

}

void Skeleton::visitEStr(EStr *estr)
{
  /* Code For EStr Goes Here */

  visitString(estr->string_);

}

void Skeleton::visitEInt(EInt *eint)
{
  /* Code For EInt Goes Here */

  visitInteger(eint->integer_);

}

void Skeleton::visitEDouble(EDouble *edouble)
{
  /* Code For EDouble Goes Here */

  visitDouble(edouble->double_);

}

void Skeleton::visitTInt(TInt *tint)
{
  /* Code For TInt Goes Here */


}

void Skeleton::visitTDouble(TDouble *tdouble)
{
  /* Code For TDouble Goes Here */


}


void Skeleton::visitListVar(ListVar* listvar)
{
  for (ListVar::iterator i = listvar->begin() ; i != listvar->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Skeleton::visitListFunction(ListFunction* listfunction)
{
  for (ListFunction::iterator i = listfunction->begin() ; i != listfunction->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Skeleton::visitListStm(ListStm* liststm)
{
  for (ListStm::iterator i = liststm->begin() ; i != liststm->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Skeleton::visitListDecl(ListDecl* listdecl)
{
  for (ListDecl::iterator i = listdecl->begin() ; i != listdecl->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Skeleton::visitListIdent(ListIdent* listident)
{
  for (ListIdent::iterator i = listident->begin() ; i != listident->end() ; ++i)
  {
    visitIdent(*i) ;
  }
}

void Skeleton::visitListExp(ListExp* listexp)
{
  for (ListExp::iterator i = listexp->begin() ; i != listexp->end() ; ++i)
  {
    (*i)->accept(this);
  }
}


void Skeleton::visitInteger(Integer x)
{
  /* Code for Integer Goes Here */
}

void Skeleton::visitChar(Char x)
{
  /* Code for Char Goes Here */
}

void Skeleton::visitDouble(Double x)
{
  /* Code for Double Goes Here */
}

void Skeleton::visitString(String x)
{
  /* Code for String Goes Here */
}

void Skeleton::visitIdent(Ident x)
{
  /* Code for Ident Goes Here */
}



