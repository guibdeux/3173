#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void reverse_Write_With_Stars () {
    char etoile[2] = "*";
    char chariot[2] = "\n";
    char* buf = NULL;
    ssize_t nbytes = 1;
    ssize_t total_nbytes = 0;

    if ((buf = malloc(sizeof(char) * 2)) == NULL) {
        perror("\nmalloc");
        _exit(1);
    } // allocating dynamically buf
    while(nbytes == 1) { // going to read the pipe char by char
        nbytes = read(0, buf, 1);
        if  (nbytes == -1 ) {
            perror("\nread");
            _exit(1);
        } else {
            total_nbytes += nbytes;
            buf[nbytes] = 0x00;
            write(1, buf, strlen(buf));
            // Write a "*" every 10 bytes
            if (total_nbytes % 10 == 0) write(1, etoile, strlen(etoile));
        }
    }
    // Rendre le code propre ajoutant un retour chariot
    write(1, chariot, strlen(chariot));
    if (close(0) == -1) { // Close READ_ONLY (pipe)
        perror("\nparent close descriptor \"0\" containing pipe");
        _exit(1);
    }
    free(buf);
}

int main() {
    // Creating a pipe
    int p[2];
    if (pipe(p) == -1) {
        perror("\npipe");
        _exit(1);
    }

    // creating a process
    switch (fork()) {
    case -1:
        perror("\nfork");
        _exit(1);
    case 0: // child code
        if (close(p[0]) == -1) { // Close READ_ONLY (pipe)
            perror("\nchild close p[0]");
            _exit(1);
        }
        if ((dup2(p[1], 1)) == -1) { // Copy WRITE_ONLY (pipe)
            perror("\ndup2(p[1], 1)");
            _exit(1);
        }
        if (close(p[1]) == -1) {
            perror("\nchild close p[1]");
            _exit(1);
        }

        if (system("rev") == -1) {
            perror("\nsystem");
            _exit(1);
        }
        _exit(0); // end of child
    }
    /***************
     * parent code *
     ***************/
    if (close(p[1]) == -1) { // Close WRITE_ONLY (pipe)
        perror("\nparent close p[1]");
        _exit(1);
    }
    if ((dup2(p[0], 0)) == -1) { // Copy READ_ONLY (pipe)
        perror("\ndup2(p[0], 0)");
        _exit(1);
    }
    if (close(p[0]) == -1) {
        perror("\nchild close p[1]");
        _exit(1);
    }

    // main function
    reverse_Write_With_Stars();
    // waiting child
    if (wait(NULL) == -1) {
        perror("\nwaiting child");
        _exit(1);
    }
    return 0;
}