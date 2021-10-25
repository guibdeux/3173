#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "stdbool.h"
typedef char* String;

void traitementExec (pid_t pid, bool** vOptionPtrOfPtr) {
    // Préparer le symlink
    char PATH[50]; // magic number semble ok
    sprintf(PATH, "/proc/%d/exe", pid);
    PATH[strlen(PATH)] = '\0';

    // Preparer la variable pour l'affichage de l'exécutable
    String maPathProcPidExe = NULL;
    ssize_t len;
    ssize_t lenMaPathProcPidExe = 2;
    if ((maPathProcPidExe = malloc(sizeof(char) * lenMaPathProcPidExe)) == NULL) {
        perror("malloc error:");
        exit(EXIT_FAILURE);
    }
    /************************************
     * Récupération de l'exécutable     *
     * Allocation de mémoire dynamique  *
     ************************************/
    while (true) { // breaks lorsque lenMaPathProcPidExe > len
        // Lecture du symlink et enregistrement du path de l'executable dans maPathProcPidExe
        len = readlink(PATH, maPathProcPidExe, sizeof(char) * lenMaPathProcPidExe);
        if (len == -1) {
            perror("readlink error: ");
            exit(127);
        } else if (len >= lenMaPathProcPidExe) {
            lenMaPathProcPidExe *=2;
            if ((maPathProcPidExe = realloc(maPathProcPidExe, sizeof(char) * lenMaPathProcPidExe)) == NULL) {
                perror("realloc error:");
                exit(EXIT_FAILURE);
            }
        } else { // lenMaPathProcPidExe > len
            break;
        }
    } // Fin allocation dynamique
    maPathProcPidExe[len] = '\0';
    // Option "-v", affichage du chemin du symlink sur le canal d'erreur
    if (**vOptionPtrOfPtr) fprintf(stderr, "%s\n", PATH);
    // Affichage du chemin de l'executable sur le canal d'erreur
    fprintf(stderr, "%s\n", maPathProcPidExe);
    free(maPathProcPidExe);
}

int traitementEnfants (const pid_t* pid, int* status, bool* vOptionPtr) {
    pid_t pidStopped;
    while (true) { // breaks lorsque (WIFEXITED(*status) || WIFSIGNALED(*status)) && pidStopped == *pid
        if ((pidStopped = waitpid(-1, status, 0)) != -1) {
            ptrace(PTRACE_SETOPTIONS, pidStopped, NULL,
                   PTRACE_O_EXITKILL
                   +PTRACE_O_TRACEEXEC
                   +PTRACE_O_TRACEFORK
                   +PTRACE_O_TRACECLONE);

            if (*status >> 8 == (SIGTRAP | (PTRACE_EVENT_EXEC << 8))) {
                traitementExec(pidStopped, &vOptionPtr);
            } else if (*status >> 8 == (SIGTRAP | (PTRACE_EVENT_FORK << 8))) {
                NULL;
            } else if (*status >> 8 == (SIGTRAP | (PTRACE_EVENT_CLONE << 8))) {
                NULL;
            }

            if (WIFEXITED(*status)) {
                if (pidStopped == *pid) {
                    return WEXITSTATUS(*status);
                }
            } else if (WIFSIGNALED(*status)) {
                if (pidStopped == *pid) {
                    return 128 + WTERMSIG(*status);
                }
            } else if (WIFSTOPPED(*status)) {
                if (WSTOPSIG(*status)) {
                    if (WSTOPSIG(*status) == SIGTRAP) {
                        if ((*status >> 8 == (SIGTRAP | (PTRACE_EVENT_EXEC << 8)))
                            || (*status >> 8 == (SIGTRAP | (PTRACE_EVENT_FORK << 8)))
                            || (*status >> 8 == (SIGTRAP | (PTRACE_EVENT_CLONE << 8))))
                            ptrace(PTRACE_CONT, pidStopped, NULL, 0);
                        else ptrace(PTRACE_CONT, pidStopped, NULL, SIGTRAP);
                    } else ptrace(PTRACE_CONT, pidStopped, NULL, WSTOPSIG(*status));
                } else ptrace(PTRACE_CONT, pidStopped, NULL, SIGCONT);
            }
        } else perror("waitpid error: "); // retour de waitpid() = -1
    }
}

int main(int argc, char *argv[]) {
    int retourTraitement;
    int status;
    int countArgc = 1;
    pid_t pid;
    bool vOption = false;

    if (argc > 1) {
        /**********************************************************
         * On veut savoir si on active l'option -v                *
         * L'option -v affiche le symlink en plus de l'executable *
         **********************************************************/
        if (strcmp(argv[countArgc], "-v") == 0) {
            if (argc == 2) {
                fprintf(stderr, "option -v sans arguments\n");
                exit(EXIT_FAILURE);
            } else {
                vOption = true; // l'option -v appartient a extra
                // Enlever le string "-v" des argv
                while ((countArgc + 1) < argc) {
                    argv[countArgc] = argv[countArgc + 1];
                    ++countArgc;
                }
                // Enlever la derniere position de argv
                argv[countArgc] = NULL;
                --argc;
            }
        } // Fin option -v

        pid = fork();
        switch (pid) {
            case -1: // error code
                perror("fork error: ");
                return EXIT_FAILURE;
            case 0: // child code
                ptrace(PTRACE_TRACEME, 0, NULL, NULL);
                raise(SIGSTOP);
                execvp(argv[1], argv + 1);
                perror("execvp error:");
                exit(127);
            default: // parent code
                retourTraitement = traitementEnfants (&pid, &status, &vOption);
        }
    } else {
        fprintf(stderr, "FAILURE\n");
        return EXIT_FAILURE;
    }
    return retourTraitement;
}
