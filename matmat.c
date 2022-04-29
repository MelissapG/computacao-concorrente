#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "timer.h"

float *mat1; //primeira matriz
float *mat2; //segunda matriz
float *saida; //matriz de saida
int nthreads; //numero de threads
int dim; //dimensao das matrizes de entrada
double inicio, fim, delta; //indicadores para os tempos

typedef struct{
  int id; //identificador da linha que a thread ira processar
} tArgs;


void * multiplica(void * arg) {
  tArgs *args = (tArgs*) arg;
  int result;
  for(int i = args->id; i<dim; i+=nthreads) {
    for(int j=0; j<dim; j++) {
      result=0;
      for(int k=0; k<dim; k++) {
        result+=mat1[i*dim+k]*mat2[k*dim+j];
      }
      saida[i*dim+j]=result;
    }
  }
  pthread_exit(NULL);
}

int check() {
  GET_TIME(inicio);
  int result;
  for(int i=0; i<dim; i++) {
    for(int j=0; j<dim; j++) {
      result=0;
      for(int k=0; k<dim; k++) {
        result+=mat1[i*dim+k]*mat2[k*dim+j];
      }
      if(saida[i*dim+j] != result) {
        return 0;
      }
    }
  }
  GET_TIME(fim);
  delta = fim - inicio;
  printf("tempo sequencial: %lf\n", delta);
  return 1;
}

int main(int argc, char* argv[]) {
  pthread_t *tid; //indicadores das threads do sistema
  tArgs *args; //identificadores locais das threads e dimensao

  //leitura e avaliacao dos parametros de entrada
  if(argc<2) {
    printf("digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
    return 1;
  }
  dim = atoi(argv[1]);
  nthreads = atoi(argv[2]);
  if(nthreads > dim) {nthreads=dim;}

  //alocacao de memoria para as estruturas de dados
  mat1 = (float *) malloc(sizeof(float) * dim * dim);
  if(mat1 == NULL) {printf("erro--malloc: matriz 1\n"); return 2;}
  mat2 = (float *) malloc(sizeof(float) * dim * dim);
  if(mat2 == NULL) {printf("erro--malloc: matriz 2\n"); return 2;}
  saida = (float *) malloc(sizeof(float) * dim * dim);
  if(saida == NULL) {printf("erro--malloc: saida\n"); return 2;}

  // seed aleatoria para gerar os numeros randomicos
  srand(time(0));

  //inicializacao das estruturas de dados
  for(int i=0; i<dim; i++) {
    for(int j=0; j<dim; j++) {
      mat1[i*dim+j] = rand() % 100;
      mat2[i*dim+j] = rand() % 100;
      saida[i*dim+j] = 0;
    }
  }

  //multiplicacao das matrizes
  //alocacao das estruturas
  tid = (pthread_t *) malloc(sizeof(pthread_t)*nthreads);
  if(tid==NULL) {puts("erro-malloc: tid"); return 2;}

  args = (tArgs *) malloc(sizeof(tArgs)*nthreads);
  if(args==NULL) {puts("erro-malloc: args"); return 2;}

  GET_TIME(inicio);
  //criacao das threads
  for(int i=0; i<dim; i++) {
    (args+i)->id = i;
    if(pthread_create(tid+i, NULL, multiplica, (void*) (args+i))) {
      puts("erro--pthread_create");
      break;
    }
  }
  //espera pelo termino das threads
  for(int i=0; i<nthreads; i++) {
    pthread_join(*(tid+i), NULL);
  }
  GET_TIME(fim);
  delta = fim - inicio;
  printf("tempo com concorrencia: %lf\n", delta);

  //confere se os valores estão corretos
  int ret = check();
  if(ret) {
    printf("sucessfull: [code %d]\n", ret);
  } else {
    printf("failed: [code %d]\n", ret);
  }
  
  //liberacao de memoria
  free(mat1);
  free(mat2);
  free(saida);
  free(tid);

  return 0;
}