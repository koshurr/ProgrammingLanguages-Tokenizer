#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>
#include <io.h>
#include "Tokenizer.h"
#include "parseTree.h"

int isUop(char c);
int isRelop(char c);
int isAriop(char c);
int isSeparator(char c);
int isKeyword(char* c);
void callSymbolTable();
char* toUpper(char* str);
char* furst();
int isDataType(char* c);
Program parseProgram();
void printProgram(Program program);
Declarations parseDeclarations();
void printDeclarations(Declarations decs);
Statements parseStatements();
void printStatements(Statements stmnts);
Assign parseAssign();
void printAssign(Assign as);
//Expr parseExp();
//Expr parseExp1();
//Expr parseExp2();
//Expr parseExp3();
//Expr parseExp4();
//Expr parseExp5();
//Expr parseExp6();
Expr* parseExp7();
Expr* parseExp8();
char* parseLit();
Expr* parseLit2();
char* parseID();
Cout parseCout();
void printCout(Cout c);
Cin parseCin();
void printCin(Cin c);
While parseWhile();
void printWhile(While whyle);
If parseIf();
void printIf(If inf);
Return parseReturn();
void printReturn(Return ret);



Token tokenizer();
char *file;
int i = 0;
Token *tokens;
int numOfTokens;
SymbolTable symbolTable;
HashTable* symbolTablesValues;


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

int main(){
    numOfTokens = 0;
    Token T;
    Program program;
    tokens = malloc(10000);
    //long input_file_size;
    //"C://users//yakov//clionprojects//tokenizer//test.txt"
    file = readFile("C:/home/test.txt");

    symbolTable.hashTable = (HashTable*)malloc(sizeof(HashTable));
    symbolTablesValues = symbolTable.hashTable;

    printf("Tokens:\n[\n");
    while(T.tt != "EOFToken"){
        T = tokenizer();
        printf("\t%d{type: '%10s'\t value: '%s'}\n",numOfTokens,T.tt,T.value);
        tokens[numOfTokens++] = (Token) T;
    }
    printf("]");
    printf("\nnum of tokens: %d\n", numOfTokens);


    program = parseProgram();
    printProgram(program);

    return 0;
}

Program parseProgram() {
    Program ret;
    i = 0;
    while (strcmp(tokens[i].value, "main") != 0) {
        i++;
    }
    i+=4;//to move past main , ( , ) , {

    ret.declarations = parseDeclarations();
    ret.statements = parseStatements();

    return ret;
}

void printProgram(Program program){
    printDeclarations(program.declarations);
    printStatements(program.statements);
}

Declarations parseDeclarations(){
    Declarations decs = {.decs = (Declaration*)malloc(5 * sizeof(Declaration))};
    int numOfDecs,numOfVars;
    numOfDecs = 0;
    numOfVars = 0;
    while(strcmp(tokens[i].value,"int") == 0 || strcmp(tokens[i].tt,"char") == 0){
        decs.decs[numOfDecs].type = tokens[i++].value;
        decs.decs[numOfDecs].variables = (char **)malloc(5 * sizeof(char*));
        while(strcmp(tokens[i].value,";") != 0){
            if(strcmp(tokens[i].tt,"IDENT") == 0){
                decs.decs[numOfDecs].variables[numOfVars] = (char*)malloc(5 * sizeof(char));
                decs.decs[numOfDecs].variables[numOfVars++] = tokens[i].value;
            }
            i++;
        }
        i++;
        decs.decs[numOfDecs].variables[numOfVars] = "\0";
        numOfDecs++;
        numOfVars = 0;
    }
    decs.decs[numOfDecs].type = 0;
    return decs;
}

void printDeclarations(Declarations decs){
    int num = 0;
    int num2 = 0;
    while(decs.decs[num].type){
        printf("\n%s ",decs.decs[num].type);
        while(strcmp(decs.decs[num].variables[num2],"\0") !=0 ){
            printf("%s ",decs.decs[num].variables[num2]);
            num2++;
        }
        num2 = 0;
        num++;
    }
}

Statements parseStatements(){
    Statements stmnts;
    stmnts.statement = (Statement *)malloc(50 * sizeof(Statement));
    int numOfStmnts = 0;
    while(strcmp(tokens[i].tt,"IDENT")==0 ||strcmp(tokens[i].value,"cout")==0 ||strcmp(tokens[i].value,"return")==0 ||
            strcmp(tokens[i].value,"cin")==0 ||strcmp(tokens[i].value,"if")==0 ||strcmp(tokens[i].value,"while")==0){
        stmnts.statement[numOfStmnts].is = 1;
        if(strcmp(tokens[i].value,"cout")==0){
            stmnts.statement[numOfStmnts].type = "cout";
            stmnts.statement[numOfStmnts++].cout = parseCout();
        }
        else if(strcmp(tokens[i].value,"cin")==0){
            stmnts.statement[numOfStmnts].type = "cin";
            stmnts.statement[numOfStmnts++].cin = parseCin();
        }
        else if(strcmp(tokens[i].value,"while")==0){
            while(strcmp(tokens[i].value,"}") != 0){//move over
                i++;
            }
            i++;
//            stmnts.statement[numOfStmnts].type = "while";
//            stmnts.statement[numOfStmnts++].whyle = parseWhile();
        }
        else if(strcmp(tokens[i].value,"if")==0){
            stmnts.statement[numOfStmnts].type = "if";
            stmnts.statement[numOfStmnts++].inf = parseIf();
        }
        else if(strcmp(tokens[i].value,"return")==0){
            stmnts.statement[numOfStmnts].type = "return";
            stmnts.statement[numOfStmnts++].ret = parseReturn();
        }
        else if(strcmp(tokens[i].tt,"IDENT")==0){
            stmnts.statement[numOfStmnts].type = "assign";
            stmnts.statement[numOfStmnts++].assign = parseAssign();
        }
        //numOfStmnts++;
    }
    printf("yoyoyyoyoyoy%d",numOfStmnts);
    return stmnts;
}

void printStatements(Statements stmnts){
    int num = 0;
    while(stmnts.statement[num].is == 1){
        Statement s = stmnts.statement[num];
        if(strcmp(s.type,"cout")==0){
            printCout(s.cout);
        }
        else if(strcmp(s.type,"cin")==0){
            printCin(s.cin);
        }
        else if(strcmp(s.type,"while")==0){
        }
        else if(strcmp(s.type,"if")==0){
        }
        else if(strcmp(s.type,"return")==0){
        }
        else if(strcmp(s.type,"assign")==0){
            printAssign(s.assign);
        }
        num++;
    }
}

Assign parseAssign(){
    Assign res = {.op=0,.expression=NULL,.identfier=NULL};
    char* id = parseID();
    char op;
    Expr* exp;
    if (strcmp(tokens[i].value,"=") == 0){
        op = '=';
        i++;
    }

    exp = parseExp7();

    res.identfier = id;
    res.op = op;
    res.expression = exp;

    return res;
}

void printAssign(Assign as){
    printf("\nAssignment");
    printf("\n%s",as.identfier);
    printf("\n%c",as.op);
    printf("\nExpression");
    printExpr(as.expression);
}

Expr* buildExp(Expr* a, char* c, Expr* b){
    //Expr ex = {.a = a, .value = c,.b = b};
    Expr* ex1 = (Expr *)malloc(sizeof(Expr));
//    Expr* ex2 = (Expr *)malloc(sizeof(Expr));
    ex1->value = c;
    ex1->a = a;
    ex1->b = b;
//
//    exp.a = ex1->a;
//    exp.b = ex1->b;
//    exp.value = ex1->value;
    return ex1;
}

//void printExpr(Expr* ex){//prints in preorder
//    if(!ex){
//        return;
//    }
//    printf("\n%s",ex->value);
//    printExpr(ex->a);
//    printExpr(ex->b);
//}

void printExpr(Expr* ex) {//prints in preorder
    printf("\n%s",ex->value);
    if(ex->a->value){
        printExpr(ex->a);
    }
    if(ex->b->value){
        printExpr(ex->b);
    }
}

void prepExpr(Expr* ex){
    ex->a = (Expr*)malloc(sizeof(Expr));
    ex->b = (Expr*)malloc(sizeof(Expr));
    ex->a->a = (Expr*)malloc(sizeof(Expr));
    ex->b->a = (Expr*)malloc(sizeof(Expr));
    ex->a->b = (Expr*)malloc(sizeof(Expr));
    ex->b->b = (Expr*)malloc(sizeof(Expr));

//    ex->a = 0;
//    ex->b = 0;
    ex->a->value = 0;
    ex->b->value = 0;
    ex->a->a->value = 0;
    ex->b->a->value = 0;
    ex->a->b->value = 0;
    ex->b->b->value = 0;
}

//Expr parseExp3();
//Expr parseExp4();
//Expr parseExp5();
//Expr parseExp6();
//
//
//void copy_string(char *target, char *source) {
//    int k = 0;
//    target = (char*)malloc(sizeof(char));
//    while(source[k])
//    {
//        target[k] = source[k];
//        k++;
//    }
//    target[k] = '\0';
//}
//
//Expr* parseExp7(){
//    Expr* ret = initExp();
//    Expr* temp = initExp();
//    ret->a = parseExp8();
//    *temp->a = *ret->a;
//    while(strcmp(tokens[i].value,"+") == 0 || strcmp(tokens[i].value,"-") == 0){
//        ret->value = tokens[i++].value;
//        copy_string(temp->value,ret->value);
//        *temp->b = *ret->b;
//        if(!strcmp(tokens[i].value,"+") != 0 && !strcmp(tokens[i].value,"-") != 0){
//            *temp->a = *temp;
//            temp->value = (char*)malloc(sizeof(char));
//            temp->b = (Expr*)malloc(sizeof(Expr));
//            ret = initExp();
//        }else{
//            printf("hi");
//            printExpr(temp);
//            return temp;
//        }
//    }
//
//    if(strcmp(ret->value,"18killstreak") == 0){
//        ret = ret->a;
//        return ret;
//    }else{
//        ret = temp;
//    }
//    //printExpr(ret);
//    return ret;
//}
//
//Expr* parseExp8(){
//    Expr* ret = initExp();
//    Expr* temp = initExp();
//
//    ret->a->value = parseLit();
//
//    while(strcmp(tokens[i].value,"*") == 0 || strcmp(tokens[i].value,"/") == 0) {
//        ret->value = tokens[i++].value;
//        ret->b->value = parseLit();
//        if (!strcmp(tokens[i].value,"*") != 0 && !strcmp(tokens[i].value,"/") != 0) {
//            if (strcmp(temp->value, "18killstreak") == 0) {
//                temp = ret;
//            } else {
//                ret->a = temp;
//                temp = ret;
//            }
//            ret = initExp();
//        }else{
//            return ret;
//        }
//    }
//    if(strcmp(ret->value,"18killstreak") == 0){
//        ret = ret->a;
//        return ret;
//    }else{
//        ret = temp;
//    }
//    return ret;
//}
//
//
//
//

Expr* parseExp7(){
    Expr *res = (Expr *)malloc(sizeof(Expr));
    prepExpr(res);
    res->a = parseExp8();
    res->value = "18killstreak";
    Expr* ex1 = NULL;
    while(strcmp(tokens[i].value,"+") == 0 || strcmp(tokens[i].value,"-") == 0){
        res->value = tokens[i++].value;
        res->b = parseExp8();
        if(ex1 == NULL){
            ex1 = res;
        }else{
            ex1 = buildExp(ex1,res->value,res->b);
        }
        res = (Expr*)malloc(sizeof(Expr));
        prepExpr(res);
    }

    if(strcmp(tokens[i].value,";")==0){
        i++;
    }

    if(ex1 == NULL){
        Expr* ex = res->a;
        return ex;
    }

    return ex1;
}

Expr* parseExp8(){
    Expr* res = (Expr *)malloc(sizeof(Expr));
    prepExpr(res);
    Expr* ex1 = NULL;

    if(strcmp(tokens[i].tt,"SEPARATOR")==0){
        res->a = parseLit2()->a;
    }else{
        res->a->value = parseLit();
    }

    while(strcmp(tokens[i].value,"*") == 0 || strcmp(tokens[i].value,"/") == 0){
        res->value = tokens[i++].value;

        if(strcmp(tokens[i].tt,"SEPARATOR")==0){
            res->b = parseLit2()->a;
        }else{
            res->b->value = parseLit();
        }
        if(ex1 == NULL){
            ex1 = res;
        }else{
            ex1 = buildExp(ex1,res->value,res->b);
        }
        res = (Expr*)malloc(sizeof(Expr));
        prepExpr(res);
    }
    if(ex1 == NULL){
        res->value = res->a->value;
        res->a->value = 0;
        return res;
    }
    return ex1;
}

//Expr parseExp9(){
//
//}

char* parseLit(){
    char* ret = (char *)malloc(sizeof(char));
    if(strcmp(tokens[i].tt,"INTEGER")==0){
        ret = tokens[i++].value;
    }
    else if(strcmp(tokens[i].tt,"CHAR")==0){
        ret = tokens[i++].value;
    }
    else if(strcmp(tokens[i].tt,"IDENT")==0){
        ret = parseID();
    }
    return ret;
}

Expr* parseLit2(){
    Expr* ret = (Expr *)malloc(sizeof(Expr));
    //prepExpr(ret);
    if(strcmp(tokens[i].value,"(")==0 || strcmp(tokens[i].value,")")==0){
        i++;
        Expr* ex = parseExp7();
        ret->a = ex;
        i++;
    }
    return ret;
}

char* parseID(){
    if(strcmp(tokens[i].tt,"IDENT") == 0){
        return tokens[i++].value;
    }
    printf("Error! Expected ID from token %d",i+1);
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
            str[strCount++] = '\0';
            T.tt = "RELOP";
            T.value = str;
            i--;
            return T;
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
            T.tt = "LINECOMMENT";
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
        if(isUop(c) == 1){
            str[strCount++] = c;
            T.tt = "UOP";
            str[strCount++] = '\0';
            T.value = str;
            return T;
        }
        if(isRelop(c) == 1){
            str[strCount++] = c;
            T.tt = "RELOP";
            str[strCount++] = '\0';
            T.value = str;
            return T;
        }
        if(isAriop(c) == 1){
            str[strCount++] = c;
            T.tt = "ARIOP";
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

int isUop(char c){
    int a = (int) c;
    if( a == 33 || a == 45 || a == 126){
        return 1;
    }
    return 0;
}

int isAriop(char c){
    int a = (int) c;
    if(a == 33 || a == 37 || a == 42 || a == 43 || a == 45 || a == 47 || a == 126){
        return 1;
    }
    return 0;
}

int isRelop(char c){
    int a = (int) c;
    if( a == 124 ){
        return 1;
    }
    if( a == 92 || a == 94){
        return 1;
    }
    if( a >= 34 && a <= 47 && a != 40 && a != 41 && a != 44){
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

void callSymbolTable(){
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
        if(strcmp(tokens[j].value,"int") == 0 || strcmp(tokens[j].value,"float") == 0
           || strcmp(tokens[j].value,"char") == 0){
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
        if(strcmp(tokens[j].tt,"KEYWORD") == 0 ){
            if(strcmp(tokens[j+1].tt,"IDENT") == 0){
                if(strcmp(tokens[j+2].value,"[") == 0){
                    if(strcmp(tokens[j+3].tt,"INTEGER") == 0 && strcmp(tokens[j+4].value,"]") == 0){
                        if(strcmp(tokens[j+5].value,"=")==0){
                            symbolTablesValues =
                                    (HashTable*)realloc(symbolTablesValues, (symbolTable.noOfElem+1) * sizeof(HashTable));
                            symbolTablesValues[size].attributes = tokens[j].value;
                            symbolTablesValues[size].name = tokens[j+1].value;
                            j += 5;
                            if(strcmp(tokens[j+1].value,"{") == 0){
                                j += 1;
                                if(strcmp(tokens[j-6].value,"char") == 0 && strcmp(tokens[j+1].tt,"STRING") == 0){
                                    j += 1;
                                    symbolTablesValues[size].value = tokens[j].value;
                                    printf("jello%s",tokens[j].value);
                                }
                                else{
                                    while(strcmp(tokens[j+1].tt,toUpper(tokens[j-5].value)) == 0){
                                        break;
                                    }
                                }
                                symbolTable.noOfElem++;
                            }
//                            if(strcmp(tokens[j+7].tt,"BADTOKEN") != 0){
//
//                                symbolTablesValues[size].value = tokens[j+7].value;
//                                symbolTable.noOfElem++;
//                                j += 7;
//                            }
                        }
                    }
                }
            }
        }
    }
    printf("\nSymbol Table:\n[\n");
    for(int h = 0; h < size; h++){
        printf("\t{ Attribute: %5s\t", symbolTablesValues[h].attributes);
        printf("Name: %7s\t", symbolTablesValues[h].name);
        printf("Value: %10s }\n", symbolTablesValues[h].value);
    }
    printf("]\n");
    printf("Number of Elements %d\n\n",size);
}

char* furst(){//return name of nonterminal which corresponds to the token on deck
    char* returnString = malloc(64);
    char* tokT = tokens[i].tt;
//    switch(tokT){
//        case "KEYWORD":
//        //    returnString = furstKeyword();
//        case "IDENT":
//        case "INTEGER":
//        case "FLOAT":
//        case "STRING":
//        case "CHAR":
//        case "RELOP":
//        case "UOP":
//        default:
//    }
    return returnString;
}

char* toUpper(char* str){
    int k = 0;
    while(str[k] != '\0'){
        str[k] = (char) toupper((unsigned char) str[k]);
        k++;
    }
    return str;
}

Cout parseCout(){
    Cout c;
    int noOfExprs = 0;
    c.exprs = (Expr*)malloc(5 * sizeof(Expr));
    while(strcmp(tokens[i].value,";") != 0){
        if(strcmp(tokens[i].value,"cout") == 0 || strcmp(tokens[i].value,"<") == 0){
            i++;
            continue;
        }
        if(strcmp(tokens[i].tt,"IDENT") == 0 || strcmp(tokens[i].tt,"INTEGER") == 0){
            c.exprs[noOfExprs++] = *parseExp7();
//            printf("yo look hr\n");
//            printExpr(&c.exprs[noOfExprs-1]);
            if(strcmp(tokens[--i].value,";")==0){
                //i++;
                break;
            }
        }
        else{
            printf("\nerror in syntax with token %d\n",i);
        }
        i++;
    }
    //printf("%d,%s",i,tokens[i].value);
    i++;//move past semicolon
    //c.exprs[noOfExprs++] =;
    return c;
}

void printCout(Cout c){
    printf("\n\ncout ");
    int count = 0;
    while(c.exprs[count].value){
        printf(">> expression");
        printExpr(&c.exprs[count++]);
        printf("\n\n");
    }
}

Cin parseCin(){
    Cin c;
    int noOfIds = 0;
    c.idents = (char **)malloc(sizeof(char));
    while(strcmp(tokens[i].value,";") != 0){
        if(strcmp(tokens[i].tt,"IDENT") == 0){
            c.idents[noOfIds++] = tokens[i].value;
        }
        i++;
    }
    i++;//move past semicolon
    c.idents[noOfIds] = 0;
    return c;
}

void printCin(Cin c){
    printf("\ncin ");
    int count = 0;
    while(c.idents[count]){
        printf("<< %s ",c.idents[count++]);
    }
}

While parseWhile(){
    While w;
    w.statement = (Statement*)malloc(sizeof(Statement));
    return w;
}

void printWhile(While whyle){

}

If parseIf(){

}

void printIf(If inf){

}

Return parseReturn(){
    Return ret;
    ret.ret = (char*)malloc(sizeof(char));
    while(strcmp(tokens[i].value,";") != 0){
        if(strcmp(tokens[i].tt,"KEYWORD") == 0){
            i++;
        }
        ret.ret = tokens[i++].value;
        if(strcmp(tokens[i].value,";") != 0){
            printf("error in syntax with token %d\n",i);
            while(strcmp(tokens[i].value,";") != 0){
                i++;
            }
        }
    }
    return ret;
}

void printReturn(Return ret){
    printf("\nreturn %s",ret.ret);
}