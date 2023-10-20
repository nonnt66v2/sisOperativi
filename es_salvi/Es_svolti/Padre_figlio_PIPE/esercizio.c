#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 1024
int main(int argc, char* argv){
	int pfd[2];
	int nread;
	int pid;
	char buf[SIZE];
	if(pipe(pfd)==-1){
		printf("pipe fallita\n");
		exit(-1);
	}
	if((pid=fork()) < 0){
		perror("fork fallita");
		exit(-2);
	}
	if (pid==0){/*figlio*/
		close(pfd[1]);/*chiuso la scrittura, che non uso*/
		while((nread=read(pfd[0], buf, SIZE))!=0)
			printf("il figlio legge: %s\n", buf);
		close(pfd[0]);
	}
	else{/*padre*/ 
		close(pfd[0]);/*chiudo la lettura, perche scrivo*/
		strcpy(buf, "Sono tuo padre!");
		write(pfd[1], buf, strlen(buf)+1);
		close(pfd[1]);
	}
exit(0);
}
