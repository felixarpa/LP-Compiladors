#ifndef tokens_h
#define tokens_h
/* tokens.h -- List of labelled tokens and stuff
 *
 * Generated from: mountains.g
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * ANTLR Version 1.33MR33
 */
#define zzEOF_TOKEN 1
#define BACKSLASH 2
#define DASH 3
#define SLASH 4
#define ASTERISK 5
#define HASH 6
#define SEMICOLON 7
#define COMMA 8
#define ADD 9
#define LEFT_P 10
#define RIGHT_P 11
#define EQUAL 12
#define GREATER 13
#define LESSER 14
#define NOT 15
#define AND 16
#define OR 17
#define WHILE 18
#define ENDWHILE 19
#define IF 20
#define ENDIF 21
#define HEIGHT 22
#define WELLFORMED 23
#define MATCH 24
#define COMPLETE 25
#define DRAW 26
#define VALLEY 27
#define PEAK 28
#define ASSIG 29
#define MID 30
#define NUM 31
#define VAR 32
#define SPACE 33

#ifdef __USE_PROTOS
void input(AST**_root);
#else
extern void input();
#endif

#ifdef __USE_PROTOS
void expr(AST**_root);
#else
extern void expr();
#endif

#ifdef __USE_PROTOS
void content(AST**_root);
#else
extern void content();
#endif

#ifdef __USE_PROTOS
void statements(AST**_root);
#else
extern void statements();
#endif

#ifdef __USE_PROTOS
void if_statement(AST**_root);
#else
extern void if_statement();
#endif

#ifdef __USE_PROTOS
void while_statement(AST**_root);
#else
extern void while_statement();
#endif

#ifdef __USE_PROTOS
void block(AST**_root);
#else
extern void block();
#endif

#ifdef __USE_PROTOS
void boolean(AST**_root);
#else
extern void boolean();
#endif

#ifdef __USE_PROTOS
void or_boolean(AST**_root);
#else
extern void or_boolean();
#endif

#ifdef __USE_PROTOS
void and_boolean(AST**_root);
#else
extern void and_boolean();
#endif

#ifdef __USE_PROTOS
void boolean_op(AST**_root);
#else
extern void boolean_op();
#endif

#ifdef __USE_PROTOS
void comparation(AST**_root);
#else
extern void comparation();
#endif

#ifdef __USE_PROTOS
void numeric_value(AST**_root);
#else
extern void numeric_value();
#endif

#ifdef __USE_PROTOS
void int_heigth(AST**_root);
#else
extern void int_heigth();
#endif

#ifdef __USE_PROTOS
void bool_match(AST**_root);
#else
extern void bool_match();
#endif

#ifdef __USE_PROTOS
void bool_wellformed(AST**_root);
#else
extern void bool_wellformed();
#endif

#ifdef __USE_PROTOS
void do_draw(AST**_root);
#else
extern void do_draw();
#endif

#ifdef __USE_PROTOS
void do_complete(AST**_root);
#else
extern void do_complete();
#endif

#ifdef __USE_PROTOS
void val_assig(AST**_root);
#else
extern void val_assig();
#endif

#ifdef __USE_PROTOS
void assignation(AST**_root);
#else
extern void assignation();
#endif

#ifdef __USE_PROTOS
void new_peak(AST**_root);
#else
extern void new_peak();
#endif

#ifdef __USE_PROTOS
void new_valley(AST**_root);
#else
extern void new_valley();
#endif

#ifdef __USE_PROTOS
void mountain(AST**_root);
#else
extern void mountain();
#endif

#ifdef __USE_PROTOS
void mountain_elem(AST**_root);
#else
extern void mountain_elem();
#endif

#ifdef __USE_PROTOS
void val(AST**_root);
#else
extern void val();
#endif

#ifdef __USE_PROTOS
void elem(AST**_root);
#else
extern void elem();
#endif

#ifdef __USE_PROTOS
void variable(AST**_root);
#else
extern void variable();
#endif

#endif
extern SetWordType zzerr1[];
extern SetWordType zzerr2[];
extern SetWordType setwd1[];
extern SetWordType zzerr3[];
extern SetWordType zzerr4[];
extern SetWordType zzerr5[];
extern SetWordType zzerr6[];
extern SetWordType setwd2[];
extern SetWordType setwd3[];
extern SetWordType zzerr7[];
extern SetWordType zzerr8[];
extern SetWordType zzerr9[];
extern SetWordType zzerr10[];
extern SetWordType zzerr11[];
extern SetWordType setwd4[];
