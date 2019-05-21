#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>


#define len 5
#define NTHREADS 2

/* Variaveis globais */

pthread_mutex_t x_mutex;
pthread_cond_t x_cond;


typedef struct {
  int * buffer;
  int lastPosition;
  int numberOfInsertions;
  int numberOfReads;
  int fibo1;
  int fibo2;
} Queue;

int ehPrimo(long unsigned int n) {
  int i;
  if(n<=1) return 0;
  if(n==2) return 1;
  if(n%2==0) return 0;
  for(i=3; i< sqrt(n)+1; i+=2) {
    if(n%i==0) return 0;
  }
  return 1;
}

/* Thread Produtora */
void *producer(void * queue){

  int temp;
  Queue *fila = (Queue *) queue;

  while (fila->numberOfInsertions < 25){

    /* Adquire o lock*/
    pthread_mutex_lock(&x_mutex);

    /* Se fila cheia, aguarda sinal do consumidor para continuar produzindo*/
    if(fila->lastPosition == 5){
      pthread_cond_wait(&x_cond, &x_mutex);
    }

    /* adiciona novo valor à fila*/
    if(fila->numberOfInsertions == 0){
      fila->buffer[fila->lastPosition] = fila->fibo1;
      fila->lastPosition++;
    }
    else{
      fila->buffer[fila->lastPosition] = fila->fibo2;
      temp = fila->fibo1;
      fila->fibo1 = fila->fibo2;
      fila->fibo2 = temp + fila->fibo1;
      fila->lastPosition++;
    }

    /* Libera o consumer avisando que há um número no buffer */
    if(fila->lastPosition == 1){
      pthread_cond_broadcast(&x_cond);
    }

    fila->numberOfInsertions++;

    /* Libera o lock*/
    pthread_mutex_unlock(&x_mutex);

  }

  pthread_exit(NULL);

}

/* Thread consumidora */
void *consumer(void * queue){

  	Queue *fila = (Queue *) queue;
    int number, i, temp;

    /* Enquanto houver algo na fila para ser consumido*/
    while(fila->numberOfReads < 25){

      /* Adquire o lock*/
      pthread_mutex_lock(&x_mutex);

        /* Entra em condição de espera caso não haja nenhum numero para ser lido no buffer */
      	if(fila->lastPosition == 0){
          pthread_cond_wait(&x_cond, &x_mutex);
        }

      	number = fila->buffer[0];
        fila->numberOfReads++;
      	fila->lastPosition--;

        /* Libera o producer avisando que há espaço para novos números */
      	if(fila->lastPosition == 4){
          pthread_cond_broadcast(&x_cond);
        }

        /* Atualiza a fila do buffer */
      	for(i = 0; i < fila->lastPosition; i++){
          if(fila->buffer[i] == -1){
            break;
          }
          fila->buffer[i] = fila->buffer[i+1];
        }

      /* Libera o lock */
      pthread_mutex_unlock(&x_mutex);

      if(ehPrimo(number)){
        printf("\n O numero %d é primo \n", number);
      }
      else{
        printf("\n O numero %d n é primo \n", number);
      }

    }

    pthread_exit(NULL);

}

/* Funcao principal */
int main (int argc, char * argv[]){

    /* Variáveis locais */
  pthread_t tid[NTHREADS];
  Queue * fila;
  int i;

  	/* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init(&x_cond, NULL);

  	/* Inicializa a fila com o buffer */
  fila = malloc(sizeof(Queue));
  fila->buffer = malloc(len * sizeof(int));
  fila->lastPosition = 0;
  fila->numberOfInsertions = 0;
  fila->numberOfReads = 0;
  fila->fibo1 = 1;
  fila->fibo2 = 1;

  for(int i = 0; i < 5; i++){
    fila->buffer[i] = -1;
  }

  /* Cria as threads */
  if (pthread_create(&tid[0], NULL, producer, (void*) fila)) {
        printf("--ERRO: pthread_create()\n"); exit(-1); }

  if (pthread_create(&tid[1], NULL, consumer, (void*) fila)) {
        printf("--ERRO: pthread_create()\n"); exit(-1); }

	/* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++)
  {
    pthread_join(tid[i], NULL);
  }

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
	pthread_exit(NULL);
  return 0;
}
