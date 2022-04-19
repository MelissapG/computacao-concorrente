#include <stdio.h>
#include <pthread.h>

#define NTHREADS 2
#define N_ELEMENTS 1000

//vetor de 1000 elementos
int vec[N_ELEMENTS];

//funcao para calcular o quadrado dos elementos
void * squared(void * arg) {
  int *number = (int *) arg;
  for(int i=0; i<(N_ELEMENTS/2); i++) {
    number[i] = number[i]*number[i];
  }
  pthread_exit(NULL);
}

int main(void) {
  pthread_t tid[NTHREADS]; //identificador da thread no sistema
  
  //cria um array com N_ELEMENTS
  for(int i=0; i<N_ELEMENTS; i++) {
    vec[i] = i;
  };
  if(pthread_create(&tid[0], NULL, squared, (void *)&vec[0])) {
    printf("erro >> pthread_create: thread 1\n");
  };
  if(pthread_create(&tid[1], NULL, squared, (void *)&vec[(N_ELEMENTS/2)])) {
    printf("erro >> pthread_create: thread 2\n");
  };
  for(int i=0; i<NTHREADS; i++) {
    if (pthread_join(tid[i], NULL)) {
      printf("erro >> pthread_join\n");
    }
  };
  //printa os valores na tela para conferir se estao corretos
  for(int i=0; i<N_ELEMENTS; i++) {
    printf("%d ", vec[i]);
  };
  printf("\n");
  pthread_exit(NULL);
  return 0;
}