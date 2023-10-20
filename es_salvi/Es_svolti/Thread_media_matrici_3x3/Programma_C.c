/*Si realizzi un programma C che lanci n thread per calcolare il valore
massimo degli elementi delle diagonali principali di un insieme di n matrici
di dimensione 3x3. Ciascuna matrice contiene interi generati pseudocasualmente
in un range da 0 a 100. Il calcolo dei massimi degli elementi di ciascuna diagonale
per ciascuna matrice deve essere effettuato concorrentemente.
Il numero di matrici deve essere fornito in input al programma da riga di comando.
Utilizzare un semaforo binario posix.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t mutex;
int massimo;
int **matrici;

void* func(void* args){
	
	int *ind=(int*)args;

	int max=*(matrici[*ind]);
	int i=1, j=1;
	while(i!=2 && j!=2){
		if(*(matrici[*ind]+i*3+j)>max)
			max=*(matrici[*ind]+i*3+j);
		i++; j++;
	}
	i=0; j=2;
	while(i!=2 && j!=0){
		if(*(matrici[*ind]+i*3+j)>max)
			max=*(matrici[*ind]+i*3+j);
		i++; j--;
	}

	sem_wait(&mutex);
	if(massimo<max)massimo=max;
	sem_post(&mutex);
pthread_exit((void*)0);
	
}

int main(int argc, char* argv[]){
	
	if(argc>2) exit(-1);
	
	sem_init(&mutex, -1, 1);
	srand(310333223);
	int n_matrici=atoi(argv[1]);
	int indici[n_matrici];

	matrici=(int**)malloc(sizeof(int*)*n_matrici);

	for(int i = 0; i < n_matrici; i++){
		indici[i]=i;
		*(matrici+i)=(int*)malloc(sizeof(int)*3*3);
		for(int j = 0; j < 3; j++){
			for(int k = 0; k < 3; k++){
				*(matrici[i]+j*3+k) = rand()%100;
			}
		}
	}
		
	for(int i = 0; i < n_matrici; i++){
		printf("\n");
		for(int j = 0; j < 3; j++){
			printf("\n");
			for(int k = 0; k < 3; k++){
				printf("%d ", *(matrici[i]+j*3+k));
			}
		}
	}
	
	pthread_t tid[n_matrici];
	for(int i=0; i<n_matrici; i++)
		pthread_create(&tid[i], NULL, func, (void*)&indici[i]);

	for(int i=0; i<n_matrici; i++)
		pthread_join(tid[i], NULL);
	
	printf("\nIl massimo e' %d\n", massimo);
	
	exit(0);
}
	
	
