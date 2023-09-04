#include<stdlib.h>
#include<stdio.h>

/*
1 - Sem máscara, sem infecção da COVID-19
2 - Com máscara, sem infecção da COVID-19
3 - Com máscara, com infecção da COVID-19
4 - Sem máscara, com infecção da COVID-19
*/

struct graph
{
int estado;
char individuo;
struct graph **adj;
}graph;

struct list
{
struct graph *ref;
int size;
}list;

struct graph *graphCreate(int linhas);
struct list *listInit(int linhas);
int num_lines(char *name_arq);
// void graphAdd(list *L, char individuo, int estado);
void listFree(struct list **L);


struct graph *graphCreate(int linhas)
{
  struct graph *G =(struct graph *)calloc(1, sizeof(struct graph));
  G->adj = (struct graph **)calloc(linhas, sizeof(struct graph *));

  return G; 
}


struct list *listInit(int linhas)
{
  struct list *L =(struct list *)calloc(1, sizeof(struct list));
  L->ref = graphCreate(linhas);
  L->size = linhas;

  return L;
}

int num_lines(char *name_arq){
  
  int linhas=0;
  char referencia[12];  

  FILE *arq;
  arq = fopen(name_arq, "r");

  if(arq == NULL){
    printf("erro..\n");
    return 1;
  } 

  while(fgets(referencia, sizeof(referencia), arq))
    linhas++;
  
  fclose(arq);

return linhas;
}


// void graphAdd(list *L, char individuo, int estado)
// {
//   grafo *aux = L->ref;
//   while(aux->prox != NULL)
//   {
//     aux = aux->prox;
//   }
//   aux->prox = infoCriar(valor);
//   L->size++;
//   return;
// }


void listFree(struct list **L)
{
  struct graph *aux = (*L)->ref;
  struct graph **ref = aux->adj;

  for(int n=0; n < (*L)->size; n++){
    free(ref[n]);
    ref[n] = NULL;
  }
  free(aux);
  free(*L);
  aux = NULL;
  *L = NULL;

}


int main(){

  int linhas = num_lines("banco.csv");
  struct list *L = listInit(linhas); 
  listFree(&L);

return 0;   
}