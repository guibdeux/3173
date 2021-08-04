//
// Created by guib on 2021-06-28.
//
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (execvp("readlink", argv) == -1) {
        printf("C'est pas un lien dur.\n");
    }
}
