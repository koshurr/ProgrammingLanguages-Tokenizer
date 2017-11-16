//
// Created by yakov on 11/15/2017.
//

#ifndef TOKENIZER_PARSETREE_H
#define TOKENIZER_PARSETREE_H

#include "Tokenizer.h"

typedef struct{

}IDENT;

typedef struct{
    int a;
    char op;
    int b;
    struct Expr* exp;
}Expr;

typedef struct{
    IDENT identfier;
    Expr expression;
}Assign;

typedef struct{
    char* type;
    Assign assign;
}Declaration;

typedef struct{
    Declaration* declaration;
}ListOfDeclarations;

typedef struct{
    ListOfDeclarations declarations;
    //listof statements
}Program;



#endif //TOKENIZER_PARSETREE_H
