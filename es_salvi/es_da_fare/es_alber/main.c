#include <stdio.h>
#include <unistd.h>

int main() {
    int glob = 6,pid;
    if ((pid = fork())){
        fork();
        glob--;
    }
    fork();
    glob--;
    if(pid) {
        pid = fork();
        glob--;
    }
    printf("padre -> %d figlio -> %d glob -> %d\n",getppid(),getpid(),glob);
}