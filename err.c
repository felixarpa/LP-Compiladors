/*
 * A n t l r  S e t s / E r r o r  F i l e  H e a d e r
 *
 * Generated from: mountains.g
 *
 * Terence Parr, Russell Quong, Will Cohen, and Hank Dietz: 1989-2001
 * Parr Research Corporation
 * with Purdue University Electrical Engineering
 * With AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR33
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
#define zzSET_SIZE 8
#include "antlr.h"
#include "ast.h"
#include "tokens.h"
#include "dlgdef.h"
#include "err.h"

ANTLRChar *zztokens[34]={
	/* 00 */	"Invalid",
	/* 01 */	"@",
	/* 02 */	"BACKSLASH",
	/* 03 */	"DASH",
	/* 04 */	"SLASH",
	/* 05 */	"ASTERISK",
	/* 06 */	"HASH",
	/* 07 */	"SEMICOLON",
	/* 08 */	"COMMA",
	/* 09 */	"ADD",
	/* 10 */	"LEFT_P",
	/* 11 */	"RIGHT_P",
	/* 12 */	"EQUAL",
	/* 13 */	"GREATER",
	/* 14 */	"LESSER",
	/* 15 */	"NOT",
	/* 16 */	"AND",
	/* 17 */	"OR",
	/* 18 */	"WHILE",
	/* 19 */	"ENDWHILE",
	/* 20 */	"IF",
	/* 21 */	"ENDIF",
	/* 22 */	"HEIGHT",
	/* 23 */	"WELLFORMED",
	/* 24 */	"MATCH",
	/* 25 */	"COMPLETE",
	/* 26 */	"DRAW",
	/* 27 */	"VALLEY",
	/* 28 */	"PEAK",
	/* 29 */	"ASSIG",
	/* 30 */	"MID",
	/* 31 */	"NUM",
	/* 32 */	"VAR",
	/* 33 */	"SPACE"
};
SetWordType zzerr1[8] = {0x0,0x0,0x14,0x46, 0x1,0x0,0x0,0x0};
SetWordType zzerr2[8] = {0x0,0x0,0x14,0x0, 0x0,0x0,0x0,0x0};
SetWordType setwd1[34] = {0x0,0xf5,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0xfa,0xf4,0xfa,0xf4,0x0,
	0x0,0x0,0xf2,0xf2,0x0,0x0,0x0,0xf2,
	0x0,0xf2,0x0};
SetWordType zzerr3[8] = {0x0,0x80,0xc0,0x81, 0x1,0x0,0x0,0x0};
SetWordType zzerr4[8] = {0x0,0x0,0xc0,0x81, 0x1,0x0,0x0,0x0};
SetWordType zzerr5[8] = {0x0,0x70,0x0,0x0, 0x0,0x0,0x0,0x0};
SetWordType zzerr6[8] = {0x0,0x0,0x40,0x80, 0x1,0x0,0x0,0x0};
SetWordType setwd2[34] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0xdc,0x0,0x0,0x0,
	0x0,0xc0,0xd0,0x0,0x1,0x0,0x1,0x22,
	0x2,0x2,0x0,0x0,0x0,0x0,0x0,0x0,
	0x22,0x22,0x0};
SetWordType setwd3[34] = {0x0,0xf0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0xf,0x3,0x3,0x3,
	0x0,0xf,0xf,0xf0,0xf0,0xf0,0xf0,0x0,
	0x0,0x0,0xf0,0xf0,0x0,0x0,0x0,0xf0,
	0x0,0xf0,0x0};
SetWordType zzerr7[8] = {0x40,0x0,0x0,0x98, 0x0,0x0,0x0,0x0};
SetWordType zzerr8[8] = {0x0,0x0,0x0,0x80, 0x1,0x0,0x0,0x0};
SetWordType zzerr9[8] = {0x2,0xb,0x3c,0x46, 0x1,0x0,0x0,0x0};
SetWordType zzerr10[8] = {0x0,0x0,0x0,0x80, 0x1,0x0,0x0,0x0};
SetWordType zzerr11[8] = {0x1c,0x0,0x0,0x0, 0x0,0x0,0x0,0x0};
SetWordType setwd4[34] = {0x0,0xff,0x0,0x0,0x0,0x0,0x0,
	0xcb,0xb0,0x0,0x0,0xff,0x0,0x0,0x0,
	0x0,0x0,0x0,0xff,0xff,0xff,0xff,0x0,
	0x0,0x0,0xff,0xff,0x0,0x0,0x0,0xff,
	0x0,0xff,0x0};