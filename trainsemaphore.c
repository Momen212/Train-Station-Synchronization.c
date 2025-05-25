#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define NUM_TRAINS 50
#define MAX_TRACKS 5

sem_t availableTracks;

void *train_routine(void *arg) {
    int id = *(int*)arg;
    free(arg);

    // Zug wartet auf freien Gleisplatz 
    sem_wait(&availableTracks); // Semaphor um 1 verringern (blockiert wenn 0)
    
    printf("Zug %d fährt ein.\n", id);

    int stay_time = (rand() % 3) + 1;
    sleep(stay_time);

    printf("Zug %d fährt nach %d Sekunden ab.\n", id, stay_time);

    // Freigeben eines Gleises 
    sem_post(&availableTracks); // Semaphor um 1 erhöhen

    pthread_exit(NULL);
}

int main() {
    pthread_t trains[NUM_TRAINS];
    srand(time(NULL));

    // Initialisiere den Semaphor mit MAX_TRACKS verfügbaren Plätzen
    if (sem_init(&availableTracks, 0, MAX_TRACKS) != 0) {
        perror("Semaphor konnte nicht initialisiert werden");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < NUM_TRAINS; i++) {
        int *id = malloc(sizeof(int));
        if (id == NULL) {
            perror("Speicherfehler");
            return EXIT_FAILURE;
        }
        *id = i + 1;

        if (pthread_create(&trains[i], NULL, train_routine, id) != 0) {
            perror("Fehler beim Erstellen des Threads");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < NUM_TRAINS; i++) {
        if (pthread_join(trains[i], NULL) != 0) {
            perror("Fehler beim Warten auf Thread");
            return EXIT_FAILURE;
        }
    }

    if (sem_destroy(&availableTracks) != 0) {
        perror("Semaphor konnte nicht zerstört werden");
        return EXIT_FAILURE;
    }

    return 0;
}
