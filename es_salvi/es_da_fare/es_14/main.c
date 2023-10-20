/// prod - cons

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/param.h>

#define MAXITEMS 100000
#define MAXTHREAD 100
int nitems;
int buff[MAXITEMS];
struct {
    pthread_mutex_t mutex;
    int nput,nval;
}put={PTHREAD_MUTEX_INITIALIZER};
struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int nready;
}nready = {PTHREAD_MUTEX_INITIALIZER,PTHREAD_COND_INITIALIZER};

void*produce(void * par){
    for(;;) {
        pthread_mutex_lock(&put.mutex);
        if (put.nput >= nitems) {
            pthread_mutex_unlock(&put.mutex);
            return NULL;
        }
        buff[put.nput] = put.nval;
        put.nput++;
        put.nval++;
        pthread_mutex_unlock(&put.mutex);

        pthread_mutex_unlock(&nready.mutex);
        if(nready.nready == 0)
            pthread_cond_signal(&nready.cond);
        nready.nready++;
        pthread_mutex_unlock(&nready.mutex);

        *((int *) par)+=1;
    }

};

void*consume(void * par){
    int i;
    for(i=0;i<nitems;i++){
        pthread_mutex_lock(&nready.mutex);
        while(nready.nready == 0)
            pthread_cond_wait(&nready.cond,&nready.mutex);
        nready.nready--;
        pthread_mutex_unlock(&nready.mutex);

        if(buff[i] != i)
            printf("buff[%d] = %d",i,buff[i]);
    }
    return NULL;

};
int main(int argc, char ** argv) {
    int nthreads,cont[MAXITEMS];
    if(argc != 3){
        printf("fornire 2 parametri");exit(-8);
    }
    nitems = MIN(atoi(argv[1]),MAXITEMS);
    nthreads = MIN(atoi(argv[2]),MAXTHREAD);

    pthread_t tid_produce[MAXTHREAD],tid_consume;
    //produt
    for(int i = 0 ; i < nthreads;i++){
        cont[i]=0;
        pthread_create(&tid_produce[i],NULL,(void*)produce,&cont[i]);
    }
    pthread_create(&tid_consume,NULL,consume,NULL);

    for(int i = 0 ; i < nthreads;i++){
        pthread_join(tid_produce[i],NULL);
        printf("count[%d] = %d\n",i,cont[i]);
    }
    void *tret;
    pthread_join(tid_consume,&tret);
   // printf("thread[%lu] = %ld\n", pthread_self(),(unsigned long) tret);

};

