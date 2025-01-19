#ifndef BUILTINS_H
#define BUILTINS_H

struct builtin_command {
    char *name;
    int (*function)(int argc, char **argv);
};

extern struct builtin_command builtin_commands[];

extern int _builtin_command_count;  

int get_builtin(char *command, struct builtin_command *builtin);

int process_builtin(int token_type, char *token, char* statement, struct builtin_command *builtin);

#endif // BUILTINS_H

