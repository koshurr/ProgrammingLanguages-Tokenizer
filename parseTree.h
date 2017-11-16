//
// Created by yakov on 11/15/2017.
//

#ifndef TOKENIZER_PARSETREE_H
#define TOKENIZER_PARSETREE_H

#include "Tokenizer.h"

typedef struct{
    int a;
    char op;
    int b;
    struct Expr* exp;
}Expr;

typedef struct{
    char name[20];
    Token t;
    int isTerminal;
    struct Node* parent;
    struct Node* children;
}Node;


#endif //TOKENIZER_PARSETREE_H
