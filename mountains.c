/*
 * A n t l r  T r a n s l a t i o n  H e a d e r
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * With AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR33
 *
 *   antlr -gt mountains.g
 *
 */

#define ANTLR_VERSION	13333
#include "pcctscfg.h"
#include "pccts_stdio.h"

#include <string>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

// struct to store information about tokens
typedef struct {
  string kind;
  string text;
} Attrib;

// function to fill token information (predeclaration)
void zzcr_attr(Attrib *attr, int type, char *text);

// fields for AST nodes
#define AST_FIELDS string kind; string text;
#include "ast.h"

// macro to create a new AST node (and function predeclaration)
#define zzcr_ast(as,attr,ttype,textt) as=createASTnode(attr,ttype,textt)
AST* createASTnode(Attrib* attr, int ttype, char *textt);
#define GENAST

#include "ast.h"

#define zzSET_SIZE 8
#include "antlr.h"
#include "tokens.h"
#include "dlgdef.h"
#include "mode.h"

/* MR23 In order to remove calls to PURIFY use the antlr -nopurify option */

#ifndef PCCTS_PURIFY
#define PCCTS_PURIFY(r,s) memset((char *) &(r),'\0',(s));
#endif

#include "ast.c"
zzASTgvars

ANTLR_INFO

#include <cstdlib>
#include <cmath>

// function to fill token information
void zzcr_attr(Attrib *attr, int type, char *text) {
  if (type == NUM) {
    attr->kind = "intconst";
    attr->text = text;
  } else if (type == MID || type == VAR) {
    attr->kind = "id";
    attr->text = text;
  } else {
    attr->kind = text;
    attr->text = "";
  }
}

// function to create a new AST node
AST* createASTnode(Attrib* attr, int type, char* text) {
  AST* as = new AST;
  as->kind = attr->kind; 
  as->text = attr->text;
  as->right = NULL; 
  as->down = NULL;
  return as;
}

/// create a new "list" AST node with one element
AST* createASTlist(AST *child) {
  AST *as = new AST;
  as->kind = "list";
  as->right = NULL;
  as->down = child;
  return as;
}

/// get nth child of a tree. Count starts at 0.
/// if no such child, returns NULL
AST* child(AST *a, int n) {
  AST *c = a->down;
  for (int i = 0; c != NULL && i < n; i++) {
    c = c->right;
  }
  return c;
} 

/// print AST, recursively, with indentation
void ASTPrintIndent(AST *a,string s) {
  if (a == NULL) {
    return;
  }
  
    cout << a->kind;
  if (a->text != "") {
    cout<<"("<<a->text<<")";
  }
  cout << endl;
  
    AST *i = a->down;
  while (i != NULL && i->right != NULL) {
    cout << s+"  \\__";
    ASTPrintIndent(i, s + "  |" + string(i->kind.size() + i->text.size(), ' '));
    i = i->right;
  }
  
    if (i != NULL) {
    cout << s+"  \\__";
    ASTPrintIndent(i, s + "   " + string(i->kind.size() + i->text.size(), ' '));
    i = i->right;
  }
}

/// print AST 
void ASTPrint(AST *a) {
  while (a != NULL) {
    cout << " ";
    ASTPrintIndent(a, "");
    a = a->right;
  }
}

// * ------------------------------------------------------------------------------- * 
// * ------------------------------------------------------------------------------- *
// * ------------------------------------------------------------------------------- * 
// * ---------------------------------- MOUNTAINS ---------------------------------- *
// * ------------------------------------------------------------------------------- *
// * ------------------------------------------------------------------------------- *
// * ------------------------------------------------------------------------------- *


// * ------------------------------------------------------------------------------- *
// * -------------------------------- INTERPRETACIÓ -------------------------------- *
// * ------------------------------------------------------------------------------- *


// * ---------------------------------- DEFINICIÓ ---------------------------------- *

// mountain element structure, type can be: "/", "-" or "\"
typedef struct {
int num;
string type;
} Element;

// an array of mountain elements is a mountain
typedef vector<Element> Mountain;

// Earth stores all the mountains by name
typedef map<string, Mountain> Earth;

// store all the integer variables by name
typedef map<string, int> Variables;

void evaluate(AST *a);
int evaluate_int(AST *a);
bool evaluate_bool(AST *a);

void draw(Mountain mountain);
void draw(string mountain_id);
bool wellformed(string mountain_id);
void complete(string mountain_id);
int heigth(string mountain_id);
bool match(string mountain_id1, string mountain_id2);
Mountain peak(AST *p);
Mountain valley(AST *v);


Earth earth;
Variables variables;


// * ----------------------------- FUNCIONS AUXILIATS ------------------------------ *

void log(string text) {
cout <<"[MOUNTAINS]: " <<text <<endl;
}

// return a mountain concatenation [left:right]
Mountain concat(Mountain left, Mountain right) {
int total_size = left.size() + right.size();
Mountain res (total_size);
for (int i = 0; i < left.size(); ++i) {
res[i] = left[i];
}
for (int i = 0; i < right.size(); ++i) {
res[i + left.size()] = right[i];
}
return res;
}

// stores mountain in the earth with name
void store(string name, Mountain mountain) {
earth[name] = mountain;
}

// stores the integer variable var in the variable map with name
void store(string name, int var) {
variables[name] = var;
}

Mountain mountain_from_ats(AST *m) {
if (m->kind == ";") {
Mountain left_mountain = mountain_from_ats(child(m, 0));
Mountain right_mountain = mountain_from_ats(child(m, 1));
return concat(left_mountain, right_mountain);
} else if (m->kind == "*") {
Mountain new_mountain (1);
new_mountain[0].num = evaluate_int(child(m, 0));
AST *type = child(m, 1);
new_mountain[0].type = type->kind.c_str();
return new_mountain;
} else if (m->kind == "id") {
return earth[m->text];
} else if (m->kind == "Peak") {
return peak(m);
} else if (m->kind == "Valley") {
return valley(m);
}
}

Mountain expand_mountain(Mountain m, int extra) {
Mountain res (m.size() + extra);
for (int i = 0; i < m.size(); ++i) {
res[i] = m[i];
}
for (int i = m.size(); i < res.size(); ++i) {
res[i].num = 1;
res[i].type = "-";
}
return res;
}


// * -------------------------------- IMPLEMENTACIÓ -------------------------------- *

void draw(Mountain mountain) {
for (int i = 0; i < mountain.size(); ++i) {
for (int j = 0; j < mountain[i].num; ++j) {
cout <<mountain[i].type;
}
}
cout <<endl;
}

void draw(string mountain_id) {
draw(earth[mountain_id]);
}

bool wellformed(string mountain_id) {
return earth[mountain_id].size() % 3 == 0;
}

void complete(string mountain_id) {
Mountain incomplete_mountain = earth[mountain_id];
int size = incomplete_mountain.size();
int extra = 3 - (size % 3);
size = size + extra;
Mountain new_mountain = expand_mountain(incomplete_mountain, extra);
if (new_mountain[size - 3].type == "\\") {
new_mountain[size - 1].type = "/";
} else {
new_mountain[size - 1].type = "\\";
}
earth[mountain_id] = new_mountain;
}

int heigth(string mountain_id) {
Mountain mountain = earth[mountain_id];
int top = 0;
int bottom = 0;
int current = 0;
for (int i = 0; i < mountain.size(); ++i) {
if (mountain[i].type == "/") {
current += mountain[i].num;
} else if (mountain[i].type == "\\") {
current -= mountain[i].num;
}

        if (current > top) {
top = current;
}
if (current < bottom) {
bottom = current;
}
}
return top - bottom;
}

bool match(string mountain_id1, string mountain_id2) {
return heigth(mountain_id1) == heigth(mountain_id2);
}

Mountain peak(AST *p) {
Mountain res (3);
res[0].num = evaluate_int(child(p, 0));
res[1].num = evaluate_int(child(p, 1));
res[2].num = evaluate_int(child(p, 2));

    res[0].type = "/";
res[1].type = "-";
res[2].type = "\\";

    return res;
}

Mountain valley(AST *v) {
Mountain res (3);
res[0].num = evaluate_int(child(v, 0));
res[1].num = evaluate_int(child(v, 1));
res[2].num = evaluate_int(child(v, 2));

    res[0].type = "\\";
res[1].type = "-";
res[2].type = "/";

    return res;
}

int evaluate_int(AST *a) {
if (a != NULL) {
if (a->kind == "intconst") {
return atoi(a->text.c_str());
} else if (a->kind == "id") {
return variables[a->text];
} else if (a->kind == "+") {
return evaluate_int(child(a, 0)) + evaluate_int(child(a, 1));
} else if (a->kind == "Height") {
return heigth(child(a, 0)->text);
}
}
}

bool evaluate_bool(AST *a) {
if (a != NULL) {
if (a->kind == "NOT") {
return !evaluate_bool(child(a, 0));
} else if (a->kind == "OR") {
return evaluate_bool(child(a, 0)) || evaluate_bool(child(a, 1));
} else if (a->kind == "AND") {
return evaluate_bool(child(a, 0)) && evaluate_bool(child(a, 1));
} else if (a->kind == "=") {
return evaluate_int(child(a, 0)) == evaluate_int(child(a, 1));
} else if (a->kind == ">") {
return evaluate_int(child(a, 0)) > evaluate_int(child(a, 1));
} else if (a->kind == "<") {
return evaluate_int(child(a, 0)) < evaluate_int(child(a, 1));
} else if (a->kind == "Match") {
return match(child(a, 0)->text, child(a, 1)->text);
} else if (a->kind == "Wellformed") {
return wellformed(child(a, 0)->text);
}
}
}

void evaluate(AST *a) {
if (a != NULL) {
if (a->kind == "is") {
string name = child(a, 0)->text;
if (name[0] == 'M') {
store(child(a, 0)->text, mountain_from_ats(child(a, 1)));
} else {
store(child(a, 0)->text, evaluate_int(child(a, 1)));
}
} else if (a->kind == "intconst") {
atoi(a->text.c_str());
} else if (a->kind == "list") {
for (int i = 0; child(a, i) != NULL; ++i) {
evaluate(child(a, i));
}
} else if (a->kind == "Draw") {
if (child(a, 0)->kind == "id") {
draw(child(a, 0)->text);
} else {
draw(mountain_from_ats(child(a, 0)));
}
} else if (a->kind == "Complete") {
complete(child(a, 0)->text);
} else if (a->kind == "if") {
if (evaluate_bool(child(a, 0))) {
evaluate(child(a, 1));
}
} else if (a->kind == "while") {
while (evaluate_bool(child(a, 0))) {
evaluate(child(a, 1));
}
}
}
}

int main() {
AST *root = NULL;
ANTLR(input(&root), stdin);
ASTPrint(root);
evaluate(root);

    cout <<"----------------------------------------" <<endl;

    for (Earth::iterator it = earth.begin(); it != earth.end(); ++it) {
cout <<"l'altitud final de " <<it->first <<" és: ";
cout <<to_string(heigth(it->first)) <<endl;
}
}

  

void
#ifdef __USE_PROTOS
input(AST**_root)
#else
input(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  expr(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(1);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x1);
  }
}

void
#ifdef __USE_PROTOS
expr(AST**_root)
#else
expr(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (setwd1[LA(1)]&0x2) ) {
      content(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  (*_root)=createASTlist(_sibling);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x4);
  }
}

void
#ifdef __USE_PROTOS
content(AST**_root)
#else
content(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==VAR) ) {
      val_assig(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (LA(1)==MID) ) {
        assignation(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {
        if ( (setwd1[LA(1)]&0x8) ) {
          statements(zzSTR); zzlink(_root, &_sibling, &_tail);
        }
        else {
          if ( (LA(1)==DRAW) ) {
            do_draw(zzSTR); zzlink(_root, &_sibling, &_tail);
          }
          else {
            if ( (LA(1)==COMPLETE) ) {
              do_complete(zzSTR); zzlink(_root, &_sibling, &_tail);
            }
            else {zzFAIL(1,zzerr1,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
          }
        }
      }
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x10);
  }
}

void
#ifdef __USE_PROTOS
statements(AST**_root)
#else
statements(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==WHILE) ) {
      while_statement(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (LA(1)==IF) ) {
        if_statement(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {zzFAIL(1,zzerr2,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x20);
  }
}

void
#ifdef __USE_PROTOS
if_statement(AST**_root)
#else
if_statement(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(IF); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  block(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(ENDIF);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x40);
  }
}

void
#ifdef __USE_PROTOS
while_statement(AST**_root)
#else
while_statement(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(WHILE); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  block(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(ENDWHILE);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x80);
  }
}

void
#ifdef __USE_PROTOS
block(AST**_root)
#else
block(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(LEFT_P);  zzCONSUME;
  boolean(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(RIGHT_P);  zzCONSUME;
  expr(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x1);
  }
}

void
#ifdef __USE_PROTOS
boolean(AST**_root)
#else
boolean(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==NOT) ) {
      zzmatch(NOT); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {
      if ( (setwd2[LA(1)]&0x2) ) {
      }
      else {zzFAIL(1,zzerr3,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  or_boolean(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x4);
  }
}

void
#ifdef __USE_PROTOS
or_boolean(AST**_root)
#else
or_boolean(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  and_boolean(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (LA(1)==OR) ) {
      zzmatch(OR); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      and_boolean(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x8);
  }
}

void
#ifdef __USE_PROTOS
and_boolean(AST**_root)
#else
and_boolean(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  boolean_op(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (LA(1)==AND) ) {
      zzmatch(AND); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      boolean_op(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x10);
  }
}

void
#ifdef __USE_PROTOS
boolean_op(AST**_root)
#else
boolean_op(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==MATCH) ) {
      bool_match(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (LA(1)==WELLFORMED) ) {
        bool_wellformed(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {
        if ( (setwd2[LA(1)]&0x20) ) {
          comparation(zzSTR); zzlink(_root, &_sibling, &_tail);
        }
        else {zzFAIL(1,zzerr4,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
      }
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x40);
  }
}

void
#ifdef __USE_PROTOS
comparation(AST**_root)
#else
comparation(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  numeric_value(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==EQUAL) ) {
      zzmatch(EQUAL); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {
      if ( (LA(1)==GREATER) ) {
        zzmatch(GREATER); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      }
      else {
        if ( (LA(1)==LESSER) ) {
          zzmatch(LESSER); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        }
        else {zzFAIL(1,zzerr5,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
      }
    }
    zzEXIT(zztasp2);
    }
  }
  numeric_value(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x80);
  }
}

void
#ifdef __USE_PROTOS
numeric_value(AST**_root)
#else
numeric_value(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==HEIGHT) ) {
      int_heigth(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (LA(1)==VAR) ) {
        zzmatch(VAR); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
      }
      else {
        if ( (LA(1)==NUM) ) {
          zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
        }
        else {zzFAIL(1,zzerr6,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
      }
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x1);
  }
}

void
#ifdef __USE_PROTOS
int_heigth(AST**_root)
#else
int_heigth(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(HEIGHT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(LEFT_P);  zzCONSUME;
  variable(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(RIGHT_P);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x2);
  }
}

void
#ifdef __USE_PROTOS
bool_match(AST**_root)
#else
bool_match(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(MATCH); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(LEFT_P);  zzCONSUME;
  variable(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(COMMA);  zzCONSUME;
  variable(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(RIGHT_P);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x4);
  }
}

void
#ifdef __USE_PROTOS
bool_wellformed(AST**_root)
#else
bool_wellformed(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(WELLFORMED); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(LEFT_P);  zzCONSUME;
  zzmatch(MID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(RIGHT_P);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x8);
  }
}

void
#ifdef __USE_PROTOS
do_draw(AST**_root)
#else
do_draw(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(DRAW); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(LEFT_P);  zzCONSUME;
  mountain(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(RIGHT_P);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x10);
  }
}

void
#ifdef __USE_PROTOS
do_complete(AST**_root)
#else
do_complete(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(COMPLETE); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(LEFT_P);  zzCONSUME;
  zzmatch(MID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(RIGHT_P);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x20);
  }
}

void
#ifdef __USE_PROTOS
val_assig(AST**_root)
#else
val_assig(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(VAR); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(ASSIG); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  val(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x40);
  }
}

void
#ifdef __USE_PROTOS
assignation(AST**_root)
#else
assignation(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(MID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(ASSIG); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  mountain(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x80);
  }
}

void
#ifdef __USE_PROTOS
new_peak(AST**_root)
#else
new_peak(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(PEAK); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(LEFT_P);  zzCONSUME;
  val(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(COMMA);  zzCONSUME;
  val(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(COMMA);  zzCONSUME;
  val(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(RIGHT_P);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd4, 0x1);
  }
}

void
#ifdef __USE_PROTOS
new_valley(AST**_root)
#else
new_valley(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(VALLEY); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(LEFT_P);  zzCONSUME;
  val(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(COMMA);  zzCONSUME;
  val(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(COMMA);  zzCONSUME;
  val(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(RIGHT_P);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd4, 0x2);
  }
}

void
#ifdef __USE_PROTOS
mountain(AST**_root)
#else
mountain(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  mountain_elem(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (LA(1)==SEMICOLON) ) {
      zzmatch(SEMICOLON); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      mountain_elem(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd4, 0x4);
  }
}

void
#ifdef __USE_PROTOS
mountain_elem(AST**_root)
#else
mountain_elem(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==NUM) ) {
      elem(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (LA(1)==HASH) ) {
        variable(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {
        if ( (LA(1)==PEAK) ) {
          new_peak(zzSTR); zzlink(_root, &_sibling, &_tail);
        }
        else {
          if ( (LA(1)==VALLEY) ) {
            new_valley(zzSTR); zzlink(_root, &_sibling, &_tail);
          }
          else {zzFAIL(1,zzerr7,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
      }
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd4, 0x8);
  }
}

void
#ifdef __USE_PROTOS
val(AST**_root)
#else
val(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==NUM) ) {
      zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {
      if ( (LA(1)==VAR) ) {
        zzmatch(VAR); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
        {
          zzBLOCK(zztasp3);
          zzMake0;
          {
          if ( (LA(1)==ADD) ) {
            zzmatch(ADD); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
            {
              zzBLOCK(zztasp4);
              zzMake0;
              {
              if ( (LA(1)==NUM) ) {
                zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
              }
              else {
                if ( (LA(1)==VAR) ) {
                  zzmatch(VAR); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
                }
                else {zzFAIL(1,zzerr8,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
              }
              zzEXIT(zztasp4);
              }
            }
          }
          else {
            if ( (setwd4[LA(1)]&0x10) ) {
            }
            else {zzFAIL(1,zzerr9,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
          }
          zzEXIT(zztasp3);
          }
        }
      }
      else {zzFAIL(1,zzerr10,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd4, 0x20);
  }
}

void
#ifdef __USE_PROTOS
elem(AST**_root)
#else
elem(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(ASTERISK); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==BACKSLASH) ) {
      zzmatch(BACKSLASH); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {
      if ( (LA(1)==DASH) ) {
        zzmatch(DASH); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
      }
      else {
        if ( (LA(1)==SLASH) ) {
          zzmatch(SLASH); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
        }
        else {zzFAIL(1,zzerr11,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
      }
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd4, 0x40);
  }
}

void
#ifdef __USE_PROTOS
variable(AST**_root)
#else
variable(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(HASH);  zzCONSUME;
  zzmatch(MID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd4, 0x80);
  }
}
