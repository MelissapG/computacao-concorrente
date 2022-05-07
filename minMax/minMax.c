#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "../timer.h"

float *vet; //vetor de entrada
long long int dim; //dimensao do vetor
int nthreads; //numero de threads

typedef struct {
  float maior; //maior numero
  float menor; //menor numero
  int id; //identificador do elemento que a thread ira processar
} tArgs;

void * find(void *arg) {
  tArgs *args = (tArgs*) arg;
  long int tBloco = dim/nthreads; //tamanho do bloco de cada thread
  long int inicio = args->id * tBloco; //elemento inicial do bloco da thread
  long int fim; //elemento final (nao processado) do bloco da thread

  if(args->id == nthreads-1) fim = dim;
  else fim = inicio + tBloco; //trata o resto se houver

  args->maior = vet[inicio];
  args->menor = vet[inicio];
  for(int i=inicio; i<fim; i++) {
    if(vet[i] > args->maior) {
      args->maior = vet[i];
    }
    if(vet[i] < args->menor) {
      args->menor = vet[i];
    }
  }
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  double inicio, fim; //tomada de tempo
  pthread_t *tid; //identificadores de thread do sistema
  tArgs *args; //identificadores locais das threads e maior/menor valores
  float maiorSeq, menorSeq; //maior e menor numeros sequencial
  float maiorConc, menorConc; //maior e menor numeros concorrente

  //confere os parametros de entrada
  if(argc<2) {
    fprintf(stderr, "digite: %s <dimensao do vetor> <numero de threads>\n", argv[0]);
    return 1;
  }
  dim = atoll(argv[1]);
  nthreads = atoi(argv[2]);

  //aloca memoria para o vetor
  vet = (float *) malloc(sizeof(float)*dim);
  if(vet==NULL) {
    fprintf(stderr, "erro--malloc\n");
    return 2;
  }

  //preenche o vetor de entrada
  srand(time(0));
  float range = 1000.1;
  for(int i=0; i<dim; i++) {
    vet[i] = ((float)rand()/(float)(RAND_MAX)) * range;
  }

  //sequencial
  GET_TIME(inicio);
  //encontra o menor e maior numero no vetor
  maiorSeq = vet[0];
  menorSeq = vet[0];
  for(int i=0; i<dim; i++) {
    if(vet[i] > maiorSeq) {
      maiorSeq = vet[i];
    }
    if(vet[i] < menorSeq) {
      menorSeq = vet[i];
    }
  }
  GET_TIME(fim);
  printf("tempo sequencial: %lf\n", fim-inicio);
  
  //concorrente
  //aloca memoria para as estruturas
  GET_TIME(inicio);
  tid = (pthread_t *) malloc(sizeof(pthread_t)*nthreads);
  if(tid == NULL) {fprintf(stderr, "erro--malloc\n"); return 2;}
  args = (tArgs *) malloc(sizeof(tArgs)*nthreads);
  if(args==NULL) {fprintf(stderr, "erro--malloc\n"); return 2;}
  //cria as threads
  for(int i=0; i<nthreads; i++) {
    (args+i)->id = i;
    if(pthread_create(tid+i, NULL, find, (void*) (args+i))) {
      fprintf(stderr, "erro--pthread_create\n");
      return 3;
    }
  }
  //espera pelo termino das threads
  maiorConc = vet[0];
  menorConc = vet[0];
  for(int i=0; i<nthreads; i++) {
    if(pthread_join(*(tid+i), NULL)) {
      fprintf(stderr, "erro--pthread_join\n");
      return 3;
    }
    if((args+i)->maior > maiorConc) {
      maiorConc = (args+i)->maior;
    }
    if((args+i)->menor < menorConc) {
      menorConc = (args+i)->menor;
    }
  }
  GET_TIME(fim);
  printf("tempo concorrente: %lf\n", fim-inicio);

  printf("SEQUENCIAL - maior: %f | menor %f\n", maiorSeq, menorSeq);
  printf("CONCORRENTE - maior: %f | menor %f\n", maiorConc, menorConc);

  //libera a alocacao dinamica de memoria
  free(vet);
  free(args);
  return 0;
}