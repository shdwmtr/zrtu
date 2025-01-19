cmake_minimum_required(VERSION 3.10)

project(zrtu)

include_directories(include)

add_executable(shell 
    src/main.c 
    src/user.c 
    src/builtins.c
    src/cd.c
    src/pwd.c
    src/tokenize.c
    src/exec.c
    src/forward.c
    src/history.c
    src/prompt.c
)