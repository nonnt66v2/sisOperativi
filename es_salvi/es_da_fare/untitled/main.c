///connect p1 p2 , p1 riceve stringhe e le legge, p2 le ordina
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1
#define SIZE 1024

int main() {
    int pf[2];
    if ((pipe(pf)) == -1) {
        perror("pipe");
        exit(-1);
    }
    pid_t pid;
    char buf[SIZE];
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(-2);
    }
    if (pid == (pid_t) 0) {
        close(pf[WRITE]);
        dup2(pf[READ], STDIN_FILENO);
        close(pf[READ]);
        if (execlp("sort", "sort", NULL) == -1) {
            perror("sort");
            exit(-1);
        }
    } else {
        close(pf[READ]);
        write(pf[1], "This is a test.\n", 16);
        write(pf[1], "Hello, world.\n", 14);
        write(pf[1], "My dog has fleas.\n", 18);
        write(pf[1], "This program is great.\n", 23);
        write(pf[1], "One fish, two fish.\n", 20);
        close(pf[WRITE]);
        waitpid(pid, NULL, 0);
    }
    return 0;
}
