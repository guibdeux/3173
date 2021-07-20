# Descripteur de fichier
### Le programme permet de créer un fichier resultat qui contient Programme Programme mystère. En effet, on a deux descripteurs qui pointent sur la même entrée dans la table des fichiers ouverts (TFO) (qui dans notre cas est le fichier resultat). Le premier descripteur est utilisé pour écrire Programme mystère\n, le deuxième est utilisé pour reculer le curseur de 9 donc le curseur est positionné après Programme , et enfin le premier descripteur est réutilisé pour réecrire Programme mystère\n.

#### Le programme redir.c
```c
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
```
````
./redir /dev/null /dev/tty ls -l /proc/self/fd/ affiche le contenu du fichier /proc/self/fd qui représente la table des descripteurs avec les fichiers sur lesquels ils pointent: 0 -> /dev/null 1 -> /dev/tty 2 -> /dev/tty 3 -> /proc/12375/fd
````