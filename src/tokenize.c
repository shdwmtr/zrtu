#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "tokenize.h"
#include <stdio.h>
#include <ctype.h>

char* current_position = NULL;  // pointer to track position in the line
char* prev_position = NULL;  // pointer to track the previous position in the line

int get_next_token(char* line, char* token) {

    // prev_position = current_position;

    if (line != NULL) {
        // If a new line is passed, reset the pointer to start from the beginning of the line
        current_position = line;
    }

    // Skip any leading whitespace
    while (current_position != NULL && isspace(*current_position)) {
        current_position++;
    }

    // If we reach the end of the line
    if (current_position == NULL || *current_position == '\0') {
        return END_OF_STATEMENT;
    }

    // Check for stdin redirection '<'
    if (*current_position == '<') {
        token[0] = '<';
        token[1] = '\0';  // Null-terminate the token
        current_position++;
        return TOKEN_STDIN_REDIRECT;
    }

    // Check for stdout redirection '>'
    if (*current_position == '>') {
        token[0] = '>';
        token[1] = '\0';  // Null-terminate the token
        current_position++;
        return TOKEN_STDOUT_REDIRECT;
    }

    // If the token is an operator (e.g., "&&", "||")
    if (*current_position == '&' || *current_position == '|') {
        int op_len = 1;
        if (*(current_position + 1) == *current_position) {
            op_len = 2;  // For "&&" or "||"
        }
        strncpy(token, current_position, op_len);
        token[op_len] = '\0';  // Null-terminate the token
        current_position += op_len;
        return TOKEN_OPERATOR;
    }

    // Otherwise, it's a parameter (any other non-space characters)
    int i = 0;
    while (current_position[i] != '\0' && !isspace(current_position[i]) && current_position[i] != '&' && current_position[i] != '|' && current_position[i] != '<' && current_position[i] != '>') {
        token[i] = current_position[i];
        i++;
    }

    if (i == 0) {
        return END_OF_STATEMENT;  // No parameter found
    }

    token[i] = '\0';  // Null-terminate the parameter string
    current_position += i;

    return TOKEN_PARAMETER;
}
