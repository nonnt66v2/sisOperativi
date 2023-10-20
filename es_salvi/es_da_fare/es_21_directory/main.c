#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "apue.h"
#include "dirent.h"
int main() {
    struct stat buf;
    struct dirent *dirp;
    DIR* dp;
    char *ptr;
    int size;
    if((dp = opendir(getcwd(ptr,size))) == NULL){
        perror("apertura");exit(8);
    }
    while((dirp = readdir(dp))!= NULL){
        if(lstat(dirp->d_name,&buf) < 0){
            printf("errore");
        }
        if(S_ISLNK(buf.st_mode)){
            printf("trovato il link %s\n",dirp->d_name);
        }
    }
    closedir(dp);
    return 0;



}
