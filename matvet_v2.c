#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

float *mat; //matriz de entrada
float *vet; //vetor de entrada
float *saida; //vetor de saida
int nthreads; //numero de threads

typedef struct{
  int id; //identificador do elemento que a thread ira processar
  int dim; //dimensao das estruturas de entrada
} tArgs;


//funcao que as threads executarao
void * tarefa(void *arg) {
  tArgs *args = (tArgs*) arg;
  for(int i = args->id; i < args->dim; i+=nthreads) {
    for(int j=0; j<(args->dim); j++) {
      saida[i] += mat[i*(args->dim) + j]*vet[j];
    }
  }
  pthread_exit(NULL);
}


int main(int argc, char* argv[]) {
  int dim; //dimensao da matriz de entrada
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
  mat = (float *) malloc(sizeof(float) * dim * dim);
  if(mat == NULL) {printf("erro--malloc\n"); return 2;}
  vet = (float *) malloc(sizeof(float) * dim);
  if(vet == NULL) {printf("erro--malloc\n"); return 2;}
  saida = (float *) malloc(sizeof(float) * dim);
  if(saida == NULL) {printf("erro--malloc\n"); return 2;}

  //inicializacao das estruturas de dados
  for(int i=0; i<dim; i++) {
    for(int j=0; j<dim; j++) {
      mat[i*dim+j] = 1;   //equivalente mat[i][j]
    }
    vet[i] = 1;
    saida[i] = 0;
  }

  //multiplicacao da matriz pelo vetor
  //alocacao das estruturas
  tid = (pthread_t *) malloc(sizeof(pthread_t)*nthreads);
  if(tid==NULL) {puts("erro-malloc"); return 2;}

  args = (tArgs *) malloc(sizeof(tArgs)*nthreads);
  if(args==NULL) {puts("erro-malloc"); return 2;}

  //criacao das threads
  for(int i=0; i<nthreads; i++) {
    (args+i)->id = i;
    (args+i)->dim = dim;
    if(pthread_create(tid+i, NULL, tarefa, (void*) (args+i))) {
      puts("erro--pthread_create");
      break;
    }
  }
  //espera pelo termino das threads
  for(int i=0; i<nthreads; i++) {
    pthread_join(*(tid+i), NULL);
  }
  //exibicao dos resultados
  puts("Vetor de saida:");
  for(int i=0; i<dim; i++) {
    printf("%1.f ", saida[i]);
  }
  puts("");
  
  //liberacao de memoria
  free(mat);
  free(vet);
  free(saida);
  free(args);
  free(tid);

  return 0;
}