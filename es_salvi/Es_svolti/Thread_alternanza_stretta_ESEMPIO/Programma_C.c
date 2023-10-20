/*Alternanza stretta per lettura e scrittura su una varaibile*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>

#define LIMIT 30

sem_t *svegliaPadre;
sem_t *svegliaProd;
int var=0;



void* prod(void* args){
	
	printf("Prod inizializzato!\n");		
	
	
	while(1){
		sem_wait(svegliaProd);
		if(var==LIMIT){sem_post(svegliaProd); pthread_exit(0);}
		var++;
		sem_post(svegliaPadre);
	}

}

int main(){
	
	
	sem_unlink("svegliaProd");
	sem_unlink("svegliaPadre");	
	
	svegliaProd=sem_open("svegliaProd", O_CREAT, 777, 1);	
	svegliaPadre=sem_open("svegliaPadre", O_CREAT, 777, 0);

	struct timespec t;
	t.tv_sec=0;
	t.tv_nsec=500000000;

	pthread_t tidprod[5];
	
	for(int i=0; i<5; i++)
		pthread_create(&tidprod[i], NULL, prod, NULL);
	
	for(int i=0; i<LIMIT; i++){
		nanosleep(&t, NULL);
		sem_wait(svegliaPadre);
		printf("Var= %d\n", var);
		sem_post(svegliaProd);
	}
	printf("Aspetto terminazione figli\n");

	for(int i=0; i<5; i++)
		pthread_join(tidprod[i], NULL);
	

	sem_close(svegliaProd);
	sem_close(svegliaPadre);
exit(0);	
	
}
