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
>   --> appelons ce dernier: **"enfant_1"**; \
>   --> le **parent** est le programme **"p"** \
> `system("rev");` c'est une fonction bibliothèque, exécute une commande et gère un processus clône indépendant grâce à `fork(2)`; \
>   --> appelons ce dernier: **"enfant_2"**; \
>   --> le **parent** est **"enfant_1"** \
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
> `Ligne 22, [pid 8191]`, **le parent "p"**, **écrit** pour la **dernière fois** sur la sortie standard, en commençant par le byte `'m'` dernièrement lu. \
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

> // "p":
> // "enfant1":
> // "enfant1":
> // "/usr/bin/rev":
> 
> ```c
> int main() {
>                                       // "p": prêt
> int p[2];
>                                       // "p": actif, création du pipe
> pipe(p);
>        pid_t f = fork();
>                                       // "enfant1": prêt
>        if (f == 0) {
>                                       // "enfant1": actif, vérifie son PID, placera (pipe WRITE_ONLY) sur STDOUT, exécutera system()
>                dup2(p[1], 1);
>                system("rev");
>                                       // "enfant2": prêt
>                                       // "enfant2": actif, exécute execl() et devient "/usr/bin/rev": prêt, `man rev.1`
>                                       // "/usr/bin/rev": actif, lis STDIN et écris chaque mots à l'envers dans STDOUT (pipe WRITE_ONLY) et quitte
>        }
>        close(p[1]);
>                                       // "p": actif, ferme (pipe WRITE_ONLY)
>                                       // "enfant1": actif, ferme (pipe WRITE_ONLY (original))
>        dup2(p[0], 0);
>                                       // "p": actif, place (pipe READ_ONLY) sur STDIN
>                                       // "enfant1": actif, idem
>        char buf[10] = ""; 
>                                       // "p": actif, initialise buffer avec une 
>                                       // "enfant1": actif, idem
>        while(read(0, buf, 10) > 0) { 
>                                       
>                write(1, buf, 10);
> 
>                write(1, "*", 1);
> 
>        } 
> 
> }
> 
> Note: plusieurs processeurs sont disponibles pour le parrallélisme.
```

### Q5

Peut-on caractériser (ou pas) la situation comme un interblocage, un « live lock », une famine, etc. ?
Justifiez votre réponse.

>
>
>
>


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

>
>
>

### Q7

Quelle est la cause selon vous de la différence de comportement entre les deux machines?
Que pensez-vous de l'attitude de l'étudiant.
Justifiez votre réponse.

>
>
>

## Correction

### Q8

En vous adressant à l'étudiant distrait, indiquez et expliquez les nombreux problèmes dans son code ainsi que ce qui aurait dû être fait.

>
>
>

### Q9

En appliquant vos indications de la question précédente, proposez une version corrigée et fonctionnelle du programme `p.c` (contrainte: vous devez garder l'utilisation de tubes et l'appel à la commande `rev`).

```c
TODO
```

## Extra

### Q10

Quelle est votre capsule préférée du cours INF3173. Expliquez pourquoi.

>
>
>
