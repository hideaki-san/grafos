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
struct graph **prox;
}graph;

struct list
{
struct graph *ref;
int size;
}list;

struct graph *graphCreate(int linhas);
struct list *listInit(int linhas);
int num_linhas(char *name_arq);
// void listaAddFinal(list *L, int valor);
// void listaImprimir(list *L);
// void listFree(struct list *L);


struct graph *graphCreate(int linhas)
{
struct graph *G =(struct graph *)calloc(1, sizeof(struct graph));
G->prox = (struct graph **)calloc(linhas, sizeof(struct graph *));

return G; 
}


struct list *listInit(int linhas)
{
struct list *L =(struct list *)calloc(1, sizeof(struct list));
L->size++;

return L;
}

int num_linhas(char *name_arq){
  
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
// void listaAddFinal(list *L, int valor)
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

// void listaImprimir(list *L)
// {
// grafo *aux = L->ref;
// printf("\nTAMANHO: %d\n", L->size);
// while(aux != NULL)
//   {
//   printf("[%d]->", aux->estado);
//   aux = aux->prox;
//   }
// printf("[NULL]\n\n");
// }

// void listFree(struct list *L)
// {
// struct graph *aux = L->ref;
// for(int f = 1; f <= L->size; f++)
//   {
//   L->ref = aux->prox;
//   free(aux);
//   aux = L->ref;
//   }

// free(L);
// L = NULL;
// }

int main(){

  int linhas = num_linhas("banco.csv");

  printf("%d", linhas);

return 0;   
}