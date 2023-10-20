/*Si scriva un programma C che apra un file di testo passato come argomento da riga di comando
e crei due processi figli e che provvedono a stampare a video, rispettivamente i caratteri di 
posizione pari e dispari. Il primo che termina invia un segnale defini dall'utente all'altro
processo che termina la propria stampa e termina il processo che ha notificato la terminazione e se stesso.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

#define P1 0
#define P2 1

int main(int argc, char* argv[]){

	pid_t pid[2]={0};

	pid[P1]=fork();
	if(pid[P1]) pid[P2]=fork();

	int fd=open(argv[1], O_RDONLY);
	
	if(!pid[P1] && !pid[P2]){//sono P1
		
		char buf;
		off_t off;
		read(fd, &buf, 1);
		do{
			
			printf("processo pari legge= %c\n", buf);
			 
			lseek(fd, 1, SEEK_CUR);
		}while(read(fd, &buf, 1)!=0);
		
		//signal(...
	}
	else if(pid[P1] && !pid[P2]){
		
		lseek(fd, 1, SEEK_SET);
		
		char buf;
		off_t off;
		while(read(fd, &buf, 1)!=0){
			
			printf("processo dispari legge= %c\n", buf);
			 
			lseek(fd, 1, SEEK_CUR);
		}
	}
	waitpid(-1, NULL, 0);
exit(0);
}
