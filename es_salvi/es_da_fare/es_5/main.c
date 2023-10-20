/*
 * Esercizio: scrivere un programma che esegua il comando di
shell “ls -R | grep <pat>” dove <pat> è un pattern inserito
dall’utente
 */
#include <stdio.h>
#include <unistd.h>
#include "stdlib.h"

#define READ 0
#define WRITE 1

int main(int argc, char *argv[]) {
    if(argc<2){
        perror("argc");
        exit(-1);
    }
    int pf[2];
    if(pipe(pf)==-1){
        perror("pipe");
        exit(0);
    }
    pid_t pid;

    if((pid=fork())<0) {
        perror("fork");
        exit(-2);
    };
    if(pid==0){
        close(pf[WRITE]);
        dup2(pf[READ],STDIN_FILENO);
        close(pf[READ]);
        execlp("grep","grep",argv[1],NULL);
    }else{
        close(pf[READ]);
        dup2(pf[WRITE],STDOUT_FILENO);
        close(pf[WRITE]);
        execlp("ls","ls","-R",NULL);
    }
}
