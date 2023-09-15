#include<stdlib.h>
#include<stdio.h>

struct graph
{
int estado;
char individuo;
struct graph *next;
}graph;

struct list
{
struct graph **adj;
char buffer_vertex[10];
int size_vertex;
int size_edge;
}list;


struct list *listInit(char *name_arq);
struct graph *graphAdd(char individuo, int estado);

int num_lines(char *name_arq);
void graphPopulate(struct list **L, char *name_arq);
void graphPrint(struct list *L);
void conditional(FILE *arq, int a, int b);
void graphTxtGen(struct list *L);
void listFree(struct list **L);


struct list *listInit(char *name_arq)
{
  int lines = num_lines(name_arq);

  FILE *arq = fopen(name_arq, "r");

  if(arq == NULL)
    return 0;

  struct list *L =(struct list *)calloc(1, sizeof(struct list));
  
  int vertex_controller = 0, add_flag = 0;
  char c = fgetc(arq);

  do{

    if((c != ' ') && (c != '\n') && (c != ','))
    {
      for(int n = 0; n < vertex_controller; n++)
      {
          if(L->buffer_vertex[n] == c)
          {
              add_flag = 1;
              n = vertex_controller;
          }
      }

      if(add_flag == 0)
      {
          L->buffer_vertex[vertex_controller] = c;
          L->size_vertex++;
          vertex_controller++;
      }

      add_flag = 0;
    }

    c = fgetc(arq);

  }while(c != EOF);

  L->size_vertex = L->size_vertex/2;
  L->adj = (struct graph **)malloc(L->size_vertex * sizeof(struct graph *));

  for(int n = 0; n < L->size_vertex; n++)
    L->adj[n] =(struct graph *)malloc(sizeof(struct graph));

  vertex_controller = 0;

  for(int n = 0; n < L->size_vertex; n++)
  {
    L->adj[n] = graphAdd(L->buffer_vertex[vertex_controller], L->buffer_vertex[vertex_controller+1]);
    vertex_controller += 2;
  }

  L->size_edge = num_lines(name_arq);

  fclose(arq);
  return L;
}

struct graph *graphAdd(char individuo, int estado){

  struct graph *G =(struct graph *)malloc(sizeof(struct graph));
  G->individuo = individuo;
  G->estado = estado - 48;
  G->next = NULL;

return G;
}

int num_lines(char *name_arq)
{  
  FILE *arq = fopen(name_arq, "r");

  if(arq == NULL)
    return 0;
  
  int lines = 0;
  char line_complete[12];   

  while(fgets(line_complete, sizeof(line_complete), arq))
    lines++;
  
  fclose(arq);

return lines;
}

void graphPopulate(struct list **L, char *name_arq)
{
  if(L == NULL || (*L)->adj == NULL)
    return;

  FILE *arq = fopen(name_arq, "r");  

  if(arq == NULL)
    return;

  struct graph **aux = (*L)->adj, *G, *g, *G_ref, *g_ref;
  char c = fgetc(arq), ref;  

  do{
    if((c != ' ' && c != ',') && (c != '\n'))
    {
      for(int v = 0; v < (*L)->size_vertex; v++)
      {
        G = aux[v];
        if(G->individuo == c)
        {
          c = fgetc(arq); ///,
          c = fgetc(arq); //int
          c = fgetc(arq); //,
          c = fgetc(arq); //char

          G = aux[v];
          g = G;
          ref = c;

          while(g->next != NULL)
            g = g->next;
          
          c = fgetc(arq); //,
          c = fgetc(arq); //int
          
          g->next = graphAdd(ref, c);

          v = (*L)->size_vertex;
        }
      }
    }

    c = fgetc(arq);
  }while(c != EOF);

}

void graphPrint(struct list *L){

  if(L == NULL || L->adj == NULL)
    return;

  struct graph *G = L->adj[0], *g;

  for(int n = 0; n < L->size_vertex; n++){
    
    g = L->adj[n];

    printf("Vertivce:[%c]: ",g->individuo);

    g = g->next;

    if(g == NULL)
      printf("NULL pois nao eh referencia para infeccao");

    while(g != NULL){
      printf("%c,%d  ", g->individuo, g->estado);
      g = g->next;
    }

  printf("\n");
  }
}

/*
0 - Sem máscara, sem infecção da COVID-19
1 - Com máscara, sem infecção da COVID-19
2 - Com máscara, com infecção da COVID-19
3 - Sem máscara, com infecção da COVID-19
*/

void conditional(FILE *arq, int a, int b){

  if((a == 0 && b == 0)||(a == 0 && b == 1)||(a == 1 && b == 0)||(a == 1 && b == 1)){
    fprintf(arq, "== 0%%");
  } 
  
  if((a == 1 && b == 2) || (a == 2 && b == 1)){
    fprintf(arq, "== 25%%");
  }

  if(((a == 0 && b == 2) || (a== 2 && b == 0)) || ((a == 1 && b == 3) || (a == 3 && b == 1))){
    fprintf(arq, "== 50%%");
  }

  if((a == 0 && b == 3) || (a == 3 && b == 0)){
    fprintf(arq, "== 100%%");
  }
}

void graphTxtGen(struct list *L)
{
  if(L == NULL || L->adj == NULL)
    return;

  FILE *txt = fopen("probabilidade.txt", "w");

  if(txt == NULL)
    return;

  fprintf(txt,"[0]Sem máscara, sem infecção da COVID-19\n[1]Com máscara, sem infecção da COVID-19\n[2]Com máscara, com infecção da COVID-19\n[3]Sem máscara, com infecção da COVID-19\n\n");
  
  struct graph *G, *g;
  int state[2], state_controller = 0;

  for(int n = 0; n < L->size_vertex; n++){
    G = L->adj[n];
    g = G->next;

    while(g != NULL){
      fprintf(txt, "VERTICE ANALISADO [%c|%d]: ", G->individuo, G->estado);
      fprintf(txt, "%c,%d  ", g->individuo, g->estado);
      conditional(txt, g->estado, G->estado);
      fprintf(txt, "\n");

      g = g->next;
    }

  }

fclose(txt);
}

void listFree(struct list **L)
{
  struct graph **aux = (*L)->adj;
  struct graph *ref, *ref2;

  for(int n=0; n < (*L)->size_vertex; n++)
  {
    ref = aux[n];
    
    while(ref != NULL){
      ref2 = ref;
      ref = ref->next;
      free(ref2);
      ref2 = NULL;
    }
  }

  free(aux);
  aux = NULL;
  free(*L);
  (*L) = NULL;

}


int main(){

  char name_arq[20] = "banco.csv";

  struct list *L = listInit(name_arq);
  graphPopulate(&L, "banco.csv");

  graphTxtGen(L);
  
  graphPrint(L);
  listFree(&L);

return 0;   
}