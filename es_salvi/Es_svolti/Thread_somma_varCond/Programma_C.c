/*Si  realizzi  un  programma  C  e  Posix  in  ambiente  Linux  che,  impiegando  la  libreria  Pthread,  generi  tre  thread. I primi due thread sommano 1000 numeri generati casualmente ed ogni volta incrementano un  contatore. Il terzo thread attende che il contatore incrementato dai due thread raggiunga un valore limite fornito da riga di comando, notifica l’avvenuta condizione e termina. Utilizzare le variabili condizione.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

struct cond{
	pthread_mutex_t m;
	pthread_cond_t c;
	int counter;
}varC={PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, 0};

void* produttori(void* args){
	
	int* lim=(int*)args;

	int mySum=0;

	for(int i=0; i<1000; i++){
		
		mySum=rand()%100+1;
		pthread_mutex_lock(&varC.m);
		varC.counter+=mySum;
		if(varC.counter==*lim){
			pthread_mutex_unlock(&varC.m);
			pthread_cond_signal(&varC.c);
			pthread_exit(0);		
		}
		pthread_mutex_unlock(&varC.m);
	}
	

pthread_exit(0);
	
}

void* lettore(void* args){
	
	int* lim=(int*)args;
	
	pthread_mutex_lock(&varC.m);
	printf("faccio chekc\n");
	while(varC.counter<*lim)
		pthread_cond_wait(&varC.c, &varC.m);
	pthread_mutex_unlock(&varC.m);
	
	printf("Limite di %d raggiunto.\n", *lim);
pthread_exit(0);
}

int main(int argc, char* argv[]){

	int limite=atoi(argv[1]);
	printf("Programma avviato con limite %d\n\n", limite);

	pthread_t TID[3];

	pthread_create(&TID[0], NULL, produttori, (void*)&limite);
	pthread_create(&TID[1], NULL, produttori, (void*)&limite);
	pthread_create(&TID[2], NULL, lettore, (void*)&limite);
	
	pthread_join(TID[0], NULL); pthread_join(TID[1], NULL); pthread_join(TID[2], NULL);

exit(0);
}
