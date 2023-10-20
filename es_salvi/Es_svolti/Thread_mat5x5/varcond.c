/*Si realizzi un programma C che crei n (n<10) thread che cercano di leggere una variabile il cui
contenuto viene impostato ogni 3 secondi, dal processo padre, che genera casualemente un numero
tra 1 e 100. Il processo che riesce a leggere la variabile ne scrive il contenuto in una matrice 5x5 allocata precedentemente.
Una volta riempita la mattrice il programma termina facendo in modo che ogni
thread stampi in output in proprio identificativo seguito dal numero di volte in cui ha letto
la variabile ed uno solo di essi provvede a stampare la matrice. Utilizzare le variabili di condizione.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

struct cond{
	pthread_mutex_t m;
	pthread_cond_t c;
	int variabile;
	int i;
	int disponibile;
	int *matrix;
}varcond={PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, 0, 0, 0};


void* func(void* args){
	
	int n_read=0;

	
	while(1){
		pthread_mutex_lock(&varcond.m);
		while(!varcond.disponibile)
			pthread_cond_wait(&varcond.c, &varcond.m);
		
		if(varcond.i!=25){
			printf("assegna\n");
			*(varcond.matrix+varcond.i++)=varcond.variabile;
			printf("assegna\n");
			n_read++;
			varcond.disponibile--;
		}
		else{
			pthread_mutex_unlock(&varcond.m);
			break;
		}

		pthread_mutex_unlock(&varcond.m);
	}

	printf("sono il thread %ld e ho letto %d volte\n", pthread_self(), n_read);
pthread_exit((void*)0);
}

int main(){
	
	pthread_t mieiTID[5]={0};
	varcond.matrix=(int*)malloc(sizeof(int)*25);
	for(int i=0; i<5; i++)
		pthread_create(&mieiTID[i], NULL, func, NULL);
	
	for(int i=0; i<25; i++){
		sleep(1);
		varcond.variabile=1+rand()%100;
		if(!varcond.disponibile)
			pthread_cond_signal(&varcond.c);
		varcond.disponibile++;
		pthread_mutex_unlock(&varcond.m);
	}

	pthread_cond_broadcast(&varcond.c);
	varcond.disponibile=5;
	printf("li aspetto\n");
	for(int i=0; i<5; i++)
		pthread_join(mieiTID[i], NULL);

	for(int i=0; i<5; i++){
		printf("\n");
		for(int j=0; j<5; j++){
			printf("%d ", *(varcond.matrix+i*5+j));
		}
	}
exit(0);
}
			

