#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

float *mat; //matriz de entrada
float *vet; //vetor de entrada
float *saida; //vetor de saida

typedef struct{
  int id; //identificador do elemento que a thread ira processar
  int dim; //dimensao das estruturas de entrada
} tArgs;


//funcao que as threads executarao
void * tarefa(void *arg) {
  tArgs *args = (tArgs*) arg;
  for(int j=0; j<(args->dim); j++) {
    saida[args->id] += mat[(args->id)*(args->dim) + j]*vet[j];
  }
  pthread_exit(NULL);
}


int main(int argc, char* argv[]) {
  int dim; //dimensao da matriz de entrada
  pthread_t *tid; //indicadores das threads do sistema
  tArgs *args; //identificadores locais das threads e dimensao

  //leitura e avaliacao dos parametros de entrada
  if(argc<2) {
    printf("digite: %s <dimensao da matriz>\n", argv[0]);
    return 1;
  }
  dim = atoi(argv[1]);

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
  tid = (pthread_t *) malloc(sizeof(pthread_t)*dim);
  if(tid==NULL) {puts("erro-malloc"); return 2;}

  args = (tArgs *) malloc(sizeof(tArgs)*dim);
  if(args==NULL) {puts("erro-malloc"); return 2;}

  //criacao das threads
  for(int i=0; i<dim; i++) {
    (args+i)->id = i;
    (args+i)->dim = dim;
    if(pthread_create(tid+i, NULL, tarefa, (void*) (args+i))) {
      puts("erro--pthread_create");
      break;
    }
  }
  //espera pelo termino das threads
  for(int i=0; i<dim; i++) {
    pthread_join(*(tid+i), NULL);
  }
  ////exibicao dos resultados
  // puts("Matriz de entrada:");
  // for(int i=0; i<dim; i++) {
  //   for(int j=0; j<dim; j++) {
  //     printf("%1.f ", mat[i*dim+j]);
  //   }
  //   puts("");
  // }
  // puts("Vetor de entrada:");
  // for(int i=0; i<dim; i++) {
  //   printf("%1.f ", vet[i]);
  // }
  // puts("");
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