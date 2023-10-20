/*
 * Scrivere un programma che prende in input un intero n, il nome di
un file di testo ed un carattere x
• Il programma ha il compito di contare le occorrenze
del carattere x nel file di testo
• Il programma esegue tale compito creando n thread, ognuno dei
quali esamina una porzione diversa del file di testo
• ad esempio, se il file è lungo 1000 bytes ed n=4, il primo thread
esaminerà i primi 250 bytes, il secondo thread esaminerà i 250 bytes
successivi, e così via
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/param.h>
#include <sys/stat.h>
#define MAX_THREAD 10000
struct info{
    int occ;
    char *filename;
    long long inizio,fine;
    char chiave;
};
void *contaOcc(void * params){
    struct info *p = ((struct info*)params);
    FILE *file = fopen(p->filename,"r");
    if(file==NULL){
        perror("file in thread");exit(-9);
    }
    fseek(file, p->inizio, SEEK_SET);
    char c;
    int cont=0;
    for (long long i = p->inizio; i < p->fine; i++) {
        c = fgetc(file);
        if (c == p->chiave) {
            cont++;
        }
    }
    p->occ=cont;
    printf("%d\n",p->occ);
    fclose(file);
    pthread_exit(NULL);

}
int main(int argc, char **argv) {
    FILE *fp = fopen(argv[2],"r");
    if(!fp){
        perror("file");
        pthread_exit(NULL);
    }
    int nthreads = MIN(atoi(argv[1]),MAX_THREAD);
    char k = argv[3][0];
    pthread_t tid[nthreads];
    struct info att_tid[nthreads];
    fseek(fp, 0, SEEK_END);
    long long size=ftell(fp);
    printf("Numero di byte su file: %lld\n", size);
    long long  fd_tid = size/nthreads;
    printf("Numero di byte per thread: %lld\n", fd_tid);

    fclose(fp);
    for(int i=0;i<nthreads;i++){
        att_tid[i].filename = argv[2];
        att_tid[i].inizio = i * fd_tid;
        att_tid[i].chiave = k;
        att_tid[i].fine =(i==nthreads-1)?size:(i+1)*fd_tid;
        pthread_create(&tid[i],NULL,(void*) contaOcc,&att_tid[i]);
    }
    int totOcc=0;
    for(int i=0;i<nthreads;i++){
        pthread_join(tid[i],NULL);
        totOcc+=att_tid[i].occ;
    }
    printf("tot occ -> %d",totOcc);





}
