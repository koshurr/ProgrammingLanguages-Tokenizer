#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>
#include <io.h>
#include "Tokenizer.h"
#include "parseTree.h"

int isOp(char c);
int isSeparator(char c);
int isKeyword(char* c);
void callSymbolTable(int numOfTokens);
void buildParseTree(int tokenSize);
Token tokenizer();
char *file;
int i = 0;
Token *tokens;
SymbolTable symbolTable;
HashTable* symbolTablesValues;
Program program;


char *readFile(char *fileName) {
    FILE *file = fopen(fileName, "r");
    char *code;
    size_t n = 0;
    int c;

    if (file == NULL) return NULL; //could not open file
    fseek(file, 0, SEEK_END);
    long f_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    code = malloc(f_size);

    while ((c = fgetc(file)) != EOF) {
        code[n++] = (char)c;
    }

    code[n] = '\0';

    return code;
}

int main() {
    int numOfTokens = 0;
    Token T;
    tokens = malloc(10000);
    //long input_file_size;
    file = readFile("C://users//yakov//clionprojects//tokenizer//test.txt");

    symbolTable.hashTable = (HashTable*)malloc(sizeof(HashTable));
    symbolTablesValues = symbolTable.hashTable;
    while(T.tt != "EOFToken"){
        T = tokenizer();
        printf("Token Type: %s\t Token Value: %s\n",T.tt,T.value);
        tokens[numOfTokens++] = (Token) T;
    }
    printf("num of tokens: %d\n", numOfTokens);
    callSymbolTable(numOfTokens);
    buildParseTree(numOfTokens);
    return 0;
}

Token tokenizer(){
    int strCount = 0;
    char c;
    char *str = (char*)malloc(sizeof(char));
    int j = 1;
    int state = 0;
    int substate = 0;
    Token T;
    while((c = file[i]) != '\0'){
        str = (char*)realloc(str, (j++) * sizeof(char));
        i+=1;
        if(state == 1){
            if(substate == 1){
                if(c == '\n'){
                    str[strCount++] = '\0';
                    T.value = str;
                    return T;
                }
                str[strCount++] = c;
                continue;
            }
            else if(substate == 2){
                if(c == '*' && file[i] == '/'){
                    str[strCount++] = c;
                    str[strCount++] = file[i++];
                    str[strCount++] = '\0';
                    T.tt = "OLDCOMMENT";
                    T.value = str;
                    return T;
                }
                str[strCount++] = c;
                continue;
            }

            if(c == '/' && substate == 0){
                substate = 1;
                str[strCount++] = c;
                continue;
            }
            else if(c == '*' && substate == 0){
                substate = 2;
                str[strCount++] = c;
                continue;
            }
        }
        if(state == 2){
            if(!isdigit(c) && !isalpha(c)){
                str[strCount++] = '\0';
                if(isKeyword(str)==1){
                    T.tt = "KEYWORD";
                    T.value = str;
                    i--;
                    return T;
                }
                if(strcmp(str,"TRUE")==0 || strcmp(str,"FALSE")==0){
                    T.tt = "BOOL";
                    T.value = str;
                    i--;
                    return T;
                }
                T.value = str;
                i--;
                return T;
            }
            str[strCount++] = c;
            continue;
        }
        if(state == 3){
            if(c == '.' && substate == 0){
                T.tt = "FLOAT";
                substate = 1;
                str[strCount++] = c;
                continue;
            }
            if(c == '.' && substate == 1){
                T.tt = "BADTOKEN";
                str[strCount++] = c;
                continue;
            }
            if(isdigit(c)){
                str[strCount++] = c;
                continue;
            }
            else{
                i--;
                str[strCount++] = '\0';
                T.value = str;
                return T;
            }
        }
        if(state == 4){
            if(c == 39){
                str[strCount++] = c;
                str[strCount++] = '\0';
                T.value = str;
                return T;
            }
            str[strCount++] = c;
            continue;
        }
        if(state == 5){
            if(c == 34){
                str[strCount++] = c;
                str[strCount++] = '\0';
                T.value = str;
                return T;
            }
            str[strCount++] = c;
            continue;
        }
        if(c == '#' || state == 99){
            if(state == 0){
                T.tt = "HEADER";
                state = 99;
            }
            if(c == '\n'){
                T.value = str;
                return T;
            }
            str[strCount++] = c;
            continue;
        }
        if(isalpha(c)){
            state = 2;
            str[strCount++] = c;
            T.tt = "IDENT";
            continue;
        }
        if(isdigit(c)){
            state = 3;
            str[strCount++] = c;
            T.tt = "INTEGER";
            continue;
        }
        if(c == '/'){
            T.tt = "COMMENT";
            state = 1;
            str[strCount++] = c;
            continue;
        }
        if(c == ' ' || c == '\n' || c == '\t'){
            continue;
        }
        if(c == 39){
            state = 4;
            T.tt = "CHAR";
            str[strCount++] = c;
            continue;
        }
        if(c == 34){
            state = 5;
            T.tt = "STRING";
            str[strCount++] = c;
            continue;
        }
        if(isOp(c) == 1){
            str[strCount++] = c;
            T.tt = "OPERATOR";
            str[strCount++] = '\0';
            T.value = str;
            return T;
        }
        if(isSeparator(c) == 1){
            str[strCount++] = c;
            T.tt = "SEPARATOR";
            str[strCount++] = '\0';
            T.value = str;
            return T;
        }

    }
    T.tt = "EOFToken";
    str[strCount++] = '\0';
    T.value = str;
    return T;
}

int isOp(char c){
    int a = (int) c;
    if( a == 124 || a == 126){
        return 1;
    }
    if( a == 92 || a == 94){
        return 1;
    }
    if( a >= 33 && a <= 47 && a != 40 && a != 41 && a != 44){
        return 1;
    }
    if( a >= 58 && a <= 64 && a != 59){
        return 1;
    }
    return 0;
}

int isSeparator(char c){
    int a = (int) c;
    if( a == 123 || a == 125 || a == 91 || a == 93 || a == 59 || a == 44 || a == 40 || a == 41){
        return 1;
    }
    return 0;
}

int isKeyword(char* c){
    const char* keywords[35]={"auto", "double", "int", "struct", "const", "float",
                        "short", "unsigned", "break", "else", "long", "switch",
                        "continue", "for", "signed", "void", "case", "enum",
                        "register", "typedef", "default", "goto", "sizeof", "volatile",
                        "char", "extern", "return", "union", "do", "if", "static",
                        "while", "cin", "cout", "bool"};

    for(int i = 0; i < 35;i++){
        if(strcmp(c,keywords[i]) == 0){
            return 1;
        }
    }
    return 0;
}

void callSymbolTable(int numOfTokens){
    int size = 0;
    int j = 0;
    int k = 0;
    for( j ; j < numOfTokens ; j++ ){
        size = symbolTable.noOfElem;
        if(strcmp(tokens[j].tt,"CONST") == 0 || strcmp(tokens[j].tt,"IDENT") == 0){
            if(strcmp(tokens[j].tt,"OPERATOR") == 0){
                if(strcmp(tokens[j].tt,"CONST") == 0 || strcmp(tokens[j].tt,"IDENT") == 0){
                    //expression;
                }
            }
        }
        if(strcmp(tokens[j].value,"int") == 0 || strcmp(tokens[j].value,"float") == 0){
            if(strcmp(tokens[j+1].tt,"IDENT") == 0){
                if(strcmp(tokens[j+2].value,"=")==0){
                    if(strcmp(tokens[j+3].tt,"BADTOKEN") != 0){
                        symbolTablesValues =
                                (HashTable*)realloc(symbolTablesValues, (symbolTable.noOfElem+1) * sizeof(HashTable));
                        symbolTablesValues[size].attributes = tokens[j].value;
                        symbolTablesValues[size].name = tokens[j+1].value;
                        symbolTablesValues[size].value = tokens[j+3].value;
                        symbolTable.noOfElem++;
                        j += 3;
                    }
                }
            }
        }
    }
    printf("\nSymbol Table Print Out\n");
    for(int h = 0; h < size; h++){
        printf("\nElement %d{\n",h+1);
        printf("Attribute: %s\n", symbolTablesValues[h].attributes);
        printf("Name: %s\n", symbolTablesValues[h].name);
        printf("Value: %s\n}", symbolTablesValues[h].value);
    }
}
void buildParseTree(int tokenSize){
    for(int y = 0; y < tokenSize; y++){

    }
}



