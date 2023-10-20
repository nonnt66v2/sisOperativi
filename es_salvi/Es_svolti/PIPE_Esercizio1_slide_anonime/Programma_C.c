/*Esercizio: scrivere un programma che esegua il comando di
shell “ls- R | grep <pat>” dove <pat> è un pattern inserito
dall’utente.
Praticamente fa la lista di quello che c'e' nella directory e lo invia a grep che ritorna 
solo la lista dei file che hanno l'estensione "pat" che tipo e' .exe, .txt etc*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

int main(int argc, char *argv []){
	
	int fd[2];
	pipe(fd);//creo la prima e unica pipe anonima
	
	if(fork()){

		close(fd[READ]);
		dup2(fd[WRITE], 1);//duplica dallo standard output al write della mia pipe
		close(fd[WRITE]);
		
		//ora possiamo fare la exec
		execlp("ls", "ls", "-R", NULL);
	}
	else{
		close(fd[WRITE]);
		dup2(fd[READ], 0);
		close(fd[READ]);
		execlp("grep", "grep", argv[1], NULL);
	}
exit(0);
}
