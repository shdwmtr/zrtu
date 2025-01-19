
#include <stddef.h>
#include "builtins.h"

// include the builtin command headers
#include "cd.h"
#include "pwd.h"

#include "tokenize.h"
#include "forward.h"    
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct builtin_command builtin_commands[] = {
    // {"exit", exit_shell},
    {"cd", set_working_directory},
    {"pwd", print_working_directory},
    // {"echo", echo},
    // {"help", help},
    { NULL, NULL }
};

int _builtin_command_count = (int)(sizeof(builtin_commands) / sizeof(builtin_commands[0]));

int get_builtin(char *command, struct builtin_command *builtin) {
    for (int i = 0; i < _builtin_command_count; i++) {
        if (builtin_commands[i].name != NULL && strcmp(command, builtin_commands[i].name) == 0) {
            *builtin = builtin_commands[i];
            return 0;
        }
    }
    return 1;
}

int process_builtin(int token_type, char *token, char* statement, struct builtin_command *builtin) {
    int argc = 0;
    char** argv;
    
    if (forward(token_type, statement, token, &argc, &argv) != 0) {
        printf("Failed to forward\n");
        return 1;
    }

    int status = (*builtin).function(argc, argv);
    free_argv(argc, argv);

    return status;
}