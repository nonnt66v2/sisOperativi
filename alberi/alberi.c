#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

int glob = 0, pid = 0, i;

int main() {
    pid = fork();
    fork();
    if (pid)
        glob = 2;
    else
        glob--;
    if(!fork())
        glob++;
    for ( i = 1; i < 2; i++) {
        if(glob >1)
            pid = fork();
        if(!pid)
            glob--;
        if(pid)
            glob--;
    }
    glob++;
    printf("glob = %d\n", glob);
}