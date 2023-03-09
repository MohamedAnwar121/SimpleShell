#ifndef SIMPLESHELL_CUSTOM_STRING_H
#define SIMPLESHELL_CUSTOM_STRING_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define  new(type,size)       (type) malloc( size * sizeof(type) )
#define MAX_STRING_SIZE       100
#define MAX_STRING_ARRAY_SIZE 20

typedef struct {
    char * arr;
    size_t length;
} string;

typedef struct {
    string * arr;
    size_t length;
} stringArray;

string newString() {
    string s;
    s.arr = NULL;
    s.length = 0;
    return s;
}

stringArray newStringArray(){
    stringArray strings;
    strings.arr = NULL;
    strings.length = 0;
    return strings;
}

string scanString() {
    string s = newString();

    ssize_t line_len = getline(&s.arr, &s.length, stdin);
    fflush(stdin);

    if (line_len == -1) {
        string error = {"error", strlen("error")};
        printf("Error: failed to read input.\n");
        return error;
    }

    s.length = strlen(s.arr);
    s.arr[s.length-- - 1] = '\0';

    return s;
}

void printString(string s) {
    printf("%s\n", s.arr);
}

void printStringArray(stringArray strings){
    for (int i = 0; i < strings.length; ++i) {
        printString(strings.arr[i]);
    }
}

stringArray splitString(string s) {
    char splitter[s.length];
    strcpy(splitter,s.arr);

    stringArray strings = newStringArray();
    strings.arr = (string *) malloc(MAX_STRING_ARRAY_SIZE * sizeof(string));

    char * token = strtok(splitter, " ");
    while (token != NULL) {
        string temp;

        temp.arr = (char *) malloc(MAX_STRING_SIZE * sizeof(char));

        strcpy(temp.arr, token);
        temp.length = strlen(temp.arr);

        temp.arr = realloc(temp.arr,temp.length * sizeof(char ));

        strings.arr[strings.length++] = temp;
        token = strtok(NULL, " ");
    }

    strings.arr = realloc(strings.arr, strings.length * sizeof(string));
    return strings;
}

string clearString(string s){
    s.arr[0] = '\0';
    s.length = 0;
    return s;
}

stringArray clearStringArray(stringArray strings){
    free(strings.arr);
    strings.length = 0;
    return strings;
}

string remove_quotes(string s){

    if (s.arr[0] == '"' && s.arr[s.length - 1] == '"'){
        for (int i = 0; i < s.length - 1; ++i)
            s.arr[i] = s.arr[i+1];

        s.arr[s.length - 2] = '\0';
        s.length -= 2;
    }
    return s;
}

char ** stringArrayTo2DChar(stringArray strings){
    char ** result = new(char **,strings.length);
    for (int i = 0; i < strings.length; ++i) result[i] = strdup(strings.arr[i].arr);
    return result;
}

#endif

/*char * token = strtok(splitter, " ");
    while (token != NULL) {
        string temp;

        printf("%s",token);

        strcpy(temp.arr,token);
        temp.length = strlen(temp.arr);

        strings.arr[strings.length++] = temp;
        token = strtok(NULL, " ");
    }*/

/*
 * string temp = newString();
    temp.arr = (char *) malloc(s.length * sizeof(char));

    for (int i = 0; i < s.length; ++i) {
        if (s.arr[i] != ' ') temp.arr[temp.length++] = s.arr[i];
        else {
            temp.arr = realloc(temp.arr,temp.length * sizeof(char));

            strings.arr[strings.length].arr = (char *)malloc(temp.length* sizeof(char));
            strings.arr[strings.length].arr = strcpy(strings.arr[strings.length].arr,temp.arr);
            strings.arr[strings.length++].length = temp.length;

            free(temp.arr);
            temp.length = 0;

            temp.arr = (char *) malloc(s.length * sizeof(char));
        }
    }

    temp.arr = realloc(temp.arr,temp.length * sizeof(char));

    strings.arr[strings.length].arr = (char *)malloc(temp.length* sizeof(char));
    strings.arr[strings.length].arr = strcpy(strings.arr[strings.length].arr,temp.arr);
    strings.arr[strings.length++].length = temp.length;
    free(temp.arr);
    */

/*string temp;
    temp.arr = (char *) malloc(MAX_STRING_SIZE * sizeof(char));
    temp.length = 0;

    char *result;
    result = (char *) malloc(MAX_STRING_SIZE * sizeof(char));
    int size = 0;

    int visited = 0;
    for (int i = 0; i < n + 1; ++i) {
        if (s[i] == '$'){
            visited++;
            continue;
        }

        if (!visited) result[size++] = s[i];

        if (visited){
            if (s[i] != ' ') temp.arr[temp.length++] = s[i];
            else {
                visited--;
                if (findInTable(temp.arr, var_table) != -1) {
                    int index = findInTable(temp.arr, var_table);
                    string rest_of_result = var_table.variables[index].value;

                    for (int j = 0; j < rest_of_result.length; ++j) result[size++] = rest_of_result.arr[j];
                }
            }
        }

        *//*if (s[i] == '$' && !visited) {
            visited++;
            continue;
        } else if (!visited) {
            result[size++] = s[i];
        }

        if (s[i] != ' ' && visited && i < n) {
            temp.arr[temp.length++] = s[i];
            continue;
        } else if (visited) {
            visited--;

            if (findInTable(temp.arr, var_table) != -1) {
                int index = findInTable(temp.arr, var_table);
                string rest_of_result = var_table.variables[index].value;

                for (int j = 0; j < rest_of_result.length; ++j) result[size++] = rest_of_result.arr[j];
            }
        }*//*
    }

    return result;*/