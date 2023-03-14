#ifndef SIMPLESHELL_CUSTOM_VAR_H
#define SIMPLESHELL_CUSTOM_VAR_H
#define MAX_TABLE_SIZE  1000

#include "custom-string.h"

typedef struct {
    string identifier;
    string value;
} var;

typedef struct {
    var * variables;
    size_t size;
} table;


table newTable(){
    table vars;
    vars.variables = (var *) malloc(MAX_TABLE_SIZE * sizeof(var));
    vars.size = 0;
    return vars;
}

void printVar(var variable){
    printf("%s -> %s\n" , variable.identifier.arr,variable.value.arr);
}

void printTable(table table){
    for (int i = 0; i < table.size; ++i) printVar(table.variables[i]);
}

int findInTable(char * s,table var_table){
    for (int i = 0; i < var_table.size; ++i) {
        if (!strcmp(s,var_table.variables[i].identifier.arr)) {
            return i;
        }
    }
    return -1;
}

#endif
