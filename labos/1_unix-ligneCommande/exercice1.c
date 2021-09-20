#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE_MAX 400
5
int main(){
	char *tab_pointeurs[6], phrase_saisie[TAILLE_MAX];
	int i;
	FILE *file;

	//ouverture du fichier où on va enregistrer
	file = fopen("enregistrement.txt", "w");
	if(!file){
		perror("enregistrement.txt");
		return 1;
	}

	for(i=0; i<6; i++){
		//saisir les phrases
		printf("Saisir la phrase %d: ", i+1);
		fgets(phrase_saisie, TAILLE_MAX, stdin);
		//allouer de l'espace mémoire sur mesure
		int len = strlen(phrase_saisie) + 1;
		tab_pointeurs[i] = malloc(len); // +1 parce que le '\0' final
		// Gestion d'éventuelles erreurs lors de l'allocation
		if (tab_pointeurs[i] == NULL) {
			perror("Allocation");
			if (i % 2 != 0) {
				free(tab_pointeurs[i-1]);
			}
			return 1;
		}
		//copier la phrase dans le vecteur alloué
		strncpy(tab_pointeurs[i], phrase_saisie, len);
		if(i%2 != 0){
			//écrire dans le fichier et ensuite libérer l'espace
			fprintf(file, "%s%s", tab_pointeurs[i-1], tab_pointeurs[i]);
			free(tab_pointeurs[i-1]);
			free(tab_pointeurs[i]);
		}
	}

	fclose(file);

}
