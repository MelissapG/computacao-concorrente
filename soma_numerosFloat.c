#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

long int dim; //dimensao do vetor de entrada
int nthreads; //numero de threads
double *vetor; //vetor de entrada com dimensao dim

//fluxo das threads
void * tarefa(void * arg) {
  long int id = (long int) arg; //identificador da thread
  long int tamBloco = dim/nthreads; //tamanho do bloco de cada thread
  long int ini = id * tamBloco; //elemento inicial do bloco da thread
  long int fim; //elemento final (nao processado) do bloco da thread
  double *somaLocal; //variavel local da soma dos elementos
  
  somaLocal = (double *) malloc(sizeof(double));
  if(somaLocal == NULL) {
    fprintf(stderr, "erro--malloc\n");
    exit(1);
  }
  *somaLocal = 0;

  if(id == nthreads-1) fim = dim;
  else fim = ini + tamBloco; //trata o resto se houver

  //soma os elementos do bloco da thread
  for(long int i=ini; i<fim; i++) {
    *somaLocal += vetor[i]; 
  }
  //retorna o resultado da soma local
  pthread_exit((void *) somaLocal);
}

//fluxo principal
int main(int argc, char *argv[]) {
  double somaSeq=0; //soma sequencial
  double somaConc=0; //soma concorrente
  double *retorno; //valor de retorno das threads
  double inicio, fim; //tomada de tempo
  pthread_t *tid; //identificadores de thread do sistema
  
  //recebe e valida os parametros de entrada (dimensao do vetor, numero de threads) 
  if(argc<3) {
    fprintf(stderr, "digite: %s <dimensao do vetor> <numero de threads>\n", argv[0]); 
    return 1;
  }
  dim = atoi(argv[1]);
  nthreads = atoi(argv[2]);
  
  //aloca o vetor de entrada
  vetor = (double *) malloc(sizeof(double)*dim);
  if(vetor == NULL) {
    fprintf(stderr, "erro--malloc\n");
    return 2;
  }
  //preenche o vetor de entrada
  for(long int i=0; i<dim; i++) {
    vetor[i] = 1000.1/(i+1);
  }

  //soma sequencial dos elementos
  GET_TIME(inicio);
  for(long int i=0; i<dim; i++) {
    somaSeq += vetor[i];
  }
  GET_TIME(fim);
  printf("tempo sequencial: %lf\n", fim-inicio);

  //soma concorrente dos elementos
  GET_TIME(inicio);
  tid = (pthread_t *) malloc(sizeof(pthread_t)*nthreads);
  if(tid == NULL) {
    fprintf(stderr, "erro--malloc\n");
    return 2;
  }
  //cria as threads
  for(long int i=0; i<nthreads; i++) {
    if(pthread_create(tid+i, NULL, tarefa, (void*) i)) {
      fprintf(stderr, "erro--pthread_create\n");
      return 3;
    }
  }
  //aguarda o termino das threads
  for(long int i=0; i<nthreads; i++) {
    if(pthread_join(*(tid+i), (void**) &retorno)) {
      fprintf(stderr, "erro--pthread_join\n");
      return 3;
    }
    somaConc+=*retorno;
    free(retorno);
  }
  GET_TIME(fim);
  printf("tempo concorrente: %lf\n", fim-inicio);
 

  //exibir os resultados
  printf("soma sequencial: %.15lf\n", somaSeq);
  printf("soma concorrente: %.15lf\n", somaConc);

  //libera a memoria alocada
  free(vetor);
  free(tid);
  return 0;
}