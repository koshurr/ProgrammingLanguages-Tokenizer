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
Expr* parseExp1();
Expr* parseExp2();
//Expr parseExp3();
//Expr parseExp4();
//Expr parseExp5();
Expr* parseExp6();
Expr* parseExp7();
Expr* parseExp8();
void printExpr(Expr* ex);
Expr* parseLit();
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
Expr* parseBoolExp();
char *strdup(const char *str);
Token tokenizer();
void runInterpreter(Program program);
void printSymbolTable();
HashTable* getIdent(char *string);
void evaluateExpr(Expr* ex);
int isSymbol(char* str);
char* getSymbolVal(char *str);
void copyExpr(Expr* ex1,Expr* ex2);


int evaluateBoolExp(Expr* exp);

void copyAssign(Assign* a1,Assign* a2);

char *file;
int i = 0;
Token *tokens;
int numOfTokens;
SymbolTable symbolTable;
HashTable* symbolTablesValues;

char *strdup(const char *str) {
    char *rv = malloc(strlen(str) + 1);
    if (rv) strcpy(rv, str);
    return rv;
}

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

int main(int argc, char *argv[]){
    numOfTokens = 0;
    Token T;
    Program program;
    tokens = malloc(10000);
    //long input_file_size;
    file = readFile(argv[1]);

    symbolTable.hashTable = (HashTable*)malloc(10 * sizeof(HashTable));
    symbolTable.noOfElem = 0;
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
    printf("\nprint empty symbol table\n");
    printSymbolTable();
    printf("\nrun interpreter\n");
    runInterpreter(program);
    printf("\nprint final symbol table\n");
    printSymbolTable();

    return 0;
}

void printSymbolTable() {
    int count = symbolTable.noOfElem - 1;
    printf("\n");
    while(count >= 0){
        printf("name: %s, value: %s\n",symbolTable.hashTable[count].name,symbolTable.hashTable[count].value);
        count--;
    }
}

void runInterpreter(Program program) {
//symbol table has already been built so I skip program declarations
    int num = 0;
    while(program.statements.statement[num].is == 1){
        Statement s = program.statements.statement[num];
        if(strcmp(s.type,"cout")==0) {
            int count = 0;
            while(!strcmp(s.cout.exprs[count].value,"18killstreak") == 0){
                evaluateExpr(&s.cout.exprs[count]);
                if(isdigit(s.cout.exprs[count].value[0]) == 0 && s.cout.exprs[count].value[0] != '-'){
                    int count2 = 0;
                    while(count2 <= symbolTable.noOfElem - 1){
                        if(strcmp(symbolTable.hashTable[count2].name, s.cout.exprs[count].value)==0){
                            printf("cout >> %s\n",symbolTable.hashTable[count2].value);
                            count++;
                            break;
                        }
                        count++;
                        count2++;
                    }
                }
                else{
                    printf("cout >> %s\n",s.cout.exprs[count].value);
                    count++;
                }
            }
        }
        else if(strcmp(s.type,"cin")==0){
            int count = 0;
            while(s.cin.idents[count]){
                //HashTable location = getIdent(s.cin.idents[count]);
                char* temp = (char*)malloc(sizeof(char));
                int j = symbolTable.noOfElem-1;
                while(j >= 0){
                    if(strcmp(symbolTable.hashTable[j].name,s.cin.idents[count]) == 0){
                        if(strcmp(symbolTable.hashTable[j].attributes,"int")==0){
                            printf("give an integer: ");
                            int temp2;
                            int result = scanf("%d",&temp2);
                            symbolTable.hashTable[j].value = (char*)malloc(sizeof(char));
                            itoa(temp2,symbolTable.hashTable[j].value,10);
                            printf("your input was %s\n", symbolTable.hashTable[j].value);
                        }
                        else if(strcmp(symbolTable.hashTable[j].attributes,"char")==0){
                            printf("give a character: ");
                            scanf("%c",temp);
                            symbolTable.hashTable[j].value = (char*)malloc(sizeof(char));
                            symbolTable.hashTable[j].value = temp;
                            printf("your input was %s\n", symbolTable.hashTable[j].value);
                        }
                    }
                    j--;
                }
                count++;
            }
        }
        else if(strcmp(s.type,"while")==0){
            while(evaluateBoolExp(s.whyle.boolExp)){
                Program newProgram;
                newProgram.statements = *s.whyle.statements;
                runInterpreter(newProgram);
            }
        }
        else if(strcmp(s.type,"if")==0){
            //not in the test file
        }
        else if(strcmp(s.type,"return")==0){
            //since theres only one function (main) this doesnt do anything
        }
        else if(strcmp(s.type,"assign")==0){
            int count = 0;
            Assign* temp = (Assign*)malloc(sizeof(Assign));
            copyAssign(&s.assign,temp);
            evaluateExpr(temp->expression);
            if(isdigit(temp->expression->value[0]) == 0 && temp->expression->value[0] != '-'){
                int count2 = 0;
                while(count2 <= symbolTable.noOfElem - 1){
                    if(strcmp(symbolTable.hashTable[count2].name, temp->expression->value)==0){
                        temp->expression->value = strdup(symbolTable.hashTable[count2].value);
                        break;
                    }
                    count2++;
                }
            }
            while(count <= symbolTable.noOfElem - 1){
                if(strcmp(symbolTable.hashTable[count].name, temp->identfier)==0){
                    symbolTable.hashTable[count].value = strdup(temp->expression->value);
                }
                count++;
            }
        }
        num++;
    }
}

HashTable* getIdent(char *string) {
    int count = symbolTable.noOfElem - 1;
    printf("\n");
    while(count >= 0){
        symbolTable.hashTable[count].value = "";
        if(strcmp(symbolTable.hashTable[count].name, string)==0){
            return &symbolTable.hashTable[count];
        }
        count--;
    }
    printf("No variable name %s, exists\n",string);
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
    char* decType;
    numOfDecs = 0;
    numOfVars = 0;
    while(strcmp(tokens[i].value,"int") == 0 || strcmp(tokens[i].tt,"char") == 0){
        decType = tokens[i].value;
        symbolTable.hashTable[symbolTable.noOfElem].attributes = decType;
        decs.decs[numOfDecs].type = tokens[i++].value;
        decs.decs[numOfDecs].variables = (char **)malloc(5 * sizeof(char*));
        while(strcmp(tokens[i].value,";") != 0){
            if(strcmp(tokens[i].tt,"IDENT") == 0){
                symbolTable.hashTable[symbolTable.noOfElem].attributes = decType;
                symbolTable.hashTable[symbolTable.noOfElem].value = "";
                symbolTable.hashTable[symbolTable.noOfElem++].name = tokens[i].value;
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
            stmnts.statement[numOfStmnts].type = "while";
            i++;//pass while
            stmnts.statement[numOfStmnts++].whyle = parseWhile();
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
            i++;//pass ;
        }
        //numOfStmnts++;

    }
    printf("no of statements%d",numOfStmnts);
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
            printWhile(s.whyle);
        }
        else if(strcmp(s.type,"if")==0){
        }
        else if(strcmp(s.type,"return")==0){
            printReturn(s.ret);
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

void printExpr(Expr* ex){//prints in preorder
    //!*ex->a->value ||
    if(strcmp(ex->value,"18killstreak") == 0){
        return;
    }
    printf("\n%s",ex->value);

    printExpr(ex->a);
    printExpr(ex->b);
}

Expr* initExp(){
    Expr* ret;
    ret = (Expr*)malloc(sizeof(Expr));
    ret->a = (Expr*)malloc(sizeof(Expr));
    ret->b = (Expr*)malloc(sizeof(Expr));
    ret->value = (char*)malloc(sizeof(char));
    ret->value = strdup("18killstreak");
    ret->a->value = strdup("18killstreak");
    ret->b->value = strdup("18killstreak");
    return ret;
}

//Expr parseExp3();
//Expr parseExp4();
//Expr parseExp5();

Expr* parseExp1(){
    char* tempVal;
    Expr* tempB;
    Expr* temp2;
    Expr* ret = initExp();
    ret->a = parseExp2();
    while(strcmp(tokens[i].value,"||") == 0){
        tempVal = tokens[i++].value;
        tempB = parseExp2();
        if(strcmp(ret->value,"18killstreak") == 0){
            ret->value = tempVal;
            ret->b = tempB;
        }else{
            temp2 = initExp();
            temp2->a = ret;
            temp2->value = tempVal;
            temp2->b = tempB;
            ret = temp2;
        }
    }

    if(strcmp(ret->value,"18killstreak") == 0){
        ret = ret->a;
        return ret;
    }
//    printf("%d\n",i);
//    printExpr(ret);
    return ret;
}

Expr* parseExp2(){
    char* tempVal;
    Expr* tempB;
    Expr* temp2;
    Expr* ret = initExp();
    ret->a = parseExp6();
    while(strcmp(tokens[i].value,"&&") == 0){
        tempVal = tokens[i++].value;
        tempB = parseExp6();
        if(strcmp(ret->value,"18killstreak") == 0){
            ret->value = tempVal;
            ret->b = tempB;
        }else{
            temp2 = initExp();
            temp2->a = ret;
            temp2->value = tempVal;
            temp2->b = tempB;
            ret = temp2;
        }
    }

    if(strcmp(ret->value,"18killstreak") == 0){
        ret = ret->a;
        return ret;
    }
//    printf("%d\n",i);
//    printExpr(ret);
    return ret;
}

Expr* parseExp6(){
    char* tempVal;
    Expr* tempB;
    Expr* temp2;
    Expr* ret = initExp();
    ret->a = parseExp7();
    while(strcmp(tokens[i].value,">") == 0 || strcmp(tokens[i].value,"<") == 0 || strcmp(tokens[i].value,">=") == 0 ||
            strcmp(tokens[i].value,"<=") == 0){
        tempVal = tokens[i++].value;
        tempB = parseExp7();
        if(strcmp(ret->value,"18killstreak") == 0){
            ret->value = tempVal;
            ret->b = tempB;
        }else{
            temp2 = initExp();
            temp2->a = ret;
            temp2->value = tempVal;
            temp2->b = tempB;
            ret = temp2;
        }
    }

    if(strcmp(ret->value,"18killstreak") == 0){
        ret = ret->a;
        return ret;
    }
//    printf("%d\n",i);
//    printExpr(ret);
    return ret;
}

Expr* parseExp7(){
    char* tempVal;
    Expr* tempB;
    Expr* temp2;
    Expr* ret = initExp();
    ret->a = parseExp8();
    while(strcmp(tokens[i].value,"+") == 0 || strcmp(tokens[i].value,"-") == 0){
        tempVal = tokens[i++].value;
        tempB = parseExp8();
        if(strcmp(ret->value,"18killstreak") == 0){
            ret->value = tempVal;
            ret->b = tempB;
        }else{
            temp2 = initExp();
            temp2->a = ret;
            temp2->value = tempVal;
            temp2->b = tempB;
            ret = temp2;
        }
    }

    if(strcmp(ret->value,"18killstreak") == 0){
        ret = ret->a;
        return ret;
    }

    return ret;
}

Expr* parseExp8(){
    char* tempVal;
    Expr* tempB;
    Expr* temp2;
    Expr* ret = initExp();

    if(strcmp(tokens[i].value,"(")==0){
        ret->a = parseLit2();
    }else{
        ret->a = parseLit();
    }
    while(strcmp(tokens[i].value,"*") == 0 || strcmp(tokens[i].value,"/") == 0 || strcmp(tokens[i].value,"%") == 0) {
        tempVal = tokens[i++].value;
        if(strcmp(tokens[i].value,"(")==0){
            tempB = parseLit2();
        }else{
            tempB = parseLit();
        }
        if(strcmp(ret->value,"18killstreak") == 0){
            ret->value = tempVal;
            ret->b = tempB;
        }else{
            temp2 = initExp();
            temp2->a = ret;
            temp2->value = tempVal;
            temp2->b = tempB;
            ret = temp2;
        }
    }
    if(strcmp(ret->value,"18killstreak") == 0){
        //Expr* temp2 = initExp();
        //free(temp2->value);
        //temp2->value = strdup(ret->a->value);
        return ret->a;
    }

    return ret;
}

Expr* parseLit(){
    Expr* ret = initExp();
    if(strcmp(tokens[i].tt,"INTEGER")==0){
        ret->value = tokens[i++].value;
    }
    else if(strcmp(tokens[i].tt,"CHAR")==0){
        ret->value = tokens[i++].value;
    }
    else if(strcmp(tokens[i].tt,"IDENT")==0){
        ret->value = parseID();
    }
    return ret;
}

Expr* parseLit2(){
    Expr* ret = (Expr *)malloc(sizeof(Expr));
    //prepExpr(ret);
    if(strcmp(tokens[i].value,"(")==0){
        i++;
        Expr* ex = parseExp1();
        ret->a = ex;
        i++;
    }
    return ret->a;
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

Cout parseCout(){
    Cout c;
    int noOfExprs = 0;
    c.exprs = (Expr*)malloc(5 * sizeof(Expr));
    while(strcmp(tokens[i].value,";") != 0){
        if(strcmp(tokens[i].value,"cout") == 0 || strcmp(tokens[i].value,"<") == 0){
            i++;
            continue;
        }
        if(strcmp(tokens[i].tt,"IDENT") == 0){
            c.exprs[noOfExprs++] = *parseExp7();
            if(strcmp(tokens[--i].value,";")==0){
                i++;
                break;
            }

        }
        else if(strcmp(tokens[i].value,",")==0){
            i++;
            continue;
        }
        else{
            printf("\nerror in syntax with token %d\n",i);
        }
        c.exprs[noOfExprs] = *initExp();
        i++;
    }
    //printf("%d,%s",i,tokens[i].value);
    i++;//move past semicolon
    //c.exprs[noOfExprs++] =;
    return c;
}

void printCout(Cout c){
    printf("\ncout ");
    int count = 0;
    while(!strcmp(c.exprs[count].value,"18killstreak") == 0){
        printf(">> expression\n");
        printExpr(&c.exprs[count++]);
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
    w.boolExp = parseExp1();
     w.statements = (Statements*)malloc(sizeof(Statements));
    i++;//pass the {
    *w.statements = parseStatements();
    i++;//pass the }
    return w;
}

void printWhile(While whyle){
    printf("here in print while");
    printExpr(whyle.boolExp);
    printStatements(*whyle.statements);
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

void evaluateExpr(Expr* ex){
    if(strcmp(ex->value,"18killstreak") == 0){
        return;
    }
    evaluateExpr(ex->a);
    evaluateExpr(ex->b);
    int a,b;
    if(!strcmp(ex->a->value,"18killstreak") == 0 && !strcmp(ex->b->value,"18killstreak") == 0){
        a = atoi(ex->a->value);
        b = atoi(ex->b->value);
        if(isdigit(ex->a->value[0]) == 0){
            if(isSymbol(ex->a->value)){
                a = atoi(getSymbolVal(ex->a->value));
            }
        }
        if(isdigit(ex->b->value[0]) == 0){
            if(isSymbol(ex->b->value)){
                b = atoi(getSymbolVal(ex->b->value));
            }
        }
        if(strcmp(ex->value,"+") == 0){
            strdup(itoa(a+b,ex->value,10));
        }
        if(strcmp(ex->value,"-") == 0){
            strdup(itoa(a-b,ex->value,10));
        }
        if(strcmp(ex->value,"&&") == 0){
            strdup(itoa(a&&b,ex->value,10));
        }
        if(strcmp(ex->value,"||") == 0){
            strdup(itoa(a||b,ex->value,10));
        }
        if(strcmp(ex->value,"*") == 0){
            strdup(itoa(a*b,ex->value,10));
        }
        if(strcmp(ex->value,"/") == 0){
            strdup(itoa(a/b,ex->value,10));
        }
        if(strcmp(ex->value,"%") == 0){
            strdup(itoa(a%b,ex->value,10));
        }
        if(strcmp(ex->value,">") == 0){
            strdup(itoa(a>b,ex->value,10));
        }
        if(strcmp(ex->value,">=") == 0){
            strdup(itoa(a>=b,ex->value,10));
        }
        if(strcmp(ex->value,"<") == 0){
            strdup(itoa(a<b,ex->value,10));
        }
        if(strcmp(ex->value,"<=") == 0){
            strdup(itoa(a<=b,ex->value,10));
        }
    }
}

int evaluateBoolExp(Expr* exp){
    Expr* ex = initExp();
    copyExpr(exp, ex);
    evaluateExpr(ex);
    if(atoi(ex->value)){
        return 1;
    }
    return 0;
}

void copyExpr(Expr* ex1,Expr* ex2){
    if(strcmp(ex1->value,"18killstreak") == 0){
        return;
    }
    ex2->value = strdup(ex1->value);
    ex2->a = initExp();
    ex2->b = initExp();
    copyExpr(ex1->a,ex2->a);
    copyExpr(ex1->b,ex2->b);
}

void copyAssign(Assign* a1,Assign* a2){
    a2->op = a1->op;
    a2->identfier = a1->identfier;
    a2->expression = initExp();
    copyExpr(a1->expression,a2->expression);
}

int isSymbol(char *str) {
    int j = symbolTable.noOfElem - 1;
    while(j >= 0){
        if(strcmp(symbolTable.hashTable[j].name,str) == 0){
            return 1;
        }
        j--;
    }
    return 0;
}

char* getSymbolVal(char *str) {
    int j = symbolTable.noOfElem - 1;
    while(j >= 0){
        if(strcmp(symbolTable.hashTable[j].name,str) == 0){
            char* ret = strdup(symbolTable.hashTable[j].value);
            return ret;
        }
        j--;
    }
}