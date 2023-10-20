/*Si realizzi un programam C che crei n (n<10) thread che cercano di leggere una variabile il cui
contenuto viene impostato ogni 3 secondi, dal processo padre, generale casualemente un numero
tra 1 e 100. 
Il processo che riesce a leggere la variabile ne scrive il contenuto in una matrice 5x5 allocata precedentemente.
Una volta riempita la mattrice il programma termina facendo in modo che ogni
thread stampi in output in proprio identificativo seguito dal numero di volte in cui ha letto
la variabile ed uno solo di essi provvede a stampare la matrice. Utilizzare le variabili di condizione.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


sem_t sem_cambiato;
sem_t sem_svegliaPadre;

struct var{
	int var;
	int i;
	int j;
	int matrix[5][5];
}vars={0, 0, 0, 0, 0};

void* func(void* args){
	int n_letti=0;
	int var=0;

while(1){
	sem_wait(&sem_cambiato);


	if(vars.i==5){
		printf("sono il Thread %ld e ho letto %d numeri\n", pthread_self(), n_letti);
		sem_post(&sem_cambiato);		
		pthread_exit(0);
	}

	printf("tid %ld legge %d\n", pthread_self(), var);
	var=vars.var;
	sem_post(&sem_svegliaPadre);
	vars.matrix[vars.i][vars.j++]=var;
	n_letti++;
	if(vars.j==5){vars.i++; vars.j=0;}

}

pthread_exit(0);

}

int main(){
	sem_init(&sem_cambiato, 0, 0);
	sem_init(&sem_svegliaPadre, 0, 1);
	srand(2243453412);

	pthread_t mieiTID[5];

	for(int i=0; i<5; i++)
		pthread_create(&mieiTID[i], NULL, func, NULL);
	
	for(int i=0; i<25; i++){
		sem_wait(&sem_svegliaPadre);

		vars.var=rand()%100+1;

		sem_post(&sem_cambiato);
	}

	for(int j=0; j<4; j++)
		sem_post(&sem_cambiato);
	printf("li aspetto\n");
	for(int i=0; i<5; i++)
		pthread_join(mieiTID[i], NULL);

	for(int i=0; i<5; i++){
		
		for(int j=0; j<5; j++){
			printf("%d ", vars.matrix[i][j]);
		}printf("\n"); 	
	}

exit(0);
}
