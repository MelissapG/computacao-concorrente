#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define NTHREADS 5

/* Variaveis globais */
int x = 0; //contador
sem_t sem_1, sem_2;

/* Thread E */
void *E (void *t) {
  printf("Seja bem-vindo!\n");
  x++;
  if (x==1) {
    sem_post(&sem_1);
  }
  pthread_exit(NULL);
}

/* Thread B */
void *B (void *t) {
  sem_wait(&sem_1);
  printf("Fique a vontade.\n");
  x++;
  if (x==4) {
    sem_post(&sem_2);
  }
  sem_post(&sem_1);
  pthread_exit(NULL);
}

/* Thread C */
void *C (void *t) {
  sem_wait(&sem_1);
  printf("Sente-se por favor.\n");
  x++;
  if (x==4) {
    sem_post(&sem_2);
  }
  sem_post(&sem_1);
  pthread_exit(NULL);
}

/* Thread D */
void *D (void *t) {
  sem_wait(&sem_1);
  printf("Aceita um copo d'agua?\n");
  x++;
  if (x==4) {
    sem_post(&sem_2);
  }
  sem_post(&sem_1);
  pthread_exit(NULL);
}

/* Thread A */
void *A (void *t) {
  sem_wait(&sem_2);
  printf("Volte sempre!\n");
  pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  pthread_t threads[NTHREADS];

  // inicializa os semaforos
  sem_init(&sem_1, 0, 0);
  sem_init(&sem_2, 0, 0);

  /* Cria as threads */
  pthread_create(&threads[4], NULL, A, NULL);
  pthread_create(&threads[3], NULL, B, NULL);
  pthread_create(&threads[2], NULL, E, NULL);
  pthread_create(&threads[1], NULL, C, NULL);
  pthread_create(&threads[0], NULL, D, NULL);

  /* Espera todas as threads completarem */
  for (int i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }
}