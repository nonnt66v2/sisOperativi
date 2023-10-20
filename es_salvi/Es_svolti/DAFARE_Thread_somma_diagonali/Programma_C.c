/*Si  realizzi  un  programma  C  e  Posix  in  ambiente  Linux che,  impiegando  la  libreria  Pthread,  
crei  una matrice  di  interi  di  dimensioni  n  x  n  con  n  numero  dispari  fornito  da  riga  di  comando,
che  provveda  a sommare in modo concorrente con quattro thread, gli elementi delle due diagonali, della riga centrale e della colonna centrale della matrice e ne determini il massimo da assegnare ad un'opportuna variabile. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char* argv){

	if(argv[1]%2==0 || argc>2){ printf("errore\n"); exit(-1);}

	pthread_t TID[4];
	
	for(int i=0; i<4; i++){
		pthread_create(&TID[i], NULL, //VABE MI SCOCCIO PO O FACC CIA
