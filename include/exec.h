#ifndef EXEC_H
#define EXEC_H

int is_path_exec(char* path);

int exec_child_proc(int argc, char** argv);

int handle_exec(int token_type, char* token, char* statement);

#endif // EXEC_H