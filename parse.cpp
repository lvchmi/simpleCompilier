#include "globles.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

static TokenType token; /* holds current token */

/* function prototypes for recursive calls */
static TreeNode * statement_list(void);
static TreeNode * statement(void);
static TreeNode * if_stmt(void);
static TreeNode * for_stmt(void);
static TreeNode * while_stmt(void);
static TreeNode * assign_list(void);
static TreeNode * assign_stmt(void);
static TreeNode * var_stmt(void);
static TreeNode * exp(void);
static TreeNode * simple_exp(void);
static TreeNode * term(void);
static TreeNode * factor(void);

static void syntaxError(char * message)
{ fprintf(listing,"\n>>> ");
  fprintf(listing,"Syntax error at line %d: %s",lineno,message);
  Error = TRUE;
}

static void match(TokenType expected)
{ if (token == expected) token = getToken();
  else {
    syntaxError("mac unexpected token -> ");
    printToken(token,tokenString);
    fprintf(listing,"      ");
  }
}

TreeNode * statement_list(void)
{ TreeNode * t = statement();
  TreeNode * p = t;
  while ((token!=ENDFILE) &&(token!=ELSE))
  { TreeNode * q;
    q = statement();
    if (q!=NULL) {
      if (t==NULL) t = p = q;
      else /* now p cannot be NULL either */
      { p->sibling = q;
        p = q;
      }
    }
  }
  return t;
}

TreeNode * statement(void)
{ TreeNode * t = NULL;
  switch (token) {
    case IF : t = if_stmt(); break;
    case FOR : t = for_stmt(); break;
    case ID : t = assign_stmt(); break;
    case WHILE : t = while_stmt(); break;
    case INT :
	case CHAR :
		t = var_stmt(); break;
	case RBRACE : match(RBRACE); break;
    default : syntaxError("sta unexpected token -> ");
              printToken(token,tokenString);
              token = getToken();
              break;
  } /* end case */
  return t;
}

TreeNode * if_stmt(void)
{ TreeNode * t = newStmtNode(IfK);
  match(IF);match(LPAREN);
  if (t!=NULL) t->child[0] = exp();
  match(RPAREN);match(LBRACE);
  if (t!=NULL) t->child[1] = statement_list();
  //match(RBRACE);
  if (token==ELSE) {
    match(ELSE);match(LBRACE);
    if (t!=NULL) t->child[2] = statement_list();
    //match(RBRACE);
  }
  return t;
}

TreeNode * for_stmt(void)
{
	TreeNode * t = newStmtNode(ForK);
	match(FOR); match(LPAREN);
	if(t!=NULL) t->child[0] = assign_list();
	match(SEMI);
	if(t!=NULL) t->child[1] = exp();
	match(SEMI);
	if(t!=NULL) t->child[2] = assign_list();
	match(RPAREN); match(LBRACE);
	if(t!=NULL) t->child[3]=statement_list();
	//match(RBRACE);
	return t;
}

TreeNode * while_stmt(void)
{
	TreeNode * t = newStmtNode(WhileK);
	match(WHILE); match(LPAREN);
	if(t!=NULL) t->child[0] = exp();
	match(RPAREN); match(LBRACE);
	if(t!=NULL) t->child[1] = statement_list();
	//match(RBRACE);
	return t;
}

TreeNode * assign_list(void)
{
	TreeNode * t = newStmtNode(AssignK);
	if((t!=NULL)&&token==ID)
		t->attr.name=copyString(tokenString);
	match(ID); match(ASSIGN);
	if(t!=NULL) t->child[0] = exp();
	return t;
}

TreeNode * assign_stmt(void)
{
	TreeNode * t = assign_list();
	match(SEMI);
	return t;
}

TreeNode * var_stmt(void)
{
	TreeNode * t = newStmtNode(Vark);
	switch(token)
	{
		case INT : match(INT); break;
		case CHAR : match(CHAR); break;
	}
	if((t!=NULL)&&token==ID) t->attr.name=copyString(tokenString);
	match(ID); match(SEMI);
	return t;
}

TreeNode * exp(void)
{
	TreeNode * t = simple_exp();
	if((token == LT) || (token == EQ)){
		TreeNode * p = newExpNode(OpK);
		if(p!=NULL) {
			p->child[0]=t;
			p->attr.op=token;
			t=p;
		}
		match(token);
		if(t!=NULL) t->child[1]=simple_exp();
	}
	return t;
}

TreeNode * simple_exp(void)
{
	TreeNode * t = term();
  while ((token==PLUS)||(token==MINUS))
  { TreeNode * p = newExpNode(OpK);
    if (p!=NULL) {
      p->child[0] = t;
      p->attr.op = token;
      t = p;
      match(token);
      t->child[1] = term();
    }
  }
  return t;
}

TreeNode * term(void)
{ TreeNode * t = factor();
  while ((token==MUL)||(token==DIV))
  { TreeNode * p = newExpNode(OpK);
    if (p!=NULL) {
      p->child[0] = t;
      p->attr.op = token;
      t = p;
      match(token);
      p->child[1] = factor();
    }
  }
  return t;
}

TreeNode * factor(void)
{ TreeNode * t = NULL;
  switch (token) {
    case NUM :
      t = newExpNode(ConstK);
      if ((t!=NULL) && (token==NUM))
        t->attr.val = atoi(tokenString);
      match(NUM);
      break;
    case ID :
      t = newExpNode(IdK);
      if ((t!=NULL) && (token==ID))
        t->attr.name = copyString(tokenString);
      match(ID);
      break;
    case LPAREN :
      match(LPAREN);
      t = exp();
      match(RPAREN);
      break;
    default:
      syntaxError("fac unexpected token -> ");
      printToken(token,tokenString);
      token = getToken();
      break;
    }
  return t;
}

TreeNode * parse(void)
{ TreeNode * t;
  token = getToken();
  t = statement_list();
  if (token!=ENDFILE)
    syntaxError("Code ends before file\n");
  return t;
}