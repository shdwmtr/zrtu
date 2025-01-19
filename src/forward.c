#include <stdio.h>
#include "tokenize.h"
#include <stdlib.h>
#include <string.h>

/**
 * Given the first token of a statement, forward it and parse the remaining tokens to the end of the statement. 
 * @note The caller is responsible for freeing the memory allocated for the argv array. You can use the free_argv function for this purpose.
 */
int forward(int token_type, char* statement, char* token, int* _argc, char*** _argv) {
    int argc = 0;
    char** argv = malloc(sizeof(char*) * 2);
    
    while ((token_type = get_next_token(statement, token)) != END_OF_STATEMENT) {
        if (token_type == TOKEN_PARAMETER) {
            argc++; 
            argv = realloc(argv, sizeof(char*) * argc);

            if (!argv) {
                fprintf(stderr, "Memory allocation failed\n");
                return 1;
            }
            argv[argc - 1] = strdup(token);
        }
        else {
            *_argc = argc;
            *_argv = argv;

            return token_type;
        }
        statement = NULL;
    }

    *_argc = argc;
    *_argv = argv;
    return 0;
}

/**
 * Free the memory allocated for the argv array.
 */
int free_argv(int argc, char** argv) {
    for (int i = 0; i < argc - 1; i++) {
        free(argv[i]);
    }
    free(argv);
    return 0;
}
