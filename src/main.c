#include <stdio.h>
#include <stdlib.h>
#include "user.h"
#include "builtins.h"
#include "tokenize.h"
#include "exec.h"
#include "forward.h"
#include <string.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <cd.h>
#include <pwd.h>
#include <termios.h>
#include <history.h>
#include <prompt.h>

void process_statement(char* statement) {
    char token[256];
    int token_type;

    token_type = get_next_token(statement, token);

    if (token_type == END_OF_STATEMENT) {
        return;
    }

    struct builtin_command builtin;

    if (get_builtin(token, &builtin) == 0) {
        process_builtin(token_type, token, statement, &builtin);
        return;
    }
    else if (is_path_exec(token)) {
        handle_exec(token_type, token, statement);
    }
    else {
        fprintf(stderr, "Unknown command: %s\n", token);
    }
}

void process_input(char *script) {
    char* statement = script;
    char* next_operator;

    while (statement != NULL && *statement != '\0') {
        next_operator = strstr(statement, "&&");
        if (next_operator == NULL) {
            next_operator = strstr(statement, "||");
        }

        if (next_operator != NULL) {
            *next_operator = '\0';
            process_statement(statement); 
            statement = next_operator + 2;
        } else {
            process_statement(statement);
            break;
        }
    }
}

// Function to set terminal to raw mode to capture special keys
void set_raw_mode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echoing
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Function to restore terminal to normal mode
void restore_terminal() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);  // Re-enable canonical mode and echoing
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

char *read_input() {
    char ch, *line = malloc(1024);
    int pos = 0;

    while (1) {
        ch = getchar();

        if (ch == 27) { 
            getchar();
            ch = getchar();

            if (ch == 'A' || ch == 'B') {
                const char* command = ch == 'A' ? get_prev_history(&history) : get_next_history(&history);

                if (command) {
                    show_prompt();
                    printf("%s", command);
                    line = strdup(command);
                    pos = strlen(line);
                    fflush(stdout);
                }
            }

        } 
        else if (ch == '\n') { 
            line[pos++] = '\n';
            line[pos] = '\0'; 
            break;
        } 
        else if (ch == 127) {
            if (pos > 0) {
                pos--;
                printf("\b \b"); 
            }
        } 
        else { 
            line[pos++] = ch;
            putchar(ch);
        }
    }
    return line;
}

int start_shell() {
    setup_shell();
    set_raw_mode();
    init_history(&history);

    char *line;
    while (1) {
        show_prompt();
        line = read_input();
        add_history(&history, line);
        
        if (!line || strcmp(line, "exit") == 0) {
            free(line);
            break;
        }

        printf("\n");
        process_input(line);
        
        free(line);
    }

    restore_terminal();
    return 0;
}

int main(int argc, char *argv[]) {
    return (start_shell());
}
