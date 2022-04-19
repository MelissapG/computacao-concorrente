#include <stdio.h>
#include <pthread.h>

#define NTHREADS 2
#define N_ELEMENTS 10000

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

int check() {
  for(int init=0; init<N_ELEMENTS; init++) {
    if(vec[init] != (init*init))
      return 0;
  };
  return 1;
}

int main(void) {
  pthread_t tid[NTHREADS]; //identificador da thread no sistema
  int ret; //retorno da funcao check

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
  //confere se os valores estão corretos 
  ret = check();
  if(ret) {
    printf("sucessfull: [code %d]\n", ret);
  } else {
    printf("failed: [code %d]\n", ret);
  }
  pthread_exit(NULL);
  return 0;
}