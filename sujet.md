---
title: Devoir final INF3173
author: Jean Privat
date: du 16 au 17 aout 2021
---

* Chaque devoir est **individuel**.
* Le sujet est en Markdown, répondez directement dans le fichier. Codage UTF-8, fins de lignes Unix.
* Répondez sur les lignes avec des chevrons (`>`), répondez après les chevrons ; vous pouvez en modifier le nombre, mais laissez les chevrons en place pour que je voie vos réponses.
* **Ne touchez pas** au reste du sujet. Un `diff` entre votre copie et le sujet ne **doit** faire apparaitre que vos réponses.
* Les réponses devront être aussi **détaillées** et **argumentées** que possible (en respectant la taille de la réponse attendue).
* Vous avez jusqu'à 23h55 du dernier jour pour remettre votre fichier Markdown via Moodle. Vous pouvez faire autant de remises que vous le souhaitez, seule la dernière sera considérée.
* Merci de **ne pas discuter** du contenu du sujet sur Mattermost (ou via tout autre médium).
* Si vous avez des questions, vous pouvez toujours tenter de me les poser en message privé sur Mattermost, mais je me réserve le droit de **ne pas répondre** (ou de répondre après la date de remise).
* Tout non-respect du format ou des consignes sera pénalisé.
* La qualité du français ainsi que la rigueur, la précision et la justesse dans les explications et dans le vocabulaire du cours utilisé seront prises en compte.


# Identification des étudiants

* Nom: Blouin
* Prénom: Guillaume
* Code permanent: BLOG09079006

Note: Vérifiez que votre code permanent est correct.


# Quiz

La première partie du devoir est sous forme d'un quiz indépendant.
Quand le quiz est commencé, il doit être terminé en moins de 30 minutes.

* <https://www.moodle2.uqam.ca/coursv3/mod/quiz/view.php?id=2407308>

# Programmation

Le méchant professeur donne un exercice aux étudiants:

Écrivez un programme C qui affiche chaque ligne de l'entrée standard à l'envers (sans toucher à l'ordre des lignes) et affiche un '*' tous les 10 octets affichés.

Ainsi, pour le fichier `exemple.txt` suivant

```
Hello, World!
Bonjour
le
monde!
```

on veut obtenir la sortie suivante.

```
!dlroW ,ol*leH
ruojno*B
el
!edno*m
```


Un étudiant distrait propose le programme `p.c`:

```
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
        } 

        close(p[1]);
        dup2(p[0], 0);
        char buf[10] = "";
        while(read(0, buf, 10) > 0) { 
                write(1, buf, 10);
                write(1, "*", 1);
        } 
}
```

## Première expérience

L'étudiant teste sur sa machine, la sortie est correcte jusqu'à un certain point puis le programme bloque et l'étudiant doit faire Ctrl-C.

```
$ ./p < exemple.txt 
!dlroW ,ol*leH
ruojno*B
el
!edno*m
el
!edno*
```

Note: il est possible que sur votre machine vous n'observiez pas ce comportement, cf. seconde expérience.

### Q1

Quels sont les processus créés ou manipulés par ce programme?
Détaillez et justifiez votre réponse en indiquant, entre autres, pour chaque processus où dans le programme celui-ci est créé et quel est son processus parent.

> `pid_t f = fork();` c'est un appel système, crée un processus clône indépendant; \
>   --> appelons ce dernier: **"enfant1"**; \
>   --> le **parent** est le programme **"p"** \
> `system("rev");` c'est une fonction bibliothèque, exécute une commande et gère un processus clône indépendant grâce à `fork(2)`; \
>   --> appelons ce dernier: **"enfant2"**; \
>   --> le **parent** est **"enfant1"** \
> `system("rev");` exécute une commande grâce à `execl(2)` (exécutant le programme `rev(1)`); \
>   -->**"enfant2" devient** l'exécutable `"/usr/bin/rev"` et **conserve "enfant1"** comme **parent** \
> Finalement, les **processus créés ou manipulés** sont: \
>   --> `"enfant1"`; \
>   --> `"enfant2"`;  \
>   -->`"/usr/bin/rev"`

### Q2

Dans l'expérience, expliquez pourquoi le programme affiche deux fois « `el !edno*` » ?
Justifiez votre réponse.

> ```c
> 1  [master] guib [~/3173] 
> 2  $ strace -f ./p < exemple.txt 2>&1 | grep "read(0\|write" 
> 3  [pid  8191] read(0,  <unfinished ...> 
> 4  [pid  8194] read(0, "Hello, World!\nBonjour\nle\nmonde!", 4096) = 31 
> 5  [pid  8194] read(0, "", 4096)           = 0 
> 6  [pid  8194] write(1, "!dlroW ,olleH\nru", 16) = 16 
> 7  [pid  8194] write(1, "ojnoB\nel\n!ednom", 15 <unfinished ...> 
> 8  [pid  8191] write(1, "!dlroW ,ol", 10!dlroW ,ol <unfinished ...> 
> 9  [pid  8194] <... write resumed>)        = 15 
> 10 [pid  8191] <... write resumed>)        = 10 
> 11 [pid  8191] write(1, "*", 1* <unfinished ...> 
> 12 [pid  8191] <... write resumed>)        = 1 
> 13 [pid  8191] read(0,  <unfinished ...> 
> 14 [pid  8191] write(1, "leH\nruojno", 10leH 
> 15 [pid  8191] <... write resumed>)        = 10 
> 16 [pid  8191] write(1, "*", 1 <unfinished ...> 
> 17 [pid  8191] <... write resumed>)        = 1 
> 18 [pid  8191] read(0,  <unfinished ...> 
> 19 [pid  8191] write(1, "B\nel\n!edno", 10B 
> 20 [pid  8191] write(1, "*", 1*)            = 1 
> 21 [pid  8191] read(0, "m", 10)            = 1 
> 22 [pid  8191] write(1, "m\nel\n!edno", 10m 
> 23 [pid  8191] write(1, "*", 1*)            = 1 
> 24 [pid  8191] read(0,  <unfinished ...> 
> 25 ^C 
> ```
> -------------------------------------------------------------------------------------------------------------------
> `Ligne 21, [pid 8191]`, **le parent "p"**, fait sa **dernière lecture**, il ne restait que le byte `'m'` à lire. \
> `Ligne 22, [pid 8191]`, **le parent "p"**, **écrit buf au complet** pour le **dernier tour** sur la sortie standard, en commençant par le byte `'m'` dernièrement lu. \
> `NOTE`, le reste de `buf` n'a pas été écrasé ni géré avec un caractère null. \
> `NOTE`, les bytes restants dans `buf` s'écrivent aussi: `'\nel\n!edno'` \
> `Ligne 23, [pid 8191]`, **le parent "p"**, écrit le byte `'*'` sur la sortie standard.

### Q3

Dans l'expérience, indiquez pour chacun des descripteurs de fichiers de chacun des processus: à quoi correspond-il (quels fichiers ou tubes ou autre), quand est-il créé (ou rendu disponible au processus) et quand est-il fermé. Précisez également où ces évènements ont lieu dans le programme.

> ```c
> 1  int main() { 
> 2  int p[2]; 
> 3                                                 // "p": 0 -> stdin, 1 -> stdout, 2 -> stderr 
> 4  pipe(p); 
> 5                                                 // "p": 0 -> stdin, 1 -> stdout, 2 -> stderr, 3 -> Pipe(READ_ONLY), 4 -> Pipe(WRITE_ONLY) 
> 6         pid_t f = fork(); 
> 7                                                 // "enfant1": 0 -> stdin, 1 -> stdout, 2 -> stderr, 3 -> Pipe(READ_ONLY), 4 -> Pipe(WRITE_ONLY) 
> 8         if (f == 0) { 
> 9                 dup2(p[1], 1); 
> 10                                                // "enfant1": 0 -> stdin, 1 -> Pipe(WRITE_ONLY), 2 -> stderr, 3 -> Pipe(READ_ONLY), 4 -> Pipe(WRITE_ONLY) 
> 11                system("rev");       
> 12                                                // "enfant2": 0 -> stdin, 1 -> Pipe(WRITE_ONLY), 2 -> stderr, 3 -> Pipe(READ_ONLY), 4 -> Pipe(WRITE_ONLY) 
> 13                                                // "/usr/bin/rev": 0 -> stdin, 1 -> Pipe(WRITE_ONLY), 2 -> stderr, 3 -> Pipe(READ_ONLY), 4 -> Pipe(WRITE_ONLY) // ("enfant2" a terminé)
> 14        }
> 15        close(p[1]); 
> 16                                                // "p": 0 -> stdin, 1 -> stdout, 2 -> stderr, 3 -> Pipe(READ_ONLY) // ("/usr/bin/rev" a terminé)
> 17                                                // "enfant1": 0 -> stdin, 1 -> Pipe(WRITE_ONLY), 2 -> stderr, 3 -> Pipe(READ_ONLY) 
> 18        dup2(p[0], 0); 
> 19                                                // "p": 0 -> Pipe(READ_ONLY), 1 -> stdout, 2 -> stderr, 3 -> Pipe(READ_ONLY) 
> 20                                                // "enfant1": 0 -> Pipe(READ_ONLY), 1 -> Pipe(WRITE_ONLY), 2 -> stderr, 3 -> Pipe(READ_ONLY) 
> 21        char buf[10] = ""; 
> 22        while(read(0, buf, 10) > 0) { 
> 23                write(1, buf, 10); 
> 24                write(1, "*", 1); 
> 25        } 
> 26 }
> ```



### Q4

Dans l'expérience, indiquez dans quel état (actif, prêt, bloqué, zombi, etc.) est chacun des processus.
Détaillez et justifiez votre réponse en indiquant, entre autres, ce que chaque processus est en train de faire (ou d'attendre) et à quel endroit du programme il est.

> ```c
> 1  int main() {
> 2                                        // "p": prêt
> 3  int p[2];
> 4                                        // "p": actif, création du pipe
> 5  pipe(p);
> 6         pid_t f = fork();
> 7                                        // "enfant1": prêt
> 8         if (f == 0) {
> 9                                        // "enfant1": actif, vérifie son PID, placera (pipe WRITE_ONLY) sur STDOUT, exécutera system()
> 10                dup2(p[1], 1);
> 11                system("rev");
> 12                                       // "enfant2": prêt
> 13                                       // "enfant2": actif, exécute execl() et devient "/usr/bin/rev": prêt, (man rev.1)
> 14                                       // "/usr/bin/rev": actif, lis STDIN et écris chaque mots à l'envers dans STDOUT (pipe WRITE_ONLY) et quitte
> 15        }
> 16        close(p[1]);
> 17                                       // "p": actif, ferme (pipe WRITE_ONLY)
> 18                                       // "enfant1": actif, ferme (pipe WRITE_ONLY (original))
> 19        dup2(p[0], 0);
> 20                                       // "p": actif, place (pipe READ_ONLY) sur STDIN
> 21                                       // "enfant1": actif, idem
> 22        char buf[10] = ""; 
> 23                                       // "p": actif, initialise buf avec une taille de 10 chars et y injecte le caractère nul en premier indice
> 24                                       // "enfant1": actif, idem
> 25        while(read(0, buf, 10) > 0) { 
> 26                                       // "p": actif, lit le descripteur 0 contenant pipe_READ_ONLY pour obtenir maximum 10 octets et le placer dans buf; consomme le data du pipe avec read(2)
> 27                                       // "enfant1": bloqué, le pipe (ressource) est occupé en lecture (atomicité)
> 28                write(1, buf, 10);
> 29                                       // "p": actif, écrit `buf` au complet sur STDIN
> 30                write(1, "*", 1);
> 31                                       // "p": actif, écrit "*" sur STDIN
> 32        } 
> 33                                       // "p": actif, fait la lecture complète reçue de "/usr/bin/rev" et procède à l'écriture sur STDIN
> 34                                       // "p": bloqué, tente une relecture sur le descripteur 0 pipe_READ_ONLY (VIDE)
> 35                                       // "enfant1": prêt
> 36                                       // "enfant1": actif, obtient la ressource pipe_READ_ONLY et repasse à Q4, ligne 25
> 37                                       // "enfant1": bloqué, tente une relecture sur le descripteur 0 pipe_READ_ONLY (VIDE); il en est aussi toujours l'écrivain.
> 38 }
> 39 
> 40                                       // Note: plusieurs processeurs sont disponibles pour le parrallélisme.
> ```

### Q5

Peut-on caractériser (ou pas) la situation comme un interblocage, un « live lock », une famine, etc. ?
Justifiez votre réponse.

> `Oui`, nous pouvons caractériser la situation comme un interblocage. En effet, un processus peut en bloquer un autre ou se bloquer lui-même. \
> Suivons les **4 conditions** nécessaires et suffisantes à un interblocage: \
> `1) Exclusion mutuelle, la ressource est soit disponible, soit assignée` \
> --> le pipe est soumis a l'atomicité, opérations entières sans pouvoir être interrompues, la ressource est pour un seul processus à la fois en lecture et un autre à la fois en écriture. \
> `2) Détention multiple (hold and wait), un processus qui détient une ressource peut en demander d’autres` \
> --> les processus peuvent détenir plusieurs pipes en lecture et en écriture \
> `3) Pas de réquisition, une ressource détenue par un processus doit être libérée par lui` \
> --> Un processus doit arrêter de lire ou écrire dans le pipe pour qu'un autre processus fasse la même opération. Ce dernier ne pourra pas interrompre le premier \
> --> les descripteurs pipe_WRITE_ONLY des écrivains sont à fermer pour libérer les lecteurs; ces descripteurs doivent être fermés par les écrivains eux-mêmes. \
> --> **Il ne doit plus rester un écrivain pour débloquer les lecteurs sur un pipe vide** \
> --> Un écrivain reçoit un signal SIGPIPE pour se terminer lorsque plus aucun lecteur existe. \
> `4) Attente circulaire, il doit y avoir un cycle dans les attentes d’événements` \
> --> les lecteurs attendent que les autres lecteurs libèrent le pipe; les écrivains attendent que les autres écrivains libèrent le pipe; \
> --> un lecteur attend en boucle l'écriture d'un écrivain; lecteur bloque si écrivain trop lent; écrivain bloque si lecteur trop lent et PIPESIZE atteint. 


## Seconde expérience

L'étudiant, peu satisfait du comportement de son programme attribue le problème à sa machine, son compilateur ainsi qu'à la `libc`, et décide de compiler et exécuter son programme sur une machine non boguée.

Il se connecte sur le serveur de son université, recompile son programme et retente l'expérience, mais cette fois c'est pire: le programme affiche plein d'étoiles avec quelques caractères de temps en temps (ce qui remplit son écran) et bloque à nouveau.

```
$ ./p < exemple.txt
****l**********************************************!**************************************W*** ***************************l***********e**************************u*********************n***********************************************e***********d*************n*o**********m**********************************************************************************************************************************************************
***********************o************************H*
***********************************e******************************************,****************r*****************************
*************************************************o**********o********
****!******************r******************************B****************d********o***********************************l*********************************************************l************************************************************************j*********j**********j**********j*************j*********j**********j***************j********j*j************j**************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************
```

### Q6

Expliquez ce qui s'est passé dans cette expérience. Indiquez, entre autres, pourquoi il y a plein d'étoiles, pourquoi toujours un blocage (est-ce le même blocage que dans l'expérience 1?).

> Le programme suit un **deuxième scénario possible**; **réinventons Q4, ligne 25**: 
>```c
> 25        while(read(0, buf, 10) > 0) { 
> 26                                       // "enfant1": actif, lit le descripteur 0 contenant pipe_READ_ONLY pour obtenir maximum 10 octets et le placer dans buf; consomme le data du pipe avec read(2)
> 27                                       // "p": bloqué, le pipe (ressource) est occupé en lecture (atomicité); 
> 28                write(1, buf, 10);
> 29                                       // "enfant1": actif, écrit `buf` au complet sur le descripteur 1 contenant pipe_WRITE_ONLY; c'est-à-dire, il écrit sur le pipe
> 30                write(1, "*", 1);
> 31                                       // "enfant1": actif, écrit "*" sur le pipe
> 32        } 
> 33                                       // "enfant1": actif, fait la lecture partielle en compétition avec "p", le data reçu de "/usr/bin/rev" ou de lui-même et procède à l'écriture sur le pipe en boucle
> 34                                       // "p": prêt; "p": actif, tente une relecture acceptée sur le descripteur 0 pipe_READ_ONLY en retournant à Q4, ligne 25
> 35                                       // "enfant1": bloqué, atomicité du pipe en lecture, "p" a réussit à obtenir une lecture sur le pipe;
> 36                                       // "enfant1": prêt, "p" termine une lecture; "enfant1": actif, tente une relecture acceptée sur le descripteur 0 pipe_READ_ONLY en retournant à Q6, ligne 25
> 37                                       // Operations commentées Q6, ligne 25 à 36 en boucle jusqu'à ce que "p" obtiennent suffisamment de tours de boucles dans la même séquence pour consommer le pipe et bloquer 
> 38                                       // Lorsque le pipe sera vide et consommé à cause de "p" qui consomme le pipe et écrit sur STDIN, "p" et "enfant1" passeront à l'état bloqué 
> 39                                       // IMPORTANT:
> 40                                       // --> "enfant1" lit le pipe et y écrit en boucle alors que "/usr/bin/rev" n'a pas terminé l'écriture en compétition avec "enfant1" et que "p" lit le pipe en compétition avec "enfant1"
> 41                                       // --> Ainsi, alors que "p" vide le pipe et l'envoie sur STDOUT, "enfant1" lit le peu de caractères restants reçus de "/usr/bin/rev" que "p" n'a pas consommé
> 42                                       // --> Le restant de caractères reçu de "/usr/bin/rev" est assez pour "enfant1" pour entrer dans la boucle, réécrire la lecture dans le pipe et y ajouter un étoile en boucle à répétition avant de perdre la prochaine lecture du pipe par "p"
> 43 }
> 44 
> 45                                       // Note: plusieurs processeurs sont disponibles pour le parrallélisme.
>```

### Q7

Quelle est la cause selon vous de la différence de comportement entre les deux machines?
Que pensez-vous de l'attitude de l'étudiant.
Justifiez votre réponse.

> La cause selon moi de la différence de comportement entre les deux machines est: \
> --> une possibilité d'accès aléatoire de "enfant1" en écriture sur le pipe selon les différentes architectures et systèmes d'exploitation, la disponibilité et l'ordonnancement des ressources. \
> --> En effet, si **"enfant1"** **exécute Q6, ligne 25**, avant que "p" puisse consommer le pipe en entier, les données envoyés sur le pipe seront imprévisibles et remplies de "*"; elles seront partiellement lisibles (à l'envers) ou complètement illisibles sur STDOUT lorsque "p" écrira à nouveau
> 
> Par exemple, sur ma machine linux (ubuntu 20.04):
> ```c
> 1 [master] guib [~/3173]
> 2 $ ./p < exemple.txt
> 3 !dlroW ,ol*leH
> 4 ruojno*B
> 5 el
> 6 !edno**
> 7 el
> 8 !edno***m*********************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************^C
> ```
> Nous voyons une **série de "\*"** à la fin de l'affichage alors que **"enfant1" a prit la main avant la consommation complète du pipe**
>
> L'attitude de l'étudiant était trop spontanée; il devrait vérifier son code avant de mettre le problème sur le dos de quelque chose de beaucoup plus compliqué que ses compétences.

## Correction

### Q8

En vous adressant à l'étudiant distrait, indiquez et expliquez les nombreux problèmes dans son code ainsi que ce qui aurait dû être fait.

> NOTE: Les **erreurs répétés** ne sont **repérées** qu'**une seule fois** pour alléger la correction
> ```c
> 1  int main() {                              // (1) vÉcrire des commentaires pour aider les lecteurs, faire des variables significatives et des fonctions.
> 2  int p[2];
> 3  pipe(p);                                  // (2) Vérifier les erreurs des appels systèmes 
> 4  
> 5          pid_t f = fork();
> 6          if (f == 0) { 
> 7                  dup2(p[1], 1);            // (3) Fermer OLD_FD après l'appel à dup2()
> 8                                            // (4) Fermer le pipe en lecture
> 9                  system("rev");
> 10                                           // (5) Terminer le processus "enfant1" 
> 11         } 
> 12 
> 13         close(p[1]);
> 14         dup2(p[0], 0);                    // (6) Fermer OLD_FD après l'appel à dup2(), vérifier l'erreur
> 15         char buf[10] = "";                // (7) Allouer dynamiquement buf et lui donner la place pour deux caractères
> 16                                           // (8) Il ne faut pas remplir buf lors de la lecture, il faut laisser une place pour le caractère nul, à l'indice décidé par le retour de read(2)
> 17         while(read(0, buf, 10) > 0) {     // (9) Lire le pipe caractère par caractère
> 18                 write(1, buf, 10);        // (10) S'assurer de ne plus avoir d'écrivains sur le pipe 
> 19                 write(1, "*", 1);         // (11) Écrire l'étoile seulement si 10 caractères sont écris sur STDOUT
> 20         }                                 // (12) Utiliser le retour de la fonction read pour vérifier la condition et changer l'opération pour: "!= 0", cela permettra de vérifier l'erreur lorsque le retour de read(2) == -1
> 21 }                                         // (13) Ne pas oublier le retour de la fonction main "return 0"
> 22                                           // (14) Ne pas oublier d'attendre l'enfant avec wait(2), sinon nous aurons un zombie jusqu'à la terminaison de "p"
> ```

### Q9

En appliquant vos indications de la question précédente, proposez une version corrigée et fonctionnelle du programme `p.c` (contrainte: vous devez garder l'utilisation de tubes et l'appel à la commande `rev`).

> ```c
> 1  #include <stdlib.h>
> 2  #include <stdio.h>
> 3  #include <unistd.h>
> 4  #include <sys/types.h>
> 5  #include <sys/wait.h>
> 6  #include <string.h>
> 7  
> 8  void reverse_Write_With_Stars () {
> 9      char etoile[2] = "*";
> 10     char chariot[2] = "\n";
> 11     char* buf = NULL;
> 12     ssize_t nbytes = 1;
> 13     ssize_t total_nbytes = 0;
> 14 
> 15     if ((buf = malloc(sizeof(char) * 2)) == NULL) {
> 16         perror("\nmalloc");
> 17         _exit(1);
> 18     } // allocating dynamically buf
> 19     while(nbytes == 1) { // going to read the pipe char by char
> 20         nbytes = read(0, buf, 1);
> 21         if  (nbytes == -1 ) {
> 22             perror("\nread");
> 23             _exit(1);
> 24         } else {
> 25             total_nbytes += nbytes;
> 26             buf[nbytes] = 0x00; // place '\0' dans buf, au bon indice
> 27             if ((write(1, buf, strlen(buf))) == -1) {
> 28                 perror("write buf");
> 29                 _exit(1);
> 30             } // STDOUT
> 31             // Write a "*" every 10 bytes
> 32             if (total_nbytes % 10 == 0)
> 33                 if ((write(1, etoile, strlen(etoile))) == -1) {
> 34                     perror("write chariot");
> 35                     _exit(1);
> 36                 }
> 37         }
> 38     }
> 39     // Rendre le code propre ajoutant un retour chariot
> 40     if ((write(1, chariot, strlen(chariot))) == -1) {
> 41         perror("write chariot");
> 42         _exit(1);
> 43     }
> 44     if (close(0) == -1) { // Close READ_ONLY (pipe)
> 45         perror("\nparent close descriptor \"0\" containing pipe");
> 46         _exit(1);
> 47     }
> 48     free(buf);
> 49 }
> 50 
> 51 int main() {
> 52     // Creating a pipe
> 53     int p[2];
> 54     if (pipe(p) == -1) {
> 55         perror("\npipe");
> 56         _exit(1);
> 57     }
> 58 
> 59     // creating a process
> 60     switch (fork()) {
> 61     case -1:
> 62         perror("\nfork");
> 63         _exit(1);
> 64     case 0: // child code
> 65         if (close(p[0]) == -1) { // Close READ_ONLY (pipe)
> 66             perror("\nchild close p[0]");
> 67             _exit(1);
> 68         }
> 69         if ((dup2(p[1], 1)) == -1) { // Copy WRITE_ONLY (pipe)
> 70            perror("\ndup2(p[1], 1)");
> 71            _exit(1);
> 72         }
> 73         if (close(p[1]) == -1) {
> 74            perror("\nchild close p[1]");
> 75             _exit(1);
> 76         }
> 77 
> 78         if (system("rev") == -1) {
> 79             perror("\nsystem");
> 80             _exit(1);
> 81         }
> 82         _exit(0); // end of child
> 83     }
> 84     /***************
> 85      * parent code *
> 86      ***************/
> 87     if (close(p[1]) == -1) { // Close WRITE_ONLY (pipe)
> 88         perror("\nparent close p[1]");
> 89         _exit(1);
> 90     }
> 91     if ((dup2(p[0], 0)) == -1) { // Copy READ_ONLY (pipe)
> 92         perror("\ndup2(p[0], 0)");
> 93         _exit(1);
> 94     }
> 95     if (close(p[0]) == -1) {
> 96         perror("\nchild close p[1]");
> 97         _exit(1);
> 98     }
> 99 
>100     // main function
>101     reverse_Write_With_Stars();
>102     // waiting child
>103     if (wait(NULL) == -1) {
>104         perror("\nwaiting child");
>105         _exit(1);
>106     }
>107     return 0;
>108 }
> ```

## Extra

### Q10

Quelle est votre capsule préférée du cours INF3173. Expliquez pourquoi.

> Normalement je dirais que ma capsule préférée c'est la **410**, celle sur les **signaux** car grâce à cette capsule, j'ai eu une **meilleure compréhension du TP1**. \
> Mais, ma capsule préférée c'est **incontestablement la capsule 430 sur les sockets**. En effet, bien manipuler les sockets semblent donner accès à un domaine ou les applications sont encore plus grandes et merveilleuses.
>
> Merci.
