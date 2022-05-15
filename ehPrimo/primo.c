#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "../timer.h"

int *vet; //vetor de entrada
int *saidaSeq; //vetor de saida seq
int *saidaConc; //vetor de saida conc
long long int dim; //dimensao do vetor
int nthreads; //numero de threads
int contador_global = 0; //variavel compartilhada entre as threads
pthread_mutex_t mutex; //variavel de lock para exclusao mutua

int ehPrimo(int numero) {
  for(int i=2; i<=numero/2; i++) {
    if (numero % i == 0) {
       return 0;
    }
  }
  return 1;
}

void * processaPrimos(void * arg) {
  int contador_local;
  do {
    pthread_mutex_lock(&mutex);
    if (contador_global<dim) {
      contador_local = contador_global;
      contador_global++;
    } else pthread_exit(NULL);
    pthread_mutex_unlock(&mutex);
    if(ehPrimo(vet[contador_local])) {
      saidaConc[contador_local] = sqrt(vet[contador_local]);
    }
    else {
      saidaConc[contador_local] = vet[contador_local];
    }
  } while(contador_global<dim);
  pthread_exit(NULL);
}

int check() {
  for(int i=0; i<dim; i++) {
    if(saidaConc[i] != saidaSeq[i]) {
      return 0;
    }
  }
  return 1;
}


int main(int argc, char *argv[]) {
  double inicio, fim; //tomada de tempo
  pthread_t *tid; //identificadores de thread do sistema

  //confere os parametros de entrada
  if(argc<2) {
    fprintf(stderr, "digite: %s <dimensao do vetor> <numero de threads>\n", argv[0]);
    return 1;
  }
  dim = atoll(argv[1]);
  nthreads = atoi(argv[2]);

 //aloca memoria para os vetores
  vet = (int *) malloc(sizeof(int)*dim);
  if(vet==NULL) {
    fprintf(stderr, "erro--malloc\n");
    return 2;
  }
  saidaSeq = (int *) malloc(sizeof(int)*dim);
  if(saidaSeq==NULL) {
    fprintf(stderr, "erro--malloc\n");
    return 2;
  }
  saidaConc = (int *) malloc(sizeof(int)*dim);
  if(saidaConc==NULL) {
    fprintf(stderr, "erro--malloc\n");
    return 2;
  }

  //preenche o vetor de entrada
  srand(time(0));
  for(int i=0; i<dim; i++) {
    vet[i] = rand() % 1000;
  }

  //seq
  GET_TIME(inicio);
  for(int i=0; i<dim; i++) {
    if (ehPrimo(vet[i]))
      saidaSeq[i] = sqrt(vet[i]);
    else
      saidaSeq[i] = vet[i];
    }
  GET_TIME(fim);
  printf("tempo sequencial: %lf\n", fim-inicio);

  //conc
  GET_TIME(inicio);
  tid = (pthread_t *) malloc(sizeof(pthread_t)*nthreads);
  if(tid == NULL) {fprintf(stderr, "erro--malloc\n"); return 2;}

  //--inicilaiza o mutex (lock de exclusao mutua)
  pthread_mutex_init(&mutex, NULL);

  //cria as threads
  for(int i=0; i <nthreads; i++) {
    if(pthread_create(tid+i, NULL, processaPrimos, NULL)) {
      fprintf(stderr, "erro--pthread_create\n");
      exit(-1);
    }
  }

  //espera pelo término das threads
  for(int i=0; i< nthreads; i++) {
    if(pthread_join(*(tid+i), NULL)) {
      fprintf(stderr, "erro--pthread_join\n");
      exit(-1);
    }
  }

  pthread_mutex_destroy(&mutex);
  GET_TIME(fim);

  printf("tempo concorrente: %lf\n", fim-inicio);

  if(!check())
    printf("FAILED\n");
  else
    printf("SUCESSFULL\n");

  //libera a alocacao dinamica de memoria
  free(vet);
  free(tid);
  free(saidaConc);
  free(saidaSeq);
  return 0;
}