#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define READ 0
#define WRITE 1
#define SIZE 1024

int main() {
    int pfd[2],nread;
    char buf[SIZE];
    pid_t pid;
    if (pipe(pfd) == -1) {
        perror("pipe");
        exit(-1);
    };
    if((pid=fork())<0){
        perror("pid");
        exit(-2);
    }
    if(pid==0){
        close(pfd[WRITE]);
        while((nread=read(pfd[READ],buf,SIZE))!=0){
            printf("il figlio legge: %s",buf);
        }
    }
    else{
        close(pfd[READ]);
        strcpy(buf,"sono tuo padre!");
        write(pfd[WRITE],buf, strlen(buf)+1);
        close(pfd[WRITE]);
    }
}
