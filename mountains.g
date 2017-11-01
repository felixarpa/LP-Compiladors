#header
<<
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
>>

<<
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

>>


// * ------------------------------------ LÈXIC ------------------------------------ *

#lexclass START

#token BACKSLASH "\/"
#token DASH "\-"
#token SLASH "\\"

#token ASTERISK "\*"
#token HASH "\#"
#token SEMICOLON "\;"
#token COMMA "\,"
#token ADD "\+"
#token LEFT_P "\("
#token RIGHT_P "\)"

#token EQUAL "\=\="
#token GREATER "\>"
#token LESSER "\<"
#token NOT "NOT"
#token AND "AND"
#token OR "OR"

#token WHILE "while"
#token ENDWHILE "endwhile"
#token IF "if"
#token ENDIF "endif"

#token HEIGHT "Height"
#token WELLFORMED "Wellformed"
#token MATCH "Match"

#token COMPLETE "Complete"
#token DRAW "Draw"

#token VALLEY "Valley"
#token PEAK "Peak"

#token ASSIG "is"

#token MID "M[0-9]*"
#token NUM "[0-9]+"
#token VAR "[a-z]+"

#token SPACE "[\ \n]" << zzskip();>>

input: expr "@"!;
expr: (content)* <<#0=createASTlist(_sibling);>>;
content: (val_assig | assignation | statements | do_draw | do_complete);

statements: (while_statement | if_statement);
if_statement: IF^ block ENDIF!;
while_statement: WHILE^ block ENDWHILE!;
block: LEFT_P! boolean RIGHT_P! expr;

boolean: (NOT | ) or_boolean;
or_boolean: and_boolean (OR^ and_boolean)*;
and_boolean: boolean_op (AND^ boolean_op)*;
boolean_op: (bool_match | bool_wellformed | comparation);
comparation: numeric_value (EQUAL^ | GREATER^ | LESSER^) numeric_value;
numeric_value: (int_heigth | VAR | NUM);

int_heigth: HEIGHT^ LEFT_P! variable RIGHT_P!;
bool_match: MATCH^ LEFT_P! variable COMMA! variable RIGHT_P!;
bool_wellformed: WELLFORMED^ LEFT_P! MID RIGHT_P!;

do_draw: DRAW^ LEFT_P! mountain RIGHT_P!;
do_complete: COMPLETE^ LEFT_P! MID RIGHT_P!;

val_assig: VAR ASSIG^ val;
assignation: MID ASSIG^ mountain;

new_peak: PEAK^ LEFT_P! val COMMA! val COMMA! val RIGHT_P!;
new_valley: VALLEY^ LEFT_P! val COMMA! val COMMA! val RIGHT_P!;

mountain: mountain_elem (SEMICOLON^ mountain_elem)*;
mountain_elem: (elem | variable | new_peak | new_valley);

val: (NUM | VAR (ADD^ (NUM | VAR) | ));
elem: NUM ASTERISK^ (BACKSLASH |DASH | SLASH);
variable: HASH! MID;
