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

/*var_table = newTable();
var_table.variables[0].identifier.arr = (char *) malloc(1 * sizeof(char));
var_table.variables[0].identifier.arr = "x";
var_table.variables[0].identifier.length = 1;

var_table.variables[0].value.arr = (char *) malloc(1 * sizeof(char));
var_table.variables[0].value.arr = "5";
var_table.variables[0].value.length = 1;

var_table.size = 1;

char *s = "echo \"hello $x\"";
string s1 = {s, strlen(s)};
echo(s1);*/

/*stringArray strings = splitString(s);

        for (int i = 0; i < strings.length; ++i) {
            if (strings.arr[i].arr[0] == '$') {

                for (int j = 0; strings.arr[i].arr[j] != '\0'; j++) {
                    strings.arr[i].arr[j] = strings.arr[i].arr[j + 1];
                }

                if (findInTable(strings.arr[i].arr, var_table) != -1) {
                    int index = findInTable(strings.arr[i].arr, var_table);
                    strings.arr[i].arr = strdup(var_table.variables[index].value.arr);
                    strings.arr[i].length = strlen(strings.arr[i].arr);
                }
            }
        }

        char *result = new(char *, MAX_STRING_SIZE);
        long size = 0;

        for (int i = 0; i < strings.length; ++i) {
            for (int j = 0; strings.arr[i].arr[j] != '\0'; ++j) {
                result[size++] = strings.arr[i].arr[j];
            }
            result[size++] = ' ';
        }

        result[size] = '\0';

        return result;*/
#endif
