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
