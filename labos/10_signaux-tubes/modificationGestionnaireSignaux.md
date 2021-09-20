Remarque: n’oubliez pas, lors de l’utilisation d’appels systèmes, de traiter les cas d’erreur.

Modification du gestionnaire d’un signal
On veut gérer le signal d’interruption de processus (signal SIGINT) de plusieurs facons:
Gestion directe du signal avec un affichage:
```c
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<signal.h>

void gere(int sig) {
printf("Réception du signal SIGINT\n");
exit(1);    
}

int main(void) {
struct sigaction action;
sigemptyset(&action.sa_mask);
action.sa_flags = 0;
action.sa_handler = gere;
sigaction(SIGINT, &action, NULL);
pause();
return 0 ;
}
En ignorant l’occurence du signal:
#include<signal.h>
#include<unistd.h>

int main(void) {
struct sigaction action;
sigemptyset(&action.sa_mask);
action.sa_flags = 0;
action.sa_handler = SIG_IGN;
sigaction(SIGINT, &action, NULL);
pause();

return 0 ;
}
```
En traitant une fois le signal, puis de revenir au traitement du signal par défaut:
```c
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<signal.h>

void gere(int sig) {
printf("Réception du signal SIGINT\n");
}

int main(void) {
struct sigaction action;
sigemptyset(&action.sa_mask);
action.sa_flags = 0;
action.sa_handler = gere;
sigaction(SIGINT, &action, NULL);
pause();
action.sa_handler = SIG_DFL;
sigaction(SIGINT, &action, NULL);
pause();
return 0 ;
}
```
```c
En traitant deux fois le signal, puis de revenir au traitement du signal par défaut:
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<signal.h>

void gere(int sig) {
printf("Réception du signal SIGINT\n");
}

int main(void) {
struct sigaction action;
sigemptyset(&action.sa_mask);
action.sa_flags = 0;
action.sa_handler = gere;
sigaction(SIGINT, &action, NULL);
pause();
pause();
action.sa_handler = SIG_DFL;
sigaction(SIGINT, &action, NULL);
pause();
return 0 ;
}
```
On peut vérifier qu’un signal n’est pas receptionné plusieurs fois en retardant la fonction gerante assez, afin de refaire un ctrl+c au clavier, et on constatera que le signal n’est pas receptionné autant de fois qu’il est envoyé.
Ici nous allons capturer le signal SIGUSR1, et afficher 5 fois un message lorsqu’on recoit ce signal:
```c
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<signal.h>

void gere(int sig) {
for(int i=0; i<5; i++){
printf("Itération %d pour la gestion du signal %d\n", i+1, sig);
}
exit(1);
}

int main(void) {
struct sigaction action;
sigemptyset(&action.sa_mask);
action.sa_flags = 0;
action.sa_handler = gere;
sigaction(SIGUSR1, &action, NULL);
pause();
return 0 ;
}
```
Dans le cas où le programme p0 reçoit le signal SIGINT durant le traitement d’un signal SIGUSR1, il y aura une interuption du traitement au sein de la fonction gérante.
Afin d’éviter ça il suffit d’ignorer le signal lors du traitement:
```c
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<signal.h>

void gere(int sig) {
for(int i=0; i<5; i++){
printf("Itération %d pour la gestion du signal %d\n", i+1, sig);
if(i==2)
sleep(5);//afin d'avoir le temps de placer un ctrl+c
}
exit(1);
}

int main(void) {
struct sigaction action;
sigemptyset(&action.sa_mask);
sigaddset(&action.sa_mask,SIGINT);//pour ignorer SIGINT durant l'execution de la gérante
action.sa_flags = 0;
action.sa_handler = gere;
sigaction(SIGUSR1, &action, NULL);
pause();
return 0 ;
}
```
Tubes shell
Le programme shellpipe:
```c
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char ** argv) {
/*on fera confiance aux arguments entrés par l'utilisateur(pour cette fois-ci)*/
int indice_debut = 0;
for (int i = 1; i != argc; i++) {
if (strcmp(argv[i], "+") == 0) {
argv[i] = NULL;
indice_debut = i + 1;
break;
}
}
int p[2];
pipe(p);
pid_t commande1 = fork();
if (commande1 == 0) {
dup2(p[1], 1);
close(p[0]);
close(p[1]);
execvp(argv[1], argv + 1);
perror(argv[1]);
return 1;
}
pid_t commande2 = fork();
if (commande2 == 0) {
dup2(p[0], 0);
close(p[0]);
close(p[1]);
execvp(argv[indice_debut], argv + indice_debut);
perror(argv[indice_debut]);
return 1;
}
close(p[0]);
close(p[1]);
int wstatus;
waitpid(commande1, NULL, 0);
waitpid(commande2, &wstatus, 0);
if (WIFEXITED(wstatus)) {
return WEXITSTATUS(wstatus);
} else {
return 1;
}
return 0;
}
```
Redirection de processus
Le programme shellfifo:
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char * argv[]) {
int i, indice2;
char * nom_tube = "fifo";
for (i = 1; i < argc; i++) {
if (strcmp(argv[i], "{}") == 0) {
argv[i] = nom_tube;
}
if (strcmp(argv[i], "<|") == 0) {
argv[i] = NULL;
indice2 = i + 1;
}
}

int descripteur;
pid_t commande1, commande2;
if (mkfifo(nom_tube, 0666) == -1) {
perror("mkfifo");
return 1;
}

commande1 = fork();
if (commande1 == 0) {
descripteur = open(nom_tube, O_WRONLY);
if (descripteur == -1) {
perror("open fichier fifo");
return 1;
}
dup2(descripteur, 1);
close(descripteur);
execvp(argv[indice2], argv + indice2);
perror(argv[indice2]);
return 1;
}

commande2 = fork();
if (commande2 == 0) {
execvp(argv[1], argv + 1);
perror(argv[1]);
return 1;
}

waitpid(commande1, NULL, 0);
waitpid(commande2, NULL, 0);
unlink(nom_tube);
return 0;
}
```
Redirection Version 2
Le programme shellfifo version2:
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char * argv[]) {
int i, indice1, indice2;
char chemin_pseudo_fichier[20];
strcpy(chemin_pseudo_fichier, "/proc/self/fd/");

for (i = 1; i < argc; i++) {
if (strcmp(argv[i], "{}") == 0) {
argv[i] = chemin_pseudo_fichier;
indice1 = i;
}
if (strcmp(argv[i], "<|") == 0) {
argv[i] = NULL;
indice2 = i + 1;
}
}

int p[2];
pid_t commande1, commande2;
pipe(p);

commande1 = fork();
if (commande1 == 0) {
dup2(p[1], 1);
close(p[1]);
execvp(argv[indice2], argv + indice2);
perror(argv[indice2]);
return 1;
}

commande2 = fork();
if (commande2 == 0) {
sprintf(argv[indice1], "%s%d", argv[indice1], p[0]);
close(p[1]);
execvp(argv[1], argv + 1);
perror(argv[1]);
return 1;
}

close(p[0]);
close(p[1]);
waitpid(commande1, NULL, 0);
waitpid(commande2, NULL, 0);
return 0;
}
```