#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

#define SEM_S "/scrittore"
#define SEM_L "/lettore"

sem_t *sem_scrittore, *sem_lettore;
struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    char *filename;
    int numLetti;
} shared = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};
int cont = -1, N; // per scrittore;

void *scrittore(void *arg) {
    int fp = open(shared.filename, O_CREAT | O_RDWR, 0777);
    if (fp == -1) {
        perror("file");
        exit(-3);
    }
    int a = 0, b = 1, c;
    for (int i = 0; i < N; i++) {
        sem_wait(sem_lettore);
        c = a + b;
        a = b;
        b = c;
        printf("num sequenza --> %d\n", b);
        lseek(fp, 0, SEEK_SET);
        write(fp, &b, sizeof(int));
        sem_post(sem_scrittore);
    }
    return NULL;
}

void *lettore(void *arg) {
    int fp = open(shared.filename, O_CREAT | O_RDWR, 0777);
    int num = 0;
    if (fp == -1) {
        perror("file");
        exit(-4);
    }
    for (int i = 0; i < N; i++) {
        sem_wait(sem_scrittore);
        lseek(fp, 0, SEEK_SET);
        read(fp, &num, sizeof(int));
        printf("ho letto -> %d\n", num);
        cont++;
        sem_post(sem_lettore);
        pthread_mutex_lock(&shared.mutex);
        shared.numLetti = cont;
        if (shared.numLetti == N) {
            pthread_cond_signal(&shared.cond);
            pthread_mutex_unlock(&shared.mutex);
        }
        pthread_mutex_unlock(&shared.mutex);
    }
    return NULL;
}

void *thUscita(void *arg) {
    pthread_mutex_lock(&shared.mutex);
    while (shared.numLetti < N) {
        pthread_cond_wait(&shared.cond, &shared.mutex);
    }
    pthread_mutex_unlock(&shared.mutex);
    printf("operazioni concluse\n");
    sem_close(sem_scrittore);
    sem_close(sem_lettore);
    sem_unlink(SEM_S);
    sem_unlink(SEM_L);
    exit(0);
}

int main(int argc, char **argv) {
    if (argc != 2 || atoi(argv[1]) <= 10 || atoi(argv[1]) > 15) {
        printf("fornire : ./a  <#num> \n <#num> compreso tra 11 e 15\n");
        exit(-1);
    }
    char *nomefile = "a.dat";
    N = atoi(argv[1]);
    printf("max: %d\n", N);
    shared.filename = nomefile;
    pthread_t tid[3];
    sem_scrittore = sem_open(SEM_S, O_CREAT | O_EXCL, 0777, 0);
    sem_lettore = sem_open(SEM_L, O_CREAT | O_EXCL, 0777, 1);
    pthread_create(&tid[0], NULL, scrittore, NULL);
    pthread_create(&tid[1], NULL, lettore, NULL);
    pthread_create(&tid[2], NULL, thUscita, NULL);
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);
    return 0;
}
