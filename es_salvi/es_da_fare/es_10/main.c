#include <stdio.h>
#include <pthread.h>
#include "apue.h"

struct att{
    int num;
    char car;
};
void *stampaInfo(struct att *par){
    for(int i=0;i<par->num;i++){
        printf("%c",par->car);
    }
}
int main() {
    //pid_t pid;
    pthread_t tid[2];
    void *tret;
    struct att thr_arg1,thr_arg2;
    thr_arg1.num = 3;
    thr_arg1.car = 'a';
    int err = pthread_create(&tid[0],NULL,(void*) stampaInfo,&thr_arg1);
    if(err != 0){
        printf("thread non creato %d",err);
    }

    thr_arg2.num = 4;
    thr_arg2.car = 'b';
    err = pthread_create(&tid[1],NULL,(void*) stampaInfo,&thr_arg2);

    if(err != 0){
        printf("thread non creato %d",err);
    }
    err = pthread_join(tid[0],&tret);
    if(err != 0){
        printf("non può essere eseguito il th1 %d",err);
    }
    printf("\nth1 exit code %ld\n",(long)tret);
    err = pthread_join(tid[1],&tret);
    if(err != 0){
        printf("non può essere eseguito il th2 %d",err);
    }
    printf("\nth2 exit code %ld\n",(long)tret);
}
