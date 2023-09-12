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
int size;
}list;


struct list *listInit(int lines);
struct graph *graphAdd(char individuo, int estado);
int num_lines(char *name_arq);
void graphPopulate(struct list **L, FILE *arq);
void graphPrint(struct list *L);
void conditional(FILE *arq, int a, int b);
void graphTxtGen(struct list *L);
void listFree(struct list **L);


struct list *listInit(int lines)
{
  struct list *L =(struct list *)calloc(1, sizeof(struct list));
  L->adj = (struct graph **)malloc(lines * sizeof(struct graph *));

  for(int n = 0; n < lines; n++)
    L->adj[n] = NULL;
  
  L->size = lines;
  return L;
}

struct graph *graphAdd(char individuo, int estado){

  struct graph *G =(struct graph *)malloc(sizeof(struct graph));
  G->individuo = individuo;
  G->estado = estado - 48;
  G->next = NULL;

return G;
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


void graphPopulate(struct list **L, FILE *arq){
   
  if(L == NULL || (*L)->adj == NULL)
    return;
  
  struct graph **aux = (*L)->adj, *ref;
  
  char line_complete[1024], buff[2], c;
  int character_pos = 0, buff_controller = 0;
  int line_controller = 0, new_insertion = 0;
 
  while(fgets(line_complete, sizeof(line_complete), arq)){      
    c = line_complete[character_pos];

    do{

      if(c != ',' && c != ' '){
        buff[buff_controller] = c;
        buff_controller++;

        if(buff_controller > 1 && new_insertion == 0){
          aux[line_controller] = graphAdd(buff[0], buff[1]);
          ref = aux[line_controller];
          buff_controller = 0;
          new_insertion = 1;
        }

        if(buff_controller > 1 && new_insertion == 1){
          ref->next = graphAdd(buff[0], buff[1]);
          ref = ref->next;
          buff_controller = 0;
        }   
      }

      character_pos++;
      c = line_complete[character_pos]; 

    }while(c != '\n' && c != '\0');
  
    line_controller++;
    new_insertion = 0;
    character_pos = 0;
    }
}

void graphPrint(struct list *L){

  if(L == NULL || L->adj == NULL)
    return;

  struct graph *g;

  for(int n = 0; n < L->size; n++){
    g = L->adj[n];
    while(g != NULL){
      printf("%c,%d  ", g->individuo, g->estado);
      g = g->next;
    }
  printf("\n");
  }
}

/*
1 - Sem máscara, sem infecção da COVID-19
2 - Com máscara, sem infecção da COVID-19
3 - Com máscara, com infecção da COVID-19
4 - Sem máscara, com infecção da COVID-19
*/

void conditional(FILE *arq, int a, int b){

  if((a == 1 && b == 1)||(a == 1 && b == 2)||(a == 2 && b == 1)||(a == 2 && b == 2)){
    fprintf(arq, "== 0%%");
  } 
  
  if((a == 2 && b == 3) || (a == 3 && b == 2)){
    fprintf(arq, "== 25%%");
  }

  if(((a == 1 && b == 3) || (a== 3 && b == 1)) || ((a == 2 && b == 4) || (a == 4 && b == 2))){
    fprintf(arq, "== 50%%");
  }

  if((a == 1 && b == 4) || (a == 4 && b == 1)){
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

  struct graph *g;
  int state[2], state_controller = 0;

  for(int n = 0; n < L->size; n++){
    g = L->adj[n];
    while(g != NULL){
      fprintf(txt, "%c,%d  ", g->individuo, g->estado);

      if(state_controller < 2){
        state[state_controller] = g->estado;
        state_controller++;
      }

      g = g->next;
    }
  if(state_controller == 2)
    conditional(txt, state[0], state[1]);
  
  state_controller = 0;
  fprintf(txt, "\n");
  }

fclose(txt);
}

void listFree(struct list **L)
{
  struct graph **aux = (*L)->adj;
  struct graph *ref = (*L)->adj[0], *ref2;

  for(int n=0; n < (*L)->size; n++){
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

  int lines = num_lines(name_arq);

  struct list *L = listInit(lines);
  
  FILE *arq = fopen(name_arq, "r");
  
  if(arq == NULL){
    printf("erro.\n");
    return 0;
  }

  graphPopulate(&L, arq);
  fclose(arq);
  
  graphPrint(L);
  graphTxtGen(L);
  listFree(&L);

return 0;   
}