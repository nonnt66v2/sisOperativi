/*Si scriva un programma C, che crei 3 thread, che leggano 3 directory passate da riga di comando, e concorrentemente
inseriscono in una variabile condivisa il numero maggiore di file da loro trovato all'interno della directory.
Quando terminano il padre dovra' stampare a video la suddetta variabile.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <pthread.h>

struct variabileCond{
	pthread_mutex_t m;
	pthread_cond_t c;
	int cont;
	int max;
}vc={PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, 0, 0};

void* func(void *args){

	char* path=(char*)args;

	printf("tid= %ld, %s\n", pthread_self(), path);

	DIR *dp=opendir(path);
	struct dirent *dirp;

	int cont=0;

	while((dirp=readdir(dp))!=NULL) cont++;

	pthread_mutex_lock(&vc.m);
	if(vc.max<cont) vc.max=cont;
	vc.cont++;
	if(vc.cont==3) pthread_cond_signal(&vc.c);
	pthread_mutex_unlock(&vc.m);

pthread_exit(0);
}

int main(int argc, int *argv[]){

	if(argc!=4){ printf("Passare 3 path da riga di comando.\n"); exit(-1);}
	
	pthread_t tid[3]={0};

	for(int i=0; i<3; i++)
		pthread_create(&tid[i], NULL, func, (void*)*(argv+i+1));

	pthread_mutex_lock(&vc.m);
	while(vc.cont!=3)
		pthread_cond_wait(&vc.c, &vc.m);
	pthread_mutex_unlock(&vc.m);

	printf("Numero max files= %d\n", vc.max);

	for(int i=0; i<3; i++)
		pthread_join(tid[i], NULL);

exit(0);
}
