---
title: Devoir intra INF3173
author: Jean Privat
date: du 28 au 29 juin 2021
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
* La qualité du français ainsi que la rigueur, la précision et la justesse dans les explications et dans le vocabulaire du cours utilisé sera pris en compte.


## Identification des étudiants

* Nom: Blouin
* Prénom: Guillaume
* Code permanent: BLOG09079006

Note: Vérifiez que votre code permanent est correct.


## Quiz

La première partie du devoir est sous forme d'un quiz indépendant.
Quand le quiz est commencé, il doit être terminé en moins de 30 minutes.

* <https://www.moodle2.uqam.ca/coursv3/mod/quiz/view.php?id=2400255>


## Fork2

### Q1

Écrivez une fonction C `pid_t fork2(void)` qui crée deux processus : un processus enfant et un processus petit-enfant.
La fonction retourne le pid du processus enfant au processus parent, le pid du processus petit-enfant au processus enfant et 0 au processus petit-enfant.

```c
pid_t fork2(void) {
    pid_t pid = fork();
    pid_t pid_2;

    switch (pid) {
        case -1: // error code
            perror("fork error: ");
            return EXIT_FAILURE;
        case 0: // first child code
            pid_2 = fork();
            switch (pid_2) {
                case -1: // first child error code
                    perror(" child fork error: ");
                    return EXIT_FAILURE;
                case 0: // second child code
                    return 0;
                default: // first child code
                    return pid_2;
            }
        default: // parent code
            return pid;
    }
}
```


### Q2

Comment l'utilisateur de la fonction `fork2` pourrait faire pour savoir s'il est dans le cas du processus parent, du processus enfant, ou du processus petit-enfant?

> Faire "pid_t pere = getpid()" avant l'appel de la fonction fork2().\
> Faire des conditions avant les prochaines instructions.\
> Si le retour de fork2() est 0, nous sommes dans le petit-fils.\
> Si le retour de fork2() est (pere + 1), nous sommes dans le père.\
> Si le retour de fork2() est (père + 2), nous sommes dans fils.


## Symbolique

Soit le programme `rl.c` suivant développé par un programmeur distrait qui sert à faire (en moins bien) ce que la commande POSIX `readlink` fait déjà: afficher le chemin lié d'un lien symbolique.

```c
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
	char link[10] = "";
	struct stat st;
	stat(argv[1], &st);
	if (st.st_mode | S_IFLNK) {
		readlink(argv[1], link, st.st_size);
		printf(link);
		printf("\n");
	} else {
		printf("C'est pas un lien dur.\n");
	}
}
```

Le programmeur distrait essaye alors son programme sur le cas suivant:

```
$ ls -l
total 4
-rw-r--r-- 1 privat privat 17 jun 28 00:01 bonjour
-rw-r--r-- 1 privat privat  0 jun 28 00:01 hello
lrwxrwxrwx 1 privat privat  5 jun 28 00:01 tata -> hello
lrwxrwxrwx 1 privat privat  7 jun 28 00:01 toto -> bonjour
$ rl toto
bonjour
$ rl tata

$
```

À sa grande surprise, le programme fonctionne pour `toto`, mais à sa plus grande surprise, le programme ne fonctionne pas pour `tata` et semble afficher une chaine vide dans ce cas.


### Q3

Indiquez quels sont les appels systèmes `execve` (ou variations), `exit` (ou `exit_group`), `fork` (ou `clone`), `read`, `readlink`, `stat` (ou autre \*stat), `wait` et `write` exécutés lors de la commande `rl toto` de l'exemple.
Indiquez et expliquez également, la valeur et l'origine des arguments ainsi que la justification de la valeur de retour.

> ### fstat(3, {st_mode=S_IFREG|0644, st_size=119770, ...}) = 0 
> S_IFREG = regular file \
> Argument1: int fd, (descripteur cible) \
> Argument2: struct stat* statbuf, st_mode contient le type du fichier et les droits, st_size contient en octets la taille du fichier pointé \
> Retour: int retour, valeur de succès/échec \
> ### fstat(3, {st_mode=S_IFREG|0755, st_size=2029224, ...}) = 0 
> S_IFREG = regular file \
> Argument1: int fd, (descripteur cible) \
> Argument2: struct stat* statbuf, st_mode contient le type du fichier et les droits, st_size contient en octets la taille du fichier pointé \
> Retour: int retour, valeur de succès/échec \
> ### stat("toto", {st_mode=S_IFREG|0644, st_size=29, ...}) = 0 
> S_IFREG = regular file \
> Argument1: char*, (fichier cible, ici c'est le symlink) \
> Argument2: struct stat* statbuf, st_mode contient le type du fichier et les droits, st_size contient en octets la taille du fichier pointé \
> Retour: int retour, valeur de succès/échec \
> ### readlink("toto", "bonjour", 29) = 7 
> Argument1: const char *pathname, (symlink) \
> Argument2: char *buf, (nom du fichier pointé), (variable link) \
> Argument3: size_t bufsiz, (taille maximale du nom du fichier pointé selon readlink, mais le chiffre est égal à la taille du fichier bonjour de 29 octets selon stat(1)) \
> Retour: int retour, nombre d'octets placés dans char *buf (Argument 2) \
> ### fstat(1, {st_mode=S_IFCHR|0620, st_rdev=makedev(0x88, 0), ...}) = 0 
> S_IFCHR = character device \
> Argument1: int fd, (descripteur cible) \
> Argument2: struct stat* statbuf, st_mode contient le type du fichier et les droits, st_size contient en octets la taille du fichier pointé\
> Retour: int retour, valeur de succès/échec \
> ### write(1, "bonjour\n", 8bonjour) = 8 
> Argument1: int fd, descripteur cible, où écrire, sortie standard
> Argument2: const void *buf, est la variable "link" du premier printf() \
> Argument2 (suite): char "\n" du deuxieme printf() \
> Argument3: size_t count, taille d'écriture en nombre de chars ainsi que les chars \
> Retour: int retour, retourne le nombre de chars écrits \
> ### exit_group(0) = ? 
> Argument1: int status, status de fin du processus (retourné au processus parent)
> Retour: la fonction ne retourne rien


### Q4

Expliquez pourquoi, lors de la commande `rl tata`, rien ne semble affiché.

> lorsque le processus lance "stat()" nous obtenons: 
> #### stat("tata", {st_mode=S_IFREG|0644, st_size=0, ...}) = 0 
> la taille en octet du fichier pointé est zéro; \
> lorsque nous entrons cette valeur dans "readlink()"
> #### readlink(argv[1], link, st.st_size);
> la taille st.st_size venue de "stat()" offre le nombre de chars à placer dans link. \
> > le nombre de chars à placer est zéro char, alors rien ne va s'écrire dans la variable "link" même si elle possède (sizeof(char) * 10).


### Q5

En vous adressant au programmeur distrait, indiquez et expliquez les nombreux problèmes dans son code ainsi que ce qui aurait dû être fait.

> Commencer le programme avec la fonction **lstat()** \
> Enlever le **magic number** pour la variable **link** \
> Faire une **allocation dynamique** \
> Utiliser le **retour** de readlink() \
> Vérifier l'**échec** de la fonction **readlink()**, au lieu d'une condition mal exécutée \
> sinon échanger **if (st.st_mode | S_IFLNK) {** par **if ((st.st_mode & S_IFMT) == S_IFLNK) {** \
> Ne pas oublier le **retour** de fonction **main** \
> Échanger le mot **"dur"** par **"symbolique"**


### Q6

En appliquant vos indications de la question précédente, proposez une version corrigée et fonctionnelle du programme `rl.c` (vous devez bien sûr conserver l'appel système `readlink`).

```c
int main(int argc, char **argv) {
    struct stat st; ssize_t len;
    lstat(argv[1], &st);
    char* link = malloc(sizeof(char) * (st.st_size + 1));
    if ((len = readlink(argv[1], link, (st.st_size + 1))) != -1) {
        link[len] = '\0';
        printf("%s\n", link);
    } else {
        free(link);
        perror("ce n'est un lien mou"); 
        exit(1);
    }
    free(link);
    return 0;
}
```


### Q7

Trouvez et justifiez une situation qui fait potentiellement crasher le programme `rl` (avec une erreur de segmentation par exemple).

> lorsque **st.st_size >= 10**, lorsque la taille du fichier dans la struct stat sera plus grande ou égale à 10. \
> *** stack smashing detected ***: terminated Aborted (core dumped) \
> En effet, on essait d'**utiliser plus de mémoire que disponible** dans la variable **link**


## Eh l'ami

L'ami du programmeur distrait lui propose plutôt la version `rl2.c` suivante qu'il juge plus simple.

```c
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
	if (execvp("readlink", argv) == -1) {
		printf("C'est pas un lien dur.\n");
	}
}
```

Cette version semble fonctionner, mais n’affiche rien si le fichier passé en argument n'existe pas (au lieu d'afficher le message d'erreur).

```
$ rl2 toto
bonjour
$ rl2 tata
hello
$ rl2 titi
$ 
```


### Q8

Dans `rl2`, à quoi correspond `argv[0]` en général et quelle en est la valeur dans les trois exemples ci-dessus (toto, tata, titi) ?

> en général, argv[0] correspond au chemin relatif du programme en exécution, c'est-à-dire, **./rl2** \
> dans les trois exemples, la valeur de argv[0] sera toujours **./rl2**, même dans l'exécution de la fonction execvp()

### Q9

Pourquoi le programme `readlink` est correctement identifié, chargé et exécuté par le `execvp` malgré la valeur inattendue de `argv[0]`?

> le programme **readlink distingue les bons arguments des mauvais** \
> Voici un exemple avec readlink(1) 
> #### readlink kawbrwrfgljwfglw wekjfbwkej **toto** wekbfwekfb erbgos akrejbgkaej **tata** 
> bonjour \
> hello


### Q10

Pourquoi le programme de l'ami n'affiche pas le message d'erreur dans le cas du `rl2 titi` ?

> execve("/home/guib/.local/bin/readlink", ["./ami", "titi"], 0x7ffc6917e030 /* 54 vars */) = -1 ENOENT (No such file or directory) \
> execve("/usr/local/sbin/readlink", ["./ami", "titi"], 0x7ffc6917e030 /* 54 vars */) = -1 ENOENT (No such file or directory) \
> execve("/usr/local/bin/readlink", ["./ami", "titi"], 0x7ffc6917e030 /* 54 vars */) = -1 ENOENT (No such file or directory) \
> execve("/usr/sbin/readlink", ["./ami", "titi"], 0x7ffc6917e030 /* 54 vars */) = -1 ENOENT (No such file or directory)
> ### execve("/usr/bin/readlink", ["./ami", "titi"], 0x7ffc6917e030 /* 54 vars */) = 0
> Nous n'entrons pas dans la condition d'erreur du programme car la **fonction execvp() est un succès**
> ### readlink("titi", 0x55b54fc08440, 64) = -1 EINVAL (Invalid argument)
> readlink retourne -1, le cas d'échec à vérifier n'est pas au bon endroit
