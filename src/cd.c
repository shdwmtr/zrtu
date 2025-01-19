#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

char* __current_directory;

int set_working_directory(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "cd: missing argument\n");
        return 1;
    }

    char *path = argv[1];

    if (path[0] == '~') {
        char *home = getenv("HOME");
        if (home == NULL) {
            fprintf(stderr, "cd: HOME not set\n");
            return 1;
        }

        char expanded_path[1024];
        snprintf(expanded_path, sizeof(expanded_path), "%s%s", home, path + 1);
        path = expanded_path;
    }

    if (chdir(path) != 0) {
        perror("cd");
        return 1;
    }

    __current_directory = path;
    return 0;
}