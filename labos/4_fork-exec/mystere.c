#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(void) {
    if (fork()*fork())
    	fork();
    else
    	fork() || fork();

    printf("Mon PID est %d et mon père est %d ! \n",getpid(),getppid());
    int wstatus;
	// Tant que waitpid retourne le PID d'un des porcessus enfants qui termine, continuer la boucle
    while(waitpid(-1,&wstatus,0) != -1);
    return 0;
}

