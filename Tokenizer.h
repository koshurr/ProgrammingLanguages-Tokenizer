//
// Created by yakov on 11/14/2017.
//

#ifndef TOKENIZER_H_INCLUDED
#define TOKENIZER_H_INCLUDED
typedef struct{
    char* tt;
    char* value;
}Token;

typedef struct{
    char *name;
    char *value;
    int address;
    char *type;
//    char *lifetime;
//    char *scope;
}Variable;

typedef struct{
    char *name;
    char *value;
    char *attributes;
}HashTable;

typedef struct{
    int noOfElem;
    HashTable *hashTable;
}SymbolTable;

char getDir();
#endif //TOKENIZER_TOKENIZER_H