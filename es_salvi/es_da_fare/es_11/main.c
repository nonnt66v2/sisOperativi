/*Si  realizzi  un  programma  C  e  Posix  in  ambiente  Linux che,  impiegando  la  libreria  Pthread,
crei  una matrice  di  interi  di  dimensioni  n  x  n  con  n  numero  dispari  fornito  da  riga  di  comando,
che  provveda  a sommare in modo concorrente con quattro thread, gli elementi delle due diagonali, della riga centrale
 e della colonna centrale della matrice e ne determini il massimo da assegnare ad un'opportuna variabile. */

#include <stdio.h>
#include <pthread.h>
#include <apue.h>
#include <error.h>

#define N_THREAD 4
int **mat;
struct attr_thr {
    int somma, numD;
};

void *diag1(struct attr_thr *param) {
    for (int i = 0; i < param->numD; i++) {
        param->somma+=mat[i][i];
    }
    return NULL;
}
void *diag2(struct attr_thr *param) {
    int k=param->numD;
    for (int i = 0; i < param->numD; i++) {
        k--;
        param->somma+=mat[i][k];
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int numD = strtol(argv[1], NULL, 10);
    if (numD % 2 == 0) {
        printf("fornire numero dispari");
        exit(-1);
    }
    mat = (int **)  malloc(numD * sizeof(int));
    for (int i = 0; i < numD; i++) {
        mat[i] = malloc(numD * sizeof(int));
    }
    for (int i = 0; i < numD; i++) {
        for (int j = 0; j < numD; ++j) {
            mat[i][j] = rand() % 50;
        }
    }
    for (int i = 0; i < numD; i++) {
        for (int j = 0; j < numD; ++j) {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
    pthread_t tid[N_THREAD];
    struct attr_thr par_tid[N_THREAD];
    for (int i = 0; i < N_THREAD; i++) {
        par_tid[i].numD = numD;
    }
    if((pthread_create(&tid[0], NULL, (void *) diag1, &par_tid[0]))!=0){
        perror("thread1");
        exit(0);
    }
    if((pthread_create(&tid[1], NULL, (void *) diag2, &par_tid[1]))!=0){
        perror("thread2");
        exit(0);
    }
    void *tret;
    pthread_join(tid[0],&tret);
    pthread_join(tid[1],&tret);
    printf("ris 1: %d\n",par_tid[0].somma);
    printf("ris 2: %d\n",par_tid[1].somma);



}
