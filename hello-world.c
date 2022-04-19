#include <stdio.h>
#include <pthread.h>

#define NTHREADS 10

int vetor[NTHREADS];

//funcao que a thread ira executar
void * tarefa(void * arg) {
  int ident = * (int *) arg;
  vetor[ident-1] = ident;
  printf("boas vindas a thread %d!\n", ident);
  pthread_exit(NULL);
}

//funcao principal
int main(void) {
  pthread_t tid[NTHREADS]; //identificador da thread no sistema
  int ident[NTHREADS]; //identificador local da thread
  //cria as threads
  for(int i=1; i<=NTHREADS; i++) {
    ident[i-1] = i;
    if (pthread_create(&tid[i-1], NULL, tarefa, (void *)&ident[i-1])) {
      printf("erro >> pthread_create\n");
    }
  }
  //espera as threads terminarem
  for(int i=0; i<NTHREADS; i++) {
    if (pthread_join(tid[i], NULL)) {
      printf("erro >> pthread_join\n");
    }
  }
  //imprime o vetor de identificadores
  for(int i=0; i<NTHREADS; i++) {
    printf("%d ", vetor[i]);
  }
  printf("\n");
  printf("hello world a thread principal!\n");
  //desvincula o término da main do término do programa
  pthread_exit(NULL);
  return 0;
}