#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>


int main(void) {
    pid_t pidPere ;
    pidPere = getpid() ;
    int wstatus ;
    printf("digraph { \nrankdir = BT\n");
    fflush(stdout);

    if (fork()*fork())
    	fork();
    else
    	fork() || fork();

 	if(pidPere != getpid()){
    	printf("%d -> %d\n",getppid(),getpid());
    }

	while(waitpid(-1,&wstatus,0) != -1);

    if(pidPere == getpid()){
    	printf("}");
    }
    return 0;
}

