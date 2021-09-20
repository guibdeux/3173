DEBUG=yes
CC=gcc
GG=g++
OPTIONS ='-03 -Wall -Wextra -std=c++17'
# SRC=$(wildcard *.c)
# EXEC=$(SRC)
# OBJ=$(SRC:.c=.o)
# all: $(EXEC)
all: helloWorld_c helloWorld_cpp prime_c backup

# -Wall: enable a set of warning, actually not all.
# -W: enable extra warning, it's advised to use -Wextra instead which has the same meaning
# -Werror: every warning is treated as an error.
# -ansi: on ne peut déclarer le type de l’itérateur dans l’initialisation d’une boucle for
# -Werror=vla: initialisations de la taille des tableaux uniquement avec macro ou nombre naturel
# -std=c11: version compilateur
# -pedantic:  Using the stricter -pedantic-errors option converts such diagnostic warnings into errors that will cause compilation to fail at such points
# -pthreads: POSIX.1 specifies a set of interfaces (functions, header files) for threaded
# -lm: lier la bibliotheque math (dynaminquement? verifier--> ```ldd executable```)
# -lstdc++: C++ standard library, explicit linking with

ifeq ($(DEBUG),yes)
	CFLAGS='-W -Wall -Werror -std=c11 -g'
#	LDFLAGS=-lSDL -lSDL_mixer ((Extra)Linker SimpleDirectMediaLayer)
#	LDFLAGS='-lm'
	@echo "Génération en mode debug"
else
	CFLAGS='-W -O2 -Wall -pedantic'
#	LDFLAGS=-lSDL -lSDL_mixer
#	LDFLAGS='-lm'
	@echo "Génération en mode release"
endif

helloWorld_c: helloWorld.c
	@$(CC) $(CFLAGS) $^ -o helloWorld_c
	@grep -xq "$@" .gitignore 2>> /dev/null || echo $@ >> .gitignore


helloWorld_cpp: helloWorld.cpp
	@$(GG) -lstdc++ $^ -o helloWorld_cpp
	@grep -xq "$@" .gitignore 2>> /dev/null || echo $@ >> .gitignore

prime_c: prime.c
	@$(CC) $(CFLAGS) $^ -o $@ -lm
	@grep -xq "$@" .gitignore 2>> /dev/null || echo $@ >> .gitignore

.PHONY: clean mrproper backup

clean:
	@rm -rf helloWorld_c helloWorld_cpp prime_c 2> /dev/null


mrproper: clean

backup:
	@rm -rf ./myBackUp 2> /dev/null
	@mkdir ./myBackUp 2> /dev/null
	@cp -Rp *.c ./myBackUp/