#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	int glob=5;
	int pid=0;
	
	pid=fork();
	glob--;

	for(int i=1; i<=glob; i++){
		if(!fork())
			glob--;
		glob--;
	}
	
	printf("mio glob= %d\n", glob);
}
