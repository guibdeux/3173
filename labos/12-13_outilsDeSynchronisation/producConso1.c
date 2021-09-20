

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MAXDONNEE 5 // Nombre maximal de données produites par chaque producteur
#define MAX_PRODUCTEUR 5 // Nombre de producteurs
#define MAX_CONSOMATEUR 1 // Nombre de consommateurs

// La déclaration des deux sémaphores permettant de savoir s'il y a une donnée disponible pour le consommateur et si le producteur doit produire une donnée
sem_t empty;
sem_t full;

// Le mutex pour éviter les accès concurrent à l'index d'écriture.
pthread_mutex_t mutex_prod = PTHREAD_MUTEX_INITIALIZER;

#define TAILLE_MAX 10

// Liste d'attente
char liste[TAILLE_MAX][100];
int index_ecriture=0;
int index_lecture=0;

void* producteur(void *id) {
    int id_producteur = *((int *) id);

    for (int i = 0; i < MAXDONNEE; i++) {
        // Gestion de la sémaphore
        sem_wait(&empty);
        pthread_mutex_lock(&mutex_prod);

        int j = index_ecriture;
        index_ecriture++;
        if (index_ecriture == TAILLE_MAX)
            index_ecriture = 0;

        // Génération de la donnée
        sprintf(liste[j], "Le producteur %d a créé la donnée %d\n", id_producteur, i);
        printf("(+) Le producteur %d a produit : \n\tLe producteur %d a créé la donnée %d\n",id_producteur ,id_producteur, i);

        int sleep_time = (rand() % 5) + 1;
        sleep(sleep_time);

        // Gestion de la sémaphore
        pthread_mutex_unlock(&mutex_prod);
        sem_post(&full);
    }
}

void* consommateur(void *id) {
    int id_consommateur = *((int *) id);

    for (int i = 0; i < MAX_PRODUCTEUR * MAXDONNEE; i++) {
        // Gestion de la sémaphore
        sem_wait(&full);

        int j = index_lecture;
        index_lecture++;
        if (index_lecture == TAILLE_MAX)
            index_lecture = 0;

        printf ("(-) Consommateur %d a consommé la donnée : \n\t\"%s\"", id_consommateur, liste[j]);

        // Gestion de la sémaphore
        sem_post(&empty);
    }
}

int main() {
    int i;
    srand(time(NULL)); // Initialise le générateur de nombres aléatoires

    // Initialisez les sémaphores
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, TAILLE_MAX);


    // Écrivez la création et la gestion des threads
    pthread_t prod_thread[MAX_PRODUCTEUR];
    pthread_t cons_thread[MAX_CONSOMATEUR];

    int arguments_prod[MAX_PRODUCTEUR], arguments_conso[MAX_CONSOMATEUR];
    // Création de l'ensemble des threads, qui exécuteront la fonction philosophe
    for (i=0; i!= MAX_PRODUCTEUR; i++){
        arguments_prod[i] = i;
        pthread_create( prod_thread + i, NULL , producteur, &arguments_prod[i]) ;
    }

    for (i=0; i!= MAX_CONSOMATEUR; i++){
        arguments_conso[i] = i;
        pthread_create( cons_thread + i, NULL , consommateur, &arguments_conso[i]) ;
    }


    // Attendre que l'ensemble des threads soit terminé.
    for (i=0; i!= MAX_PRODUCTEUR; i++){
        pthread_join( prod_thread[i], NULL ) ;
    }

    for (i=0; i!= MAX_CONSOMATEUR; i++){
        pthread_join( cons_thread[i], NULL ) ;
    }

    // Ajoutez la destruction des sémaphores
    sem_destroy(&full);
    sem_destroy(&empty);

    return 0;
}