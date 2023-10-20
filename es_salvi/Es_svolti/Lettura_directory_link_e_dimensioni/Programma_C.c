/*Scrivere un programma C, che presa una direcotry da riga di comando, provveda a scorrerla allo scopo di stampare i nomi di tutti i file puntati dai link simbolici e le loro dimensioni*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
int main(){

	DIR *dp;
	struct dirent *dirp;
	struct stat buf;

	dp=opendir("/home/andrea/Scrivania/Esercizi");
	
	while((dirp=readdir(dp))!=NULL){

		lstat(dirp->d_name, &buf);
		printf("Nome= %s, size= %ld\n", dirp->d_name, buf.st_size);
	
	}
exit(0);
}
