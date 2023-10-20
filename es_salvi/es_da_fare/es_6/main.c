
/*
 * Esercizio: scrivere un programma che esegua il comando di
shell “ls | sort| grep <pat>” con tre processi distinti
 */
#include <stdio.h>
#include "stdlib.h"
#include "unistd.h"

#define READ 0
#define WRITE 1
#define SIZE 1024

int main(int argc, char *argv[]) {
    char *tmp = argv[1];
    int fp1[2], fp2[2];
    if ((pipe(fp1)) == -1) {
        perror("pipe1");
        exit(-1);
    }
    if ((pipe(fp2)) == -1) {
        perror("pipe1");
        exit(-1);
    }
    pid_t pid1,pid2;
    if((pid1 = fork())<0){
        perror("fork1");
        exit(-2);
    }if((pid2 = fork())<0){
        perror("fork2");
        exit(-3);
    }
    if(pid1==0) {
        close(fp1[READ]);
        dup2(fp1[WRITE],STDOUT_FILENO);
        close(fp1[WRITE]);
        execlp("ls","ls",NULL);
        perror("ls");
        exit(-5);
    }
    else if(pid2 == 0){
        close(fp1[WRITE]);
        dup2(fp1[READ],STDIN_FILENO);
        close(fp1[READ]);
        dup2(fp2[WRITE],STDOUT_FILENO);
        close(fp2[WRITE]);

        execlp("sort","sort",NULL);
        perror("sort");
        exit(-6);
    }
    else{
        close(fp1[WRITE]);
        close(fp1[READ]);
        close(fp2[WRITE]);
        dup2(fp2[READ],STDIN_FILENO);
        close(fp2[READ]);
        execlp("grep","grep",argv[1],NULL);
        perror("grep");
        exit(-7);
    }

}
