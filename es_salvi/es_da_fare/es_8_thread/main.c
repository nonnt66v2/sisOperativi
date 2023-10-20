#include <stdio.h>
#include "/home/nonnt66/Scaricati/uni/so/apue.3e/include/apue.h"
#include <pthread.h>

pthread_t ntid;

void printds(const char *s){
    pid_t pid;
    pthread_t tid;
    pid = getpid();
    tid=pthread_self();
    printf("%s pid %lu tid %lu (0x%lx)\n",s,
           (pid_t) pid,
           (unsigned long)tid,
           (unsigned long) tid);
}

void * thr_fn(void *arg){
    printds("nuovo thread:");
    return((void*) 0);
}
int main(void) {
    int err = pthread_create(&ntid,NULL,thr_fn,NULL);
    if(err != 0){
        printf("non posso creare il thread %s\n", strerror(err));
    }
    printds("thread principale:");
    sleep(1);
    exit(0);
}
