#include <user.h>
#include <pwd.h>
#include <stdlib.h>
#include <stdio.h>

void show_prompt() {
    char *cwd = get_formatted_working_directory();

    printf("\r\033[K%s@%s %s:~$ ", current_user_name, current_host_name, cwd);
    free(cwd);
}