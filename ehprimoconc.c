#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

pthread_mutex_t mutex; //variavel de lock para exclusao mutua
long long int currentNumber = 1;
long long int numberOfPrimes = 0;
long long int inp;

void *ehPrimo(void *threadid) {

    long long int i, n;
    int tid = *(int*) threadid;
    int prime = 0;

    while( n <= inp ){

      pthread_mutex_lock(&mutex);

        n = currentNumber;
        currentNumber++;
        if(prime){
          numberOfPrimes++;
        }
        prime = 0;

      pthread_mutex_unlock(&mutex);

      if (n==2){
        prime = 1;
      }

      if( (n % 2) != 0 && n > 1){
        for (i=3; i<sqrt(n)+1; i+=2){
          if(n%i==0) n = 0;
        }
        if(n != 0){
          prime = 1;
        }
      }

    }

    pthread_exit(NULL);
}

int main (int argc, char *argv[]){

    pthread_t *tid;
    int NTHREADS, *id, t;
    double ini, fim;
    char buf[50];

    pthread_mutex_init(&mutex, NULL);

    printf("Digite o limite do intervalo : \n");
    fgets(buf, 20, stdin);

    printf("Digite o número de threads : \n");
    scanf("%d", &NTHREADS);

    inp = atoll(buf);

    tid = malloc(NTHREADS * sizeof(pthread_t)); if (tid == NULL) return -1;
    id = malloc(NTHREADS * sizeof(int)); if(id == NULL) return -1;

    GET_TIME(ini);

    for(t = 0; t < NTHREADS; t++){
      id[t]=t;
      if (pthread_create(&tid[t], NULL, ehPrimo, (void *) &id[t])) {
        printf("--ERRO: pthread_create()\n"); exit(-1);
      }
    }

    for (t=0; t<NTHREADS; t++) {
      if (pthread_join(tid[t], NULL)) {
           printf("--ERRO: pthread_join() \n"); exit(-1);
      }
    }

    pthread_mutex_destroy(&mutex);

    GET_TIME(fim);

    printf("Tempo = %lf\n", fim-ini);
    printf("Quantidade de números primos = %d\n", numberOfPrimes);

    pthread_exit(NULL);
}
