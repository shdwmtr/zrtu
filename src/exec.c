#include <sys/stat.h>
#include <stddef.h>
#include <unistd.h>
#include "user.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "forward.h"
#include "tokenize.h"

int is_path_exec(char* pname) {
    if (!path_env) {
        fprintf(stderr, "PATH environment variable not provided.\n");
        return 0;
    }

    char *path_copy = strdup(path_env);
    if (!path_copy) {
        perror("strdup");
        return 0;
    }

    char *dir = strtok(path_copy, ":");
    while (dir) {
        char full_path[4096];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, pname);

        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            return 1;
        }

        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return 0; 
}

int exec_child_proc(int argc, char** argv, const char* input, char** output) {
    int stdout_pipe[2];
    int stdin_pipe[2];
    pid_t pid;

    if (pipe(stdout_pipe) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if (pipe(stdin_pipe) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { 
        // Child process
        close(stdout_pipe[0]); // Close unused read end of stdout pipe
        close(stdin_pipe[1]);  // Close unused write end of stdin pipe

        if (dup2(stdout_pipe[1], STDOUT_FILENO) == -1) {
            perror("dup2 stdout");
            exit(EXIT_FAILURE);
        }
        close(stdout_pipe[1]); // Close the duplicated stdout write end

        if (dup2(stdin_pipe[0], STDIN_FILENO) == -1) {
            perror("dup2 stdin");
            exit(EXIT_FAILURE);
        }
        close(stdin_pipe[0]); // Close the duplicated stdin read end

        execvp(argv[0], argv);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else { 
        // Parent process
        close(stdout_pipe[1]); // Close unused write end of stdout pipe
        close(stdin_pipe[0]);  // Close unused read end of stdin pipe

        // Write input to the child's stdin
        if (input != NULL) {
            size_t input_length = strlen(input);
            if (write(stdin_pipe[1], input, input_length) != input_length) {
                perror("write to stdin");
                close(stdin_pipe[1]);
                close(stdout_pipe[0]);
                exit(EXIT_FAILURE);
            }
        }
        close(stdin_pipe[1]); // Close the write end of stdin pipe after writing

        *output = malloc(1024);
        if (*output == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        size_t buffer_size = 1024;
        size_t output_length = 0;

        char buffer[1024];
        ssize_t count;
        while ((count = read(stdout_pipe[0], buffer, sizeof(buffer))) > 0) {
            if (output_length + count + 1 > buffer_size) {
                buffer_size *= 2;
                *output = realloc(*output, buffer_size);
                if (*output == NULL) {
                    perror("realloc");
                    exit(EXIT_FAILURE);
                }
            }
            memcpy(*output + output_length, buffer, count);
            output_length += count;
        }

        if (count < 0) {
            perror("read");
            free(*output);
            exit(EXIT_FAILURE);
        }

        (*output)[output_length] = '\0';

        close(stdout_pipe[0]); // Close the read end of stdout pipe
        wait(NULL);            // Wait for the child process to terminate
    }

    return 0;
}

int handle_exec(int token_type, char* token, char* statement) {
    int argc = 0;
    char** argv;
    int _end_token_type;

    char* output_descriptor = NULL;
    char* input_descriptor = NULL;

    if ((_end_token_type = forward(token_type, statement, token, &argc, &argv)) != 0) {
        if (_end_token_type == TOKEN_STDOUT_REDIRECT) {

            token_type = get_next_token(current_position, token);
            if (token_type == TOKEN_PARAMETER) {
                output_descriptor = strdup(token);
                printf("Output descriptor: %s\n", output_descriptor);
            } 
            else {
                fprintf(stderr, "Expected filename after stdout redirect\n");
                return 1;
            }
        }
        else if (_end_token_type == TOKEN_STDIN_REDIRECT) {
            token_type = get_next_token(current_position, token);
            if (token_type == TOKEN_PARAMETER) {
                input_descriptor = strdup(token);
                printf("Input descriptor: %s\n", input_descriptor);
            } 
            else {
                fprintf(stderr, "Expected filename after stdin redirect\n");
                return 1;
            }
        }
    }

    char* output = NULL;

    int status = exec_child_proc(argc, argv, input_descriptor, &output);

    if (output_descriptor != NULL) {
        int fd = open(output_descriptor, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            perror("open");
            return 1;
        }

        if (write(fd, output, strlen(output)) == -1) {
            perror("write");
            return 1;
        }

        close(fd);
    }
    else {
        printf("%s", output);
    }

    if (status != 0) {
        fprintf(stderr, "Command failed with status %d\n", status);
        return 1;
    }

    return 0;
}