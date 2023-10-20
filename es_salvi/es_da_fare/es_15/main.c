/*
 * Si realizzi un programma in C e Posix sotto Linux che,
utilizzando la libreria Pthread
• lancia n thread per cercare un elemento in una matrice nxn di
caratteri
• Ognuno dei thread cerca l'elemento in una delle righe della
matrice
• Non appena un thread ha trovato l'elemento cercato, rende note
agli altri thread le coordinate dell'elemento e tutti i thread
terminano (sono cancellati)
 */
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <malloc.h>
#include <stdlib.h>
#include <bits/sigthread.h>

#define N 5

struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    bool trovato;

} finder = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, false};
struct {
    int riga;
    char car;
    int posizione[2];
}info[N]={0};


char mat[N][N];

void *trovaEl(int k) {
    pthread_mutex_lock(&finder.mutex);
    for(int i=0;i<N;i++){
        if(info[k].car == mat[k][i]){
            finder.trovato=true;
            info[i].posizione[0]=k;
            info[i].posizione[1]=i;
            //pthread_cond_signal(&finder.cond);
            printf("thread[%lu]: riga[%d]-colonna[%d]",pthread_self(), info[i].posizione[0], info[i].posizione[1]);

            if(finder.trovato == true) pthread_cond_signal(&finder.cond);
            pthread_mutex_unlock(&finder.mutex);
        }
        //pthread_cond_wait(&finder.cond,&finder.mutex);
    }
    pthread_mutex_unlock(&finder.mutex);
    return NULL;
}

int main() {

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            mat[i][j]='s';
        }
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            mat[i][j]=((rand() % (90-65)) + 65);
        }
    }
    mat[2][2]='A';
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("%c ",mat[i][j]);
        }
        printf("\n");
    }
    pthread_t tid[N];
    for (int i = 0; i < N; i++) {
        info[i].riga=i;
        info[i].car='A';
        if (pthread_create(&tid[i], NULL, (void *) trovaEl, (void *) i)) {
            printf("thread [%d] non creato\n", i);
        }
    }
    void *tret;

    pthread_mutex_lock(&finder.mutex);
    while(finder.trovato == false){
        pthread_cond_wait(&finder.cond,&finder.mutex);
        for(int i=0;i<N;i++) pthread_kill(tid[i],0);
    }
    pthread_mutex_unlock(&finder.mutex);
    for (int i = 0; i < N; i++) { pthread_join(tid[i], &tret); }



}
