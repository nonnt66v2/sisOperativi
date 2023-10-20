#include <stdio.h>
#include <sys/stat.h>
#include <mqueue.h>
#include "unistd.h"
#include "stdlib.h"
#include "sys/types.h"
#define FIFO_NAME "/home/nonnt66/Scaricati/uni/so/es_salvi/es_da_fare/es_7/cmake-build-debug/my_fifo"
int main() {
    int res = mkfifo(FIFO_NAME,0777);
    if(res==0){
        printf("fifo creata");
        exit(0);
    }
    int res1 = open(FIFO_NAME,O_RDONLY);
    printf("res: %d",res1);
}
