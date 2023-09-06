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
struct graph *next;
}graph;

struct list
{
struct graph **adj;
int size;
}list;


struct list *listInit(int lines);
int num_lines(char *name_arq);
struct graph *graphAdd(char individuo, int estado);
void listFree(struct list **L);
void graphPrint(struct list *L);


struct list *listInit(int lines)
{
  struct list *L =(struct list *)calloc(1, sizeof(struct list));
  L->adj = (struct graph **)malloc(lines * sizeof(struct graph *));

  for(int n = 0; n < lines; n++)
    L->adj[n] = NULL;
  
  L->size = lines;
  return L;
}


int num_lines(char *name_arq){
  
  int lines=0;
  char line_complete[12];  

  FILE *arq;
  arq = fopen(name_arq, "r");

  if(arq == NULL){
    printf("erro..\n");
    return 1;
  } 

  while(fgets(line_complete, sizeof(line_complete), arq))
    lines++;
  
  fclose(arq);

return lines;
}

struct graph *graphAdd(char individuo, int estado){

  struct graph *G =(struct graph *)malloc(sizeof(struct graph));
  G->individuo = individuo;
  G->estado = estado;
  G->next = NULL;

return G;
}

void graphPrint(struct list *L){

  if(L == NULL || L->adj[0] == NULL)
    return;

  struct graph *g;

  for(int n = 0; n < L->size; n++){
    g = L->adj[n];
    while(g != NULL){
      printf("ind = %c, est = %d - ", g->individuo, g->estado);
      g = g->next;
    }
  
  printf("\n");
  }
}


void listFree(struct list **L)
{
  struct graph **aux = (*L)->adj;
  struct graph *ref = (*L)->adj[0]->next, *ref2;

  for(int n=0; n < (*L)->size; n++){
    while(ref != NULL){
      ref2 = ref;
      ref = ref->next;
      free(ref2);
      ref2 = NULL;
    }
  }

  free((*L)->adj);
  free(*L);
  (*L) = NULL;

}


int main(){

  int lines = num_lines("banco.csv");

  struct list *L = listInit(lines);
  
  struct graph **aux, *ref;
   
  char line_complete[10], buff[2], c;
  int character_pos = 0, controller = 0;
  int line_controller = 0, new_insertion = 0;

  FILE *arq = fopen("banco.csv", "r");
  if(arq == NULL){
    printf("erro.\n");
    return 1;
  }

  while(fgets(line_complete, sizeof(line_complete), arq)){      
    c = line_complete[character_pos];

    do{
      if(c != ','){
        buff[controller] = c;
        controller++;

        if(controller > 1 && new_insertion == 0){
          aux[line_controller] = graphAdd(buff[0], buff[1]);
          controller = 0;
          new_insertion = 1;
          printf("%c%c -> ", buff[0], buff[1]);
        }
        if(controller > 1 && new_insertion == 1){
          ref = aux[line_controller];
          ref->next = graphAdd(buff[0], buff[1]);
          ref = ref->next;
          printf("%c%c -> ", buff[0], buff[1]);
          controller = 0;
        }
      }
    character_pos++;
    c = line_complete[character_pos];  
    }while(c != '\n' && c != '\0');
  
    line_controller++;
    new_insertion = 0;
    character_pos = 0;
    }

  
  fclose(arq);
  graphPrint(L);
  listFree(&L);

return 0;   
}