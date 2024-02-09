#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

int glob = 5, pid = 0, i;

int main() {
    pid = fork();
    glob--;
    fork();
    glob--;

    if (pid > 0){
        pid = fork();
        glob--;
    }
    printf("pid = %d, glob = %d,pid = %d\n", getpid(), glob,pid);

}