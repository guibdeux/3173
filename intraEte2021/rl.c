//
// Created by guib on 2021-06-28.
//

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
    struct stat st; ssize_t len;
    lstat(argv[1], &st);
    char* link = malloc(sizeof(char) * (st.st_size + 1));
    if ((len = readlink(argv[1], link, (st.st_size + 1))) != -1) {
        link[len] = '\0';
        printf("%s\n", link);
    } else {
        perror("ce n'est un lien mou");
        free(link); exit(1);
    }
    free(link); return 0;
}