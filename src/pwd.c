#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> 

char *get_formatted_working_directory() {
    char *cwd = getcwd(NULL, 0);
    if (!cwd) {
        perror("pwd");
        return NULL;
    }

    // Get the HOME directory path
    char *home_dir = getenv("HOME");

    // If the current directory is the home directory, return "~"
    if (home_dir && strcmp(cwd, home_dir) == 0) {
        free(cwd); // Don't forget to free memory
        return strdup("~");
    }

    // Find the last directory name in the path
    char *last_dir = strrchr(cwd, '/');
    if (last_dir) {
        last_dir++; // Skip the '/'
    } else {
        last_dir = cwd; // In case cwd is just a single directory (e.g., "/")
    }

    // Create the formatted directory string
    char *formatted_dir = strdup(last_dir);

    // Free the cwd memory since we no longer need it
    free(cwd);

    return formatted_dir;
}

int print_working_directory(int argc, char **argv) {
    if (argc != 1) {
        fprintf(stderr, "pwd: too many arguments\n");
        return 1;
    }

    char *cwd = getcwd(NULL, 0);
    if (!cwd) {
        perror("pwd");
        return 1;
    }

    printf("%s\n", cwd);
    free(cwd);
    return 0;
}