#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <ctype.h>
#include <stdbool.h>

char *paths[100] = {NULL};

void error() {
    fprintf(stderr, "An error has occurred\n");
}

// Check if the input character is a delimiter
int isDelim(char input_character) {
    if (input_character == ' ' || input_character == '\t' || input_character == '>' || input_character == '&') {
        return 1;
    }
    return 0;
}

// Parse the input into tokens
void parseCommands(char *buffer, char *tokens[]) {
    int buffer_len = strlen(buffer);
    int sstring_start = -1;
    int j = 0;
    for (int i = 0; i < buffer_len; i++) {
        if (buffer[i] == '>' || buffer[i] == '&') {
            tokens[j] = malloc(sizeof(char) + 1);
            strncpy(tokens[j], &buffer[i], 1);
            j++;
            continue;
        }
        if (!isDelim(buffer[i])) {
            if (i == 0 || isDelim(buffer[i - 1])) {
                sstring_start = i;
            }
            if (i == buffer_len - 1 || isDelim(buffer[i + 1])) {
                int substring_len = i - sstring_start + 1;
                tokens[j] = malloc((substring_len + 1) * sizeof(char));
                strncpy(tokens[j], &buffer[sstring_start], substring_len);
                j++;
            }
        }
    }
}

// Parse the input into commands
int parseInput(char *buffer, char *commands[]) {
    char *token, *rest = buffer;
    int i = 0;
    while ((token = strsep(&rest, "&")) != NULL) {
        if (token[0] != '\0') {
            commands[i] = malloc(strlen(token) + 1);
            strcpy(commands[i], token);
            i++;
        }
    }
    return i;
}

// Change directory built-in command
void cd(char *directory) {
    if (chdir(directory) == -1) {
        error();
    }
}

// Path built-in command
void path(char *inputs[]) {
    if (inputs[1] == NULL) {
        int i = 0;
        while (paths[i] != NULL) {
            paths[i] = NULL;
            i++;
        }
    }
    int i = 1;
    while (inputs[i] != NULL) {
        paths[i - 1] = (char *) malloc(strlen(inputs[i]) + 1);
        strcpy(paths[i - 1], inputs[i]);
        i++;
    }
}


// Execute built-in commands
void builtInCommands(char *inputs[]) {
    if (strcmp(inputs[0], "exit") == 0) {
        if (inputs[1] != NULL) {
            error();
            return;
        } else {
            exit(0);
        }
    }
    if (strcmp(inputs[0], "cd") == 0) {
        if (inputs[1] == NULL || inputs[2] != NULL) {
            error();
            return;
        } else {
            cd(inputs[1]);
            return;
        }
    }
    if (strcmp(inputs[0], "path") == 0) {
        path(inputs);
        return;
    }
}

// Check if the command is in the path
char *checkPath(char *input) {
    int i = 0;
    while (paths[i] != NULL) {
        char *path = (char *) malloc(strlen(paths[i]) + strlen(input) + 2);
        strcpy(path, paths[i]);
        strcat(path, "/");
        strcat(path, input);
        if (access(path, X_OK) == 0) {
            return path;
        }
        i++;
    }
    return NULL;
}

// Check if the command has redirection
int hasRedirection(char *command[]) {
    int i = 0;
    while (command[i] != NULL) {
        if (strcmp(command[i], ">") == 0) {
            return i;
        }
        i++;
    }
    return -1;
}

// Check if the redirection is valid
bool checkRedirection(char *command[]) {
    int i = 0;
    while (command[i] != NULL) {
        if (strcmp(command[i], ">") == 0) {
            if (command[i + 1] == NULL || command[i + 2] != NULL || i == 0) {
                return false;
            }
        }
        i++;
    }
    return true;
}

// Redirect the output of the command to a file
void redirection(char *command[]) {
    int i = 0;
    while (command[i] != NULL) {
        if (strcmp(command[i], ">") == 0) {
            int fd = open(command[i + 1], O_WRONLY | O_CREAT | O_TRUNC);
            dup2(fd, STDOUT_FILENO);
            close(fd);
            command[i] = NULL;
            break;
        }
        i++;
    }
}


// Execute Commands
void executeCommand(char *commands) {
    char *tokens[100] = {NULL};
    parseCommands(commands, tokens);
    if (strcmp(tokens[0], "exit") == 0 || strcmp(tokens[0], "cd") == 0 || strcmp(tokens[0], "path") == 0) {
        builtInCommands(tokens);
        return;
    }
    char *path = checkPath(tokens[0]);
    if (path == NULL) {
        error();
        return;
    }
    int pid = fork();
    if (pid == 0) {
        if (hasRedirection(tokens) != -1) {
            if (checkRedirection(tokens) == true) {
                redirection(tokens);
            } else {
                error();
                exit(1);
            }
        }
        execv(path, tokens);
        error();
        exit(1);
    }
}

// Get the length of an input array
int getArrayLength(char *array[]) {
    int i = 0;
    while (array[i] != NULL) {
        i++;
    }
    return i;
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        if (argc == 2) {
            paths[0] = "/bin";
            struct stat st;
            stat(argv[1], &st);
            if (st.st_size <= 0) {
                error();
                exit(1);
            }
            FILE *fp = fopen(argv[1], "r");
            if (fp == NULL) {
                error();
                exit(1);
            }
            char *buffer = NULL;
            size_t len = 0;
            while (getline(&buffer, &len, fp) != -1) {
                buffer[strlen(buffer) - 1] = '\0';
                if (buffer[0] == '\0') {
                    continue;
                }
                bool empty = true;
                for (int i = 0; i < strlen(buffer); i++) {
                    if (!isspace(buffer[i])) {
                        empty = false;
                        break;
                    }
                }
                if (empty) {
                    continue;
                }

                char *commands[100] = {NULL};
                parseInput(buffer, commands);
                for (int i = 0; i < getArrayLength(commands); i++) {
                    executeCommand(commands[i]);
                }
                for (int i = 0; i < getArrayLength(commands); i++) {
                    wait(NULL);
                }
            }
        } else {
            error();
            exit(1);
        }
    } else if (argc == 1) {
        paths[0] = "/bin";
        for (;;) {
            printf("wish> ");
            char *buffer = NULL;
            size_t len = 0;
            getline(&buffer, &len, stdin);
            buffer[strlen(buffer) - 1] = '\0';
            bool empty = true;
            for (int i = 0; i < strlen(buffer); i++) {
                if (!isspace(buffer[i])) {
                    empty = false;
                    break;
                }
            }
            if (empty) {
                continue;
            }
            char *commands[100] = {NULL};
            parseInput(buffer, commands);
            for (int i = 0; i < getArrayLength(commands); i++) {
                executeCommand(commands[i]);
            }
            for (int i = 0; i < getArrayLength(commands); i++) {
                wait(NULL);
            }
        }
    }
}
