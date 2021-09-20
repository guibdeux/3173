#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<signal.h>

/**************************************************************************************************************
 * La valeur du backlog de listen(2) mise à 0
 * En mettant backlog à 0, au maximum 2 clients arrivent à se connecter: un client en cours de traitement
 * et l’autre attend dans la file d’attente du serveur, les autres clients vont essayer de se connecter
 * (le code(côté client) bloque à l’appel système connect) jusqu’à ce qu’une place se libère. NB: lorsqu’un
 * client est accepté il ne compte plus dans le backlog. En écrivant une ligne dans le client n°2, le serveur
 * ne répond pas car il est en train de traiter avec le client n°1, lorsqu’on termine le client n°1 avec Ctrl-d,
 * le serveur commence à servir le client n°2 et le client n°3 se connecte au serveur. La valeur du backlog de
 * listen(2) mise à SOMAXCONN. En mettant backlog au max, plusieurs clients arrivent à se connecter et attendent
 * que le client 1 soit servi.
 * **************************************************************************************************************/

void gere(int sig) {
    fprintf(stderr, "Connexion perdue\n");
    exit(1);
}

int main(int argc, char **argv)
{
    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = gere;
    sigaction(SIGPIPE, &action, NULL);

    int sock = socket(AF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "sock", sizeof(addr.sun_path)-1);

    fprintf(stderr, "**Connexion en cours**\n");
    int res = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    if(res==-1) {
        perror("connect");
        exit(1);
    }
    fprintf(stderr, "**Connecté**\n");
    int resWrite, resRead;
    char buf[100];
    while(fgets(buf, sizeof(buf)-1, stdin)!=NULL){
        fprintf(stderr, "**Envoi en cours**\n");
        resWrite=write(sock, buf, strlen(buf));
        if(resWrite==-1){
            perror("write cli");
            exit(1);
        }
        fprintf(stderr, "**Message envoyé**\n");
        fprintf(stderr, "**En attente du serveur...**\n");
        resRead = read(sock, buf, sizeof(buf)-1 );
        if(resRead == -1){
            perror("read cli");
            return 1;
        }
        buf[resRead]='\0';
        printf("Serveur: %s",buf);
    }

    close(sock);
    fprintf(stderr, "**Connexion teminée**\n");
    return 0;
}