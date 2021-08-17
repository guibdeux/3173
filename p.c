#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int p[2];
    pipe(p);

    pid_t f = fork();
    if (f == 0) {
        dup2(p[1], 1);
        system("rev");
//        _exit(0);
    }

    close(p[1]);
    dup2(p[0], 0);
    char buf[10] = "";
    while(read(0, buf, 10) > 0) {
        write(1, buf, 10);
        write(1, "*", 1);
    }
    pause();
}