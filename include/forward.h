#ifndef FORWARD_H
#define FORWARD_H

int forward(int token_type, char* statement, char* token, int* _argc, char*** _argv);

int free_argv(int argc, char** argv);
#endif