#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define LEN 100
#define N_THREADS 2

typedef struct {
  int *v;
  int currentPosition;
} v_struct;

void *Increment (void *v){
  v_struct *vector = (v_struct *) v;
  int t;

  while(vector->currentPosition < 100){
    vector->v[vector->currentPosition] += 1;
    vector->currentPosition += 1;
  }

  pthread_exit(NULL);

}

int main () {

  pthread_t tid_sistema[N_THREADS];
  v_struct * v;
  int t = 0;

  v = malloc(sizeof(v_struct));
  v->v = malloc(LEN * sizeof(int));

  v->currentPosition = 0;

  printf("\nPrintando os Valores iniciais do vetor : \n\n");

  for(t = 0; t < 100; t++){
    v->v[t] = 0;
    printf ("%d ", v->v[t]);
  }

  for(t = 0; t < N_THREADS ; t++){
    if (pthread_create(&tid_sistema[t], NULL, Increment, (void*) v)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }

  for (t=0; t<N_THREADS; t++) {
    if (pthread_join(tid_sistema[t], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1);
    }
  }

  printf("\n\nPrintando os Valores finais do vetor : \n\n");
  for(t = 0; t < LEN ; t++){
    printf("%d ", v->v[t]);
  }
  printf("\n\n");
  pthread_exit(NULL);

}
