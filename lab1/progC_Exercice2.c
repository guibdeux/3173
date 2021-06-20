#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE_MAX 50

int main(int argc, char *argv[]){

	FILE *file;
	char phrase[TAILLE_MAX], **tableau_lignes = NULL, *phrase_tmp;
	int nb_lignes = 0, i, j;

	if (argc != 2) {
		fprintf(stderr, "Vous devez fournir un fichier en argument");
		return 1;
	}

	//ouverture du fichier passé en paramètre au programme en mode lecture
	file = fopen(argv[1], "r");
	if(!file){
		perror(argv[1]);
		return 1;
	}

	//lire le fichier et allouer l'espace mémoire pour chaque phrase
	while(fgets(phrase, TAILLE_MAX, file) != NULL){
		tableau_lignes = realloc(tableau_lignes, sizeof(*tableau_lignes)*(nb_lignes + 1));
	    *(tableau_lignes + nb_lignes) = malloc(sizeof(char)*strlen(phrase) + 1); // tableau_lignes[nb_lignes]
		if (tableau_lignes[nb_lignes] == NULL) {
			perror("Allocation");
			// Libérer l'espace des phrases précédentes
			for(i = 0; i < nb_lignes; i++){
				free(tableau_lignes[i]);
			}
			free(tableau_lignes);
			return 1;
		}
	    strcpy(*(tableau_lignes + nb_lignes), phrase);
	    nb_lignes++;
	}


	//trier les phrases lexicographiquement
	for (i=0 ; i < nb_lignes-1; i++)
	{
		for (j=0 ; j < nb_lignes-i-1; j++)
		{
		   if (strcmp(tableau_lignes[j], tableau_lignes[j+1]) > 0)
		   {
		      phrase_tmp = tableau_lignes[j];
		      tableau_lignes[j] = tableau_lignes[j+1];
		      tableau_lignes[j+1] = phrase_tmp;
		   }
		}
	}

	//afficher le résultat
	for(i=0; i<nb_lignes; i++){
		printf("%s", tableau_lignes[i]);
		free(tableau_lignes[i]);
	}
	free(tableau_lignes);

	//fermeture du fichier
	fclose(file);

}
