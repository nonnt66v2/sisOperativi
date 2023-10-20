#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int my_global;
void * inc1(){
    int j=0;
    for (int i = 0; i < 20; ++i) {
        pthread_mutex_lock(&mutex);
        j = j+1;
        printf("o");
        fflush(stdout);
        sleep(1);
        my_global=j;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
int main() {
    pthread_t tid[1];
    if(pthread_create(&tid[0],NULL,(void*)inc1,NULL)){
        perror("thread");exit(-1);
    }
    for (int i = 0; i < 20; ++i) {
        pthread_mutex_lock(&mutex);
        my_global = my_global+1;
        pthread_mutex_unlock(&mutex);
        printf(".");
        fflush(stdout);
        sleep(1);
    }
    if(pthread_join(tid[0],NULL)){
        perror("join thread");exit(-2);
    }
    printf("my global = %d",my_global);
}
