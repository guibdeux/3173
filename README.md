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

* Nom:
* Prénom:
* Code permanent:

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
	// TODO
}
```


### Q2

Comment l'utilisateur de la fonction `fork2` pourrait faire pour savoir s'il est dans le cas du processus parent, du processus enfant, ou du processus petit-enfant?

>
>
>


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

>
>
>
>
>
>
>


### Q4

Expliquez pourquoi, lors de la commande `rl tata`, rien ne semble affiché.

>
>
>
>


### Q5

En vous adressant au programmeur distrait, indiquez et expliquez les nombreux problèmes dans son code ainsi que ce qui aurait dû être fait.

>
>
>
>
>
>


### Q6

En appliquant vos indications de la question précédente, proposez une version corrigée et fonctionnelle du programme `rl.c` (vous devez bien sûr conserver l'appel système `readlink`).

```c
// TODO
```


### Q7

Trouvez et justifiez une situation qui fait potentiellement crasher le programme `rl` (avec une erreur de segmentation par exemple).


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

>
>
>
>


### Q9

Pourquoi le programme `readlink` est correctement identifié, chargé et exécuté par le `execvp` malgré la valeur inattendue de `argv[0]`?

>
>
>
>
>


### Q10

Pourquoi le programme de l'ami n'affiche pas le message d'erreur dans le cas du `rl2 tata` ?

>
>
>
>
>
