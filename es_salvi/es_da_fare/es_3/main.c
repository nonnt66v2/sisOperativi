/// ls | wc -c
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "stdlib.h"
#include "error.h"
#define READ 0
#define WRITE 1

int main() {
    int pf[2];
    if(pipe(pf)==-1){
        perror("pipe");
        exit(-1);
    }
    pid_t pid;
    if((pid = fork())<0){
        perror("pipe");
        exit(-2);
    }
    if(pid==0){
        close(pf[WRITE]);
        if((dup2(pf[READ],STDIN_FILENO) == -1)){ perror("dup2");exit(-3);}
        if((execlp("wc","wc","-c",NULL))==-1){
            perror("wc");
            exit(-3);
        }
        perror("wc");
        exit(-3);
    }
    else{
        close(pf[READ]);
        dup2(pf[WRITE],STDOUT_FILENO);
        execlp("ls","ls",NULL);
        perror("ls");
        exit(-4);
    }
}
