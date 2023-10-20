/*Esercizio: scrivere un programma che esegua il comando di
shell “ls| sort| grep <pat>” con tre processi distinti*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

int main(int argc, char* argv[]){

	int fdp1[2];
	int fdp2[2];
	pid_t pidF1=0, pidF2=0;
	
	pipe(fdp1); pipe(fdp2);

	pidF1=fork();
	if(pidF1) pidF2=fork();

	if(pidF1 && pidF2){
		printf("padre\n");
		close(fdp1[READ]);
		dup2(fdp1[WRITE], 1);
		close(fdp1[WRITE]);
		
		execlp("ls", "ls", NULL);
	}
	else if(!pidF1 && !pidF2){
		printf("secondo figlio\n");
		close(fdp1[WRITE]);
		dup2(fdp1[READ], 0);
		close(fdp1[READ]);
		
		close(fdp2[READ]);
		dup2(fdp2[WRITE], 1);
		close(fdp2[WRITE]);

		execlp("sort", "sort", NULL);
	}
	else if( pidF1 && !pidF2){
		printf("terzo figlio\n");
		close(fdp1[READ]);
		close(fdp1[WRITE]);//chiudo anche tutti gli altri file descriptor

		close(fdp2[WRITE]);
		dup2(fdp2[READ], 0);
		close(fdp2[READ]);
		
		execlp("grep", "grep", argv[1], NULL);
	}
exit(0);
}
