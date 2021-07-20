#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include<fcntl.h>

void main(int argc, char **argv) {
    if(argc < 4){
        printf("Usage : ./redir IN OUT commande [argument...]\n");
        exit(EXIT_FAILURE);
    }
    int fd_in, fd_out, retour_dup_0;
    fd_in = open(argv[1], O_RDONLY);
    if(fd_in == -1){
        perror(argv[1]);
    }
    fd_out = creat(argv[2], 0666);
    if(fd_out == -1){
        perror(argv[2]);
    }
    if(dup2(fd_in, 0) == -1 || dup2(fd_out, 1) == -1 || dup2(fd_out, 2) == -1){
        perror("echec dup2");
    }
    close(fd_in);
    close(fd_out);
    execvp (argv[3], argv+3);
    perror("La commande ne peut pas être exécutée");
    exit(EXIT_FAILURE);
}