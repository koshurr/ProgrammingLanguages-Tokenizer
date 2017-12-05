//
// Created by yakov on 11/15/2017.
//

#ifndef TOKENIZER_PARSETREE_H
#define TOKENIZER_PARSETREE_H

#include "Tokenizer.h"
typedef struct Statement Statement;
typedef struct Statements Statements;
typedef struct Expr Expr;

typedef struct{

}BoolExp;

typedef struct{
    char **idents;
}Cin;

typedef struct{
    Expr* exprs;
}Cout;

typedef struct{
    BoolExp boolExp;
    Statement* statement;
}While;

typedef struct{//“if” “(" <exp> ")" <statement> ["else" <statement>]
    Expr* exp;
    Statements* stmnts;
    Statements* elseStmnts;
}If;

typedef struct{
    char* ret;
}Return;

struct Expr{
    struct Expr* a;
    char* value;
    struct Expr* b;
};

typedef struct{
    char* identfier;
    char op;
    struct Expr* expression;
}Assign;

typedef struct{
    char* type;
    char** variables;
}Declaration;

typedef struct{
    Declaration* decs;
}Declarations;

struct Statement{
    int is;
    char* type;
    Cin cin;
    Cout cout;
    If inf;
    Assign assign;
    While whyle;
    Return ret;
};

struct Statements{
    Statement* statement;
};

typedef struct{
    Declarations declarations;
    Statements statements;
}Program;



#endif //TOKENIZER_PARSETREE_H
