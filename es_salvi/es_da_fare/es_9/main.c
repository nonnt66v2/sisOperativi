#include <stdio.h>
#include "/home/nonnt66/Scaricati/uni/so/apue.3e/include/apue.h"
#include <pthread.h>
struct char_print_parms{
    char car;
    int count;
};

void * char_print(struct char_print_parms *param){
    for (int i = 0; i <  param->count ; i++) fputc(param->car,stderr);
    return NULL;

}
int main() {
    pthread_t tid1;
    pthread_t tid2;
    struct char_print_parms tid1_args;
    struct char_print_parms tid2_args;
/* crea un thread per stampare 30000 ‘x’ */
    tid1_args.car = 'y';
    tid1_args.count = 30;
    pthread_create(&tid1,NULL, (void*)char_print, (void *)&tid1_args);
/* crea un thread per stampare 20000 'y’ */
    tid2_args.car = 'x';
    tid2_args.count = 20;
    pthread_create(&tid2,NULL, (void*)char_print, (void*)&tid2_args);
    sleep(1);
    return 0;
}
