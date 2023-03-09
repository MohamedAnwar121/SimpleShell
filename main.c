#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>

#include "custom-var.h"

#define GREEN "\033[1m\033[32m"
#define BLUE "\033[1m\033[34m"
#define RESET "\033[0m"


table var_table;
FILE *logging;

void checkForError(int result) {
    if (result < 0) {
        perror("Error");
        exit(1);
    }
}

void printCWD() {
    char cwd[1024];
    printf(GREEN"%s@%s:"RESET, getenv("USER"), getenv("USER"));
    printf(BLUE"%s$ "RESET, getcwd(cwd, 1024));
}

void writeToLog() {
    logging = fopen("/home/mohamed/Projects/SimpleShell/logging.txt", "a");

    if (logging == NULL) {
        printf("failed to open file\n");
        return;
    }

    fprintf(logging, "Child process was terminated\n");
    fclose(logging);
}

void on_child_exit(int signal) {
    int status;
    pid_t pid;

    pid = waitpid((pid_t) -1, &status, WNOHANG);
    writeToLog();
}

void setup_environment() {
    chdir(getenv("HOME"));
}

char *handleDollarSign(string input) {

    string s;
    s.arr = new(char *,input.length + 1);

    s.arr = strdup(input.arr);
    s.length = input.length + 1;

    s.arr[s.length - 1] = ' ';

    string temp;
    temp.arr = (char *) malloc(MAX_STRING_SIZE * sizeof(char));
    temp.length = 0;


    char *result;
    result = (char *) malloc(MAX_STRING_SIZE * sizeof(char));
    int size = 0;

    int visited = 0;
    for (int i = 0; i < s.length; ++i) {
        if (s.arr[i] == '$') {
            visited++;
            continue;
        }

        if (!visited) result[size++] = s.arr[i];

        if (visited) {
            if (s.arr[i] != ' ') temp.arr[temp.length++] = s.arr[i];
            else {
                visited--;
                temp.arr[temp.length] = '\0';
                if (findInTable(temp.arr, var_table) != -1) {
                    int index = findInTable(temp.arr, var_table);
                    string rest_of_result = var_table.variables[index].value;
                    for (int j = 0; j < rest_of_result.length; ++j) result[size++] = rest_of_result.arr[j];
                }
                result[size++] = ' ';

                free(temp.arr);
                temp.arr = (char *) malloc(MAX_STRING_SIZE * sizeof(char));
                temp.length = 0;
            }
        }
    }

    result[size] = '\0';

    return result;
}

void cd(stringArray input) {

    string dir = input.arr[1];

    if (!strcmp(dir.arr, "~")) {
        chdir(getenv("HOME"));
    } else {
        chdir(dir.arr);
    }
}

void echo(string input) {
    string value;
    value.arr = (char *) malloc((input.length - 5) * sizeof(char));
    value.length = input.length - 5;

    for (int i = 0; i < value.length; ++i) {
        value.arr[i] = input.arr[i + 5];
    }

    value = remove_quotes(value);

    char *result = handleDollarSign(value);
    printf("%s\n", result);
}

void export(string input) {

    string value;
    value.arr = new(char *, MAX_STRING_SIZE);
    value.length = 0;

    for (int i = 7; i < input.length; ++i) value.arr[value.length++] = input.arr[i];
    value.arr[value.length] = '\0';

    char temp_var[MAX_STRING_SIZE] = "";
    size_t var_size = 0;

    int start_index = 0;
    for (int i = 0; i < value.length; ++i) {
        if (value.arr[i] != '=') temp_var[var_size++] = value.arr[i];
        else {
            start_index = i;
            break;
        }
    }


    char temp_val[MAX_STRING_SIZE] = "";
    size_t val_size = 0;

    for (int i = start_index + 1; i < value.length; ++i)
        temp_val[val_size++] = value.arr[i];


    string temp_str;
    temp_str.arr = temp_val;
    temp_str.length = val_size;

    if (temp_val[0] == '"' && temp_val[val_size - 1] == '"')
        temp_str = remove_quotes(temp_str);

    if (findInTable(temp_var, var_table) != -1) {
        int index = findInTable(temp_var, var_table);
        strcpy(var_table.variables[index].value.arr, temp_str.arr);
        var_table.variables[index].value.length = temp_str.length;
    } else {
        var variable;

        variable.identifier.arr = (char *) malloc(var_size * sizeof(char));
        strcpy(variable.identifier.arr, temp_var);
        variable.identifier.length = var_size;

        variable.value.arr = (char *) malloc(val_size * sizeof(char));
        strcpy(variable.value.arr, temp_val);
        variable.value.length = val_size;

        var_table.variables[var_table.size++] = variable;
    }

}

void execute_shell_builtin(string s) {

    stringArray input = splitString(s);

    char *command = input.arr[0].arr;

    if (!strcmp(command, "cd")) cd(input);
    else if (!strcmp(command, "echo")) echo(s);
    else if (!strcmp(command, "export")) export(s);

}

void execute_command(string s) {

    char *temp = handleDollarSign(s);
    s.arr = strdup(temp);

    int status, res;
    int background = 0;
    pid_t pid;

    stringArray strings = splitString(s);
    char **commands = stringArrayTo2DChar(strings);

    if (!strcmp(commands[strings.length - 1], "&")) {
        background = 1;
        commands[strings.length - 1] = NULL;
    } else {
        commands = realloc(commands, (strings.length + 1) * sizeof(char **));
        commands[strings.length] = NULL;
    }



    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork Failed");
        return;
    } else if (pid == 0) {
        res = execvp(strings.arr[0].arr, commands);
        checkForError(res);
    } else {
        if (!background)
            waitpid(pid, &status, 0);
    }
}

string expression_evaluation(string s) {

    stringArray strings = splitString(s);
    string command = strings.arr[0];
    string result = newString();

    if (!strcmp(command.arr, "cd") || !strcmp(command.arr, "echo") || !strcmp(command.arr, "export")) {
        result.arr = "execute_shell_builtin";
    } else if (!strcmp(command.arr, "exit")) {
        result.arr = "exit";
    } else {
        result.arr = "execute_command";
    }
    return result;
}


void shell() {
    var_table = newTable();

    while (1) {
        printCWD();

        string input = scanString();
        string input_type = expression_evaluation(input);

        if (!strcmp(input_type.arr, "execute_shell_builtin")) execute_shell_builtin(input);
        else if (!strcmp(input_type.arr, "execute_command")) execute_command(input);
        else if (!strcmp(input_type.arr, "exit")) {
            kill(0, SIGKILL);
            break;
        }
    }
}


int main() {
    signal(SIGCHLD, on_child_exit);
    setup_environment();
    shell();
    return 0;
}

