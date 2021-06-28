//
// Created by guib on 2021-06-28.
//

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
    char link[10] = "";
    struct stat st;
    stat(argv[1], &st);
    if (st.st_mode | S_IFLNK) {
        readlink(argv[1], link, st.st_size);
        printf(link);
        printf("\n");
    } else {
        printf("C'est pas un lien dur.\n");
    }
}