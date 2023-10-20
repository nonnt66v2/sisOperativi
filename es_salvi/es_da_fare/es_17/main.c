#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <apue.h>
#include "pthread.h"
#include "fcntl.h"
#define NBUFF 1000
#define SEM_MUTEX "/mutex"
#define SEM_NEMPTY "/empty"
#define SEM_NSTORED "/stored"
int nitems;
struct{
    int buff[NBUFF];
    sem_t *mutex,*nempty,*nstored;
}shared;

void *produce(){
    for(int i=0;i<nitems;i++){
        sem_wait(shared.nempty);
        sem_wait(shared.mutex);
        shared.buff[i%NBUFF] = i;
        sem_post(shared.mutex);
        sem_post(shared.nstored);
    }
    return NULL;
};
void *consume(){
    for(int i=0;i<nitems;i++){
        sem_wait(shared.nstored);
        sem_wait(shared.mutex);
        if(shared.buff[i] !=i){
            printf("errore in posizione buffer[%d] = %d\n",i,shared.buff[i]);
        }
        sem_post(shared.mutex);
        sem_post(shared.nempty);
    }
    return NULL;
}
int main(int argc , char **argv) {
    pthread_t tid_prod,tid_cons;
    if(argc != 2){
        printf("fornire : <#items>");exit(8);
    }
    nitems = atoi(argv[1]);
    shared.mutex = sem_open(SEM_MUTEX,O_CREAT | O_EXCL, FILE_MODE,1);
    shared.nempty = sem_open(SEM_NEMPTY,O_CREAT | O_EXCL, FILE_MODE,NBUFF);
    shared.nstored = sem_open(SEM_NSTORED,O_CREAT | O_EXCL, FILE_MODE,0);
    pthread_create(&tid_prod,NULL,(void*)produce,NULL);
    pthread_create(&tid_cons,NULL,(void*)consume,NULL);
    void *tret;
    pthread_join(tid_prod,&tret);
    pthread_join(tid_cons,&tret);
    sem_unlink(SEM_MUTEX);
    sem_unlink(SEM_NSTORED);
    sem_unlink(SEM_NEMPTY);
    exit(0);

}
