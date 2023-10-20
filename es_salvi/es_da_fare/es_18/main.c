#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "unistd.h"

#define N 1000      // Numero di passeggeri
#define M 5         // Numero di code di accesso al metal detector
#define K 10        // Lunghezza minima delle code
#define MAX_WAIT 5  // Massimo tempo di attesa in secondi

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int code[N] = {0}; // Array per tenere traccia delle lunghezze delle code
int addetti_disponibili = M; // Numero di addetti disponibili

void* passeggero(void* arg) {
    int id = *((int*)arg);
    int scelta = rand() % N; // Scelta casuale di una coda
    sleep(rand() % MAX_WAIT); // Simula il tempo di arrivo del passeggero

    pthread_mutex_lock(&mutex);
    code[scelta]++; // Incrementa la lunghezza della coda scelta
    if (code[scelta] <= K && addetti_disponibili > 0) {
        printf("Passeggero %d entra nella coda %d\n", id, scelta);
        addetti_disponibili--;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond); // Segnala agli addetti
    } else {
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_lock(&mutex);
    while (code[scelta] > K / 2) {
        pthread_cond_wait(&cond, &mutex); // Aspetta finché la condizione non è soddisfatta
    }

    printf("Passeggero %d attraversa il metal detector\n", id);
    code[scelta]--;
    addetti_disponibili++;
    pthread_mutex_unlock(&mutex);

    // Simula il tempo impiegato per attraversare il metal detector
    sleep(rand() % MAX_WAIT);

    printf("Passeggero %d si dirige all'imbarco\n", id);
    return NULL;
}

void* addetto(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (addetti_disponibili == M) {
            pthread_cond_wait(&cond, &mutex); // Aspetta finché ci sono passeggeri in attesa
        }

        // Trova la coda più breve
        int coda_minima = 0;
        for (int i = 1; i < N; i++) {
            if (code[i] < code[coda_minima]) {
                coda_minima = i;
            }
        }

        if (code[coda_minima] <= K) {
            addetti_disponibili--;
            pthread_mutex_unlock(&mutex);
            pthread_cond_signal(&cond); // Segnala al passeggero
            sleep(rand() % MAX_WAIT); // Simula il tempo impiegato per indirizzare il passeggero
            pthread_mutex_lock(&mutex);
            addetti_disponibili++;
            pthread_mutex_unlock(&mutex);
        } else {
            pthread_mutex_unlock(&mutex);
        }
    }
}

int main() {
    pthread_t passeggeri[N];
    pthread_t addetti[M];
    int id[N];

    srand(time(NULL));

    for (int i = 0; i < N; i++) {
        id[i] = i;
        pthread_create(&passeggeri[i], NULL, passeggero, &id[i]);
    }

    for (int i = 0; i < M; i++) {
        pthread_create(&addetti[i], NULL, addetto, NULL);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(passeggeri[i], NULL);
    }

    return 0;
}
