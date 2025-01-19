#ifndef HISTORY_H
#define HISTORY_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HISTORY 100
#define MAX_COMMAND_LENGTH 256

typedef struct {
    char *commands[MAX_HISTORY];
    int count;
    int current;
} History;

extern History history;

void init_history(History *history);
void add_history(History *history, const char *command);
const char *get_prev_history(History *history);
const char *get_next_history(History *history);
void free_history(History *history);

#endif  // HISTORY_H