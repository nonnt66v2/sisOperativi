#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

void handler(int SIGNO){
	if(SIGNO==SIGUSR1)
		printf("Sono stato sbloccato\n");
}

int main(int argc, char* argv[]){
	printf("padre\n");
	signal(SIGUSR1, handler);
	pid_t *pid;
	pid=calloc(argc-1, sizeof(pid_t));

	for(int i=0; i<argc-1; i++){
		if(!pid[i])
			pid[i]=fork();
		if(!pid[i]){
			pause();
			execlp(argv[i+1], argv[i+1], NULL);
			exit(0);
		}
	}	
	
	int stato;
	
	
	if(getpid()%2){
		for(int i=1; i<argc-1; i+=2)
			kill(pid[i], SIGUSR1);
		
		for(int i=0; i<argc-1; i+=2)
			kill(pid[i], SIGABRT);
			
	}
	else{
		for(int i=0; i<argc-1; i+=2)
			kill(pid[i], SIGUSR1);
	
		for(int i=1; i<argc-1; i+=2)
			kill(pid[i], SIGABRT);
	}
	

	
	for(int i=0; i<argc-1; i++){
		waitpid(pid[i], &stato, 0);
		printf("Stato figlio %d: %d\n", i, stato);
	}
	
exit(0);
	
}
