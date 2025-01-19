#include <stdio.h>
#include <unistd.h>
#include <bits/local_lim.h>
#include <stdlib.h>
#include <string.h>

char* current_user_name;
char* current_host_name;
char* path_env;

int get_current_user() {
    char *username = getenv("USER");
    if (username) {
        current_user_name = username;
        return 0;
    } else {
        printf("Unable to determine the current user\n");
        return 1;
    }
}

int get_host_name() {
    char hostname[HOST_NAME_MAX + 1]; 

    if (gethostname(hostname, sizeof(hostname)) == 0) {
        current_host_name = strdup(hostname);
        if (!current_host_name) {
            fprintf(stderr, "Memory allocation failed for hostname\n");
            return 1;
        }
        return 0;
    } else {
        printf("failed to get hostname\n");
        perror("gethostname"); 
        return 1;
    }
}

int get_path_env() {
    char *path = getenv("PATH");
    if (path) {
        path_env = path;
        return 0;
    } else {
        printf("Unable to determine the PATH environment variable\n");
        return 1;
    }
}

void setup_shell() {
    get_current_user();
    get_host_name();
    get_path_env();
}