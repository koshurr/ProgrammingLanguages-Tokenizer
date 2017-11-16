//
// Created by yakov on 11/14/2017.
//

#include <io.h>
#include <stdio.h>
#include <malloc.h>
#include <windef.h>
#include <stdbool.h>
#include "Tokenizer.h"


/*
 * test function
 * to see results of parser
 */
char getDir(){
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        fprintf(stdout, "Current working dir: %s\n", cwd);
    else
        perror("getcwd() error");
}