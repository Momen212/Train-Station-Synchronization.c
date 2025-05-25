#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define NUM_TRAINS 50
#define MAX_TRACKS 5

int occupiedTracks = 0;
pthread_mutex_t lock;

void *train_routine(void *arg) {
    int id = *(int*)arg;
    free(arg); // Speicher wieder freigeben

    int hasEntered = 0;

    while (!hasEntered) {
        pthread_mutex_lock(&lock);

        if (occupiedTracks < MAX_TRACKS) {
            occupiedTracks++;
            printf("Zug %d fährt ein. (Belegt: %d)\n", id, occupiedTracks);
            hasEntered = 1;

            pthread_mutex_unlock(&lock);

            int stay_time = (rand() % 3) + 1;
            sleep(stay_time);

            pthread_mutex_lock(&lock);
            occupiedTracks--;
            printf("Zug %d fährt nach %d Sekunden ab. (Belegt: %d)\n", id, stay_time, occupiedTracks);
            pthread_mutex_unlock(&lock);
        } else {
            pthread_mutex_unlock(&lock);
            usleep(100000); // 0.1 Sekunden warten und erneut versuchen
        }
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t trains[NUM_TRAINS];
    srand(time(NULL)); // Zufallszahlen initialisieren

    if (pthread_mutex_init(&lock, NULL) != 0) {
        perror("Fehler beim Initialisieren des Mutex");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < NUM_TRAINS; i++) {
        int *id = malloc(sizeof(int));
        if (id == NULL) {
            perror("Speicher konnte nicht zugewiesen werden");
            return EXIT_FAILURE;
        }

        *id = i + 1;
        if (pthread_create(&trains[i], NULL, train_routine, id) != 0) {
            perror("Thread konnte nicht erstellt werden");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < NUM_TRAINS; i++) {
        if (pthread_join(trains[i], NULL) != 0) {
            perror("Thread konnte nicht verbunden werden");
            return EXIT_FAILURE;
        }
    }

    pthread_mutex_destroy(&lock);
    return 0;
}
