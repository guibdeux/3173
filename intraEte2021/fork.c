//
// Created by guib on 2021-06-28.
//
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "stdbool.h"
typedef char* String;

pid_t fork2(void) {
    pid_t pid = fork();
    pid_t pid_2;

    switch (pid) {
        case -1: // error code
            perror("fork error: ");
            return EXIT_FAILURE;
        case 0: // first child code
            pid_2 = fork();
            switch (pid_2) {
                case -1: // first child error code
                    perror(" child fork error: ");
                    return EXIT_FAILURE;
                case 0: // second child code
                    return 0;
                default: // first child code
                    return pid_2;
            }
        default: // parent code
            return pid;
    }
}

int main (void) {
    printf("getpid() est %d\n", getpid());
    pid_t pid = fork2();
    printf("getpid() est %d\n", getpid());
    printf("mon pid est %d\n", pid);
    return 0;
}
