/// prod - cons

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/param.h>

#define MAXITEMS 1000
#define MAXTHREAD 100
int nitems;
struct var_cond{
    pthread_mutex_t mutex;
    int buff[MAXITEMS];
    int nput,nval;
}shared={PTHREAD_MUTEX_INITIALIZER};
void*produce(void * par){
    for(;;) {
        pthread_mutex_lock(&shared.mutex);
        if (shared.nput >= nitems) {
            pthread_mutex_unlock(&shared.mutex);
            return NULL;
        }
        shared.buff[shared.nput] = shared.nval;
        shared.nput++;
        shared.nval++;
        pthread_mutex_unlock(&shared.mutex);
        *((int *) par)+=1;
    }

};
void consume_wait(int i){
    for(;;){
        pthread_mutex_lock(&shared.mutex);
        if(i < shared.nput){
            pthread_mutex_unlock(&shared.mutex);
            return;
        }
        pthread_mutex_unlock(&shared.mutex);
    }
}
void*consume(void * par){
    int i;
    for(i=0;i<nitems;i++){
        consume_wait(i);
        if(shared.buff[i] != i){
            printf("buff[%d] = %d\n",i,shared.buff[i]);
        }
    }
    return NULL;

};
int main(int argc, char * argv[]) {
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
    printf("thread[%lu] = %ld\n", pthread_self(),(unsigned long) tret);

};
