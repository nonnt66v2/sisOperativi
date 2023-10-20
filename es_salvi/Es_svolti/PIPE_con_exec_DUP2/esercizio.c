#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 1024
#define READ 0
#define WRITE 1
/*La exec() non cambia la tavola dei descrittori ma si perdono le
variabili locali: come fare per accedere alla stessa pipe se gli
unici riferimenti comuni erano in variabili locali? (pfd)*/


int main(int argc,char** argv){
	int pfd[2], pid;

	if (pipe(pfd) == -1) {/*creo pipe*/
		perror("pipe() failed)");
		exit(-1);
	}
	if((pid=fork()) < 0 ) {
		perror("fork() failed");
		exit(-2);
	}
	if(pid==0) { /* figlio */
		close(pfd[WRITE]); /*chiudo la scrittura perche leggo*/
		dup2(pfd[READ],0); /*duplico la lettura sul file descriptor 0, standard input*/
		close(pfd[READ]); /*lo chiudo perche l'ho duplicato*/
		execlp("wc","wc","-w",NULL);
		perror("wc fallita");
		exit(-3);
	} else { /* padre */
		close(pfd[READ]); /*chiudo la scrittura perche leggo*/
		dup2(pfd[WRITE],1); /*duplico la scrittura sullo standard output 1*/
		close(pfd[WRITE]); /*chiudo perche duplicato e non mi serve*/
		execlp("ls","ls",NULL);
		perror("ls fallita");
		exit(-4);
	}
exit(0);
}
