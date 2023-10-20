#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_CLIENTI 100
#define MAX_SPORTELLI_A 5
#define MAX_SPORTELLI_B 3
#define ORARIO_A 10 // Orario in cui inizia il servizio A (supponiamo alle 10:00)
#define ORARIO_B 14 // Orario in cui inizia il servizio B (supponiamo alle 14:00)

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int coda_A[MAX_CLIENTI];
int coda_B[MAX_CLIENTI];
int numero_cliente_A = 1;
int numero_cliente_B = 1;
int coda_A_length = 0;
int coda_B_length = 0;
int sportelli_A_disponibili = MAX_SPORTELLI_A;
int sportelli_B_disponibili = MAX_SPORTELLI_B;

void* cliente(void* arg) {
    char tipo_servizio = *(char*)arg;
    int mio_numero;

    pthread_mutex_lock(&mutex);
    if (tipo_servizio == 'A') {
        mio_numero = numero_cliente_A++;
        coda_A[coda_A_length++] = mio_numero;
        printf("Cliente %ld del servizio A ha preso il numero %d.\n", pthread_self(), mio_numero);
    } else {
        mio_numero = numero_cliente_B++;
        coda_B[coda_B_length++] = mio_numero;
        printf("Cliente %ld del servizio B ha preso il numero %d.\n", pthread_self(), mio_numero);
    }
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void* impiegato_A(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (coda_A_length == 0) {
            pthread_cond_wait(&cond, &mutex);
        }

        int mio_numero = coda_A[0];
        printf("Impiegato A al servizio dello sportello %ld sta servendo il cliente %d del servizio A.\n", pthread_self(), mio_numero);
        for (int i = 0; i < coda_A_length - 1; i++) {
            coda_A[i] = coda_A[i + 1];
        }
        coda_A_length--;
        sportelli_A_disponibili++;
        pthread_mutex_unlock(&mutex);
        sleep(2); // Tempo per servire il cliente
    }
}

void* impiegato_B(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (coda_B_length == 0) {
            pthread_cond_wait(&cond, &mutex);
        }

        int mio_numero = coda_B[0];
        printf("Impiegato B al servizio dello sportello %ld sta servendo il cliente %d del servizio B.\n", pthread_self(), mio_numero);
        for (int i = 0; i < coda_B_length - 1; i++) {
            coda_B[i] = coda_B[i + 1];
        }
        coda_B_length--;
        sportelli_B_disponibili++;
        pthread_mutex_unlock(&mutex);
        sleep(2); // Tempo per servire il cliente
    }
}

int main() {
    pthread_t clienti[MAX_CLIENTI];
    pthread_t sportelli_A[MAX_SPORTELLI_A];
    pthread_t sportelli_B[MAX_SPORTELLI_B];

    for (int i = 0; i < MAX_SPORTELLI_A; i++) {
        pthread_create(&sportelli_A[i], NULL, impiegato_A, NULL);
    }

    for (int i = 0; i < MAX_SPORTELLI_B; i++) {
        pthread_create(&sportelli_B[i], NULL, impiegato_B, NULL);
    }

    int orario_corrente = 0;
    while (1) {
        if (orario_corrente >= ORARIO_A && coda_A_length < MAX_CLIENTI) {
            pthread_create(&clienti[coda_A_length], NULL, cliente, (void*)'A');
            orario_corrente = 0;
        }
        if (orario_corrente >= ORARIO_B && coda_B_length < MAX_CLIENTI) {
            pthread_create(&clienti[coda_B_length], NULL, cliente, (void*)'B');
            orario_corrente = 0;
        }
        orario_corrente++;
        sleep(1); // Passa un minuto
    }

    return 0;
}
