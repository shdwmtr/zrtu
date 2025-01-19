#ifndef TOKENIZER_H
#define TOKENIZER_H

extern char* current_position;
extern char* prev_position;

enum {
    END_OF_STATEMENT = 2,
    TOKEN_OK = 3,
    TOKEN_OPERATOR = 4,
    TOKEN_PARAMETER = 5,
    TOKEN_STDIN_REDIRECT = 6,
    TOKEN_STDOUT_REDIRECT = 7,
};

int get_next_token(char* line, char* token);
#endif
