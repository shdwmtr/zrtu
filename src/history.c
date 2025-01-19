
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "history.h"

#define MAX_HISTORY 100
#define MAX_COMMAND_LENGTH 256

History history;

void init_history(History *history) {
    history->count = 0;
    history->current = -1;
    for (int i = 0; i < MAX_HISTORY; i++) {
        history->commands[i] = NULL;
    }
}

void add_history(History *history, const char *command) {
    if (history->count > 0 && strcmp(history->commands[history->count - 1], command) == 0) {
        // Do not add if the command is the same as the last one
        return;
    }

    if (history->count < MAX_HISTORY) {
        history->commands[history->count] = strdup(command);
        history->count++;
    } else {
        free(history->commands[0]);
        for (int i = 1; i < MAX_HISTORY; i++) {
            history->commands[i - 1] = history->commands[i];
        }
        history->commands[MAX_HISTORY - 1] = strdup(command);
    }
    history->current = history->count;
}

const char *get_prev_history(History *history) {
    if (history->current > 0) {
        history->current--;
        return history->commands[history->current];
    }
    return NULL; 
}

const char *get_next_history(History *history) {
    if (history->current < history->count - 1) {
        history->current++;
        return history->commands[history->current];
    }
    return NULL; 
}

void free_history(History *history) {
    for (int i = 0; i < history->count; i++) {
        free(history->commands[i]);
    }
}
