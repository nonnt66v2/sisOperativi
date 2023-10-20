/*Implementare in C un programma che esegua la seguente pipeline:
cat file.txt | tr'[[:space:]] [[:punct:]]''\n' | sort | uniq*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define R 0
#define W 1

int main(){
	
	int fdp1[2], fdp2[2], fdp3[2];
	pipe(fdp1); pipe(fdp2); pipe(fdp3);

	pid_t pid[3]={0};
	
	pid[0]=fork();
	if(pid[0]) pid[1]=fork();
	if(pid[1]) pid[2]=fork();

	if(pid[0] && pid[1] && pid[2]){//sono padre
		
		close(fdp1[R]);
		dup2(fdp1[W], 1);
		close(fdp1[W]);
	
		execlp("cat", "cat", "file.txt", NULL);
	}
	else if(!pid[0] && !pid[1] && !pid[2]){//sono figlio
		close(fdp1[W]);
		dup2(fdp1[R], 0);
		close(fdp1[R]);

		close(fdp2[R]);
		dup2(fdp2[W], 1);
		close(fdp2[W]);
		
		execlp("tr", "tr", "'[[:space:]] [[:punct:]]'", "'\n'", NULL);
	}
	else if(pid[0] && !pid[1] && !pid[2]){//secondo figlio
		close(fdp1[R]);
		close(fdp1[W]);
		close(fdp2[W]);
		
		close(fdp2[W]);
		dup2(fdp2[R], 0);
		close(fdp2[R]);
		
		//close(fdp3[R]);
		//dup2(fdp3[W], 1);
		//close(fdp3[W]);
		
		execlp("sort", "sort", NULL);
	}
	else if(pid[0] && pid[1] && !pid[2]){//terzo figlio
		close(fdp1[R]);
		close(fdp1[W]);
		close(fdp2[R]);
		close(fdp2[W]);
		
		close(fdp3[W]);
		dup2(fdp3[R], 0);
		close(fdp3[R]);
		
		execlp("uniq", "uniq", NULL);
	}
exit(0);
}
