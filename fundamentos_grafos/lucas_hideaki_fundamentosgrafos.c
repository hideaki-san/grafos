#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

struct graph
{
char individuo;
struct graph *next;
}graph;

struct list
{
struct graph **adj;
char buffer_vertex[30];
int size_vertex;
int size_edge;
}list;


struct list *listInit(char *name_arq);
struct graph *graphAdd(char destino);
int num_lines(char *name_arq);
void graphPopulate(struct list **L, char *name_arq);
void graphPrint(struct list *L);
void listFree(struct list **L);
int fatorial(int fat);

bool graphLoop(struct list *L);
void graphGrau(struct list *L);
bool graphParalelo(struct list *L);
void graphMinMaxGrau(struct list *L);
int graphOrdem(struct list *L);
// bool graphSimples(struct list *L);
// bool graphMultigrafo(struct list *L);
// bool graphCompleto(struct list *L);
// bool graphRegular(struct list *L);


struct list *listInit(char *name_arq)
{
    FILE *arq = fopen(name_arq, "r");

    if(arq == NULL)
        return NULL;

    struct list *L =(struct list *)calloc(1, sizeof(struct list));
    
    L->size_edge = num_lines(name_arq);

    int vertex_controller = 0, add_flag = 0;  
    char c;

    do{
        c = fgetc(arq);
    }while(c != '{');

    do{

        if((c != ' ') && (c != '-') && (c != ';') && (c != '{') && (c != '}') && (c != '\n'))
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

    fclose(arq);

    L->adj = (struct graph **)malloc(L->size_vertex * sizeof(struct graph *));

    for(int n = 0; n < L->size_vertex; n++)
    {
        L->adj[n] =(struct graph *)malloc(sizeof(struct graph));
        L->adj[n]->individuo = L->buffer_vertex[n];
        L->adj[n]->next = NULL;
    }

return L;
}

struct graph *graphAdd(char destino)
{
    struct graph *G =(struct graph *)malloc(sizeof(struct graph));
    G->individuo = destino;
    G->next = NULL;

return G;
}

int num_lines(char *name_arq)
{
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

return lines-3;
}

void graphPopulate(struct list **L, char *name_arq)
{    
    if(L == NULL || (*L)->adj == NULL)
        return;
    
    FILE *arq = fopen(name_arq, "r");

    if(arq == NULL)
        return;

    struct graph **aux = (*L)->adj, *G, *g;
    int buff_controller = 0, add_flag = 0;
    char c, buff[2];
    
    do{
        c = fgetc(arq);
    }while(c != '{');

    do{
        if((c != ' ') && (c != '-') && (c != ';') && (c != '{') && (c != '}') && (c != '\n'))
        {
            if(add_flag == 0)
            {
                buff[buff_controller] = c;
                buff_controller++;
            }

            if(add_flag == 1)
            {
                buff[buff_controller] = c;
                buff_controller = 0;

                for(int n = 0; n < (*L)->size_vertex; n++)
                {
                    G = aux[n];
                    if(G->individuo == buff[0])
                    {
                        g = G;
                        while(g->next != NULL)
                            g = g->next;

                        g->next = graphAdd(buff[1]);
                    }

                    if(G->individuo == buff[1] && (buff[0] != buff[1]))
                    {
                        g = G;
                        while(g->next != NULL)
                            g = g->next;

                        g->next = graphAdd(buff[0]);
                    }
                }
            }
        }

        if(buff_controller == 1)
            add_flag = 1;
        
        if(buff_controller == 0)
            add_flag = 0;


    c = fgetc(arq);

    }while(c != EOF);

    fclose(arq);
}

void graphPrint(struct list *L)
{
    if(L == NULL || L->adj == NULL)
        return;

    struct graph **aux = L->adj, *G, *g;


    for(int n = 0; n < L->size_vertex; n++)
    {
        G = aux[n];
        printf("Vertice[%c]: ", G->individuo);
        g = G->next;

        while(g != NULL)
        {
            printf("%c -> ", g->individuo);
            
            if(g->next == NULL)
                printf("NULL");

            g = g->next;
        }

    printf("\n");
    }
}

void listFree(struct list **L)
{
    if(L == NULL || (*L)->adj == NULL)
        return;
    
    struct graph **aux = (*L)->adj;
    struct graph *ref, *ref2;

      for(int n = 0; n < (*L)->size_vertex; n++)
    {
        ref = aux[n];

        while(ref != NULL)
        {
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

int fatorial(int fat)
{
    for(int n = (fat-1); n > 1; n--)    
        fat *= n;

return fat;
}

//2)
bool graphLoop(struct list *L)
{ 
    if(L == NULL || L->adj == NULL)
        return 0;

    struct graph **aux = L->adj, *G, *g;

    for(int n = 0; n < L->size_vertex; n++)
    {
        G = aux[n];
        g = G->next;
        while(g != NULL)
        {   
            if(G->individuo == g->individuo)
                return 1;

            g = g->next;
        }
    }
return 0;
}

//3)
bool graphParalelo(struct list *L)
{
    if(L == NULL || L->adj == NULL)
        return 0;

    struct graph **aux = L->adj, *G, *g, *g_ref;

    for(int n = 0; n < L->size_vertex; n++)
    {
        G = aux[n];
        g = G->next;
        while(g != NULL)
        {   
            for(g_ref = g->next; g_ref != NULL; g_ref = g_ref->next)
            {
                if(g_ref->individuo == g->individuo)
                    return 1;
            }
        g = g->next;
        }
        
    }
return 0;
}

//4)
void graphGrau(struct list *L)
{
    if(L == NULL || L->adj == NULL)
        return;

    struct graph **aux = L->adj, *G, *g;
    int degree = 0;

    for(int n = 0; n < L->size_vertex; n++)
    {
        G = aux[n];
        g = G->next;
        while(g != NULL)
        {
            degree++;
            g = g->next;
        }
        printf("[%c] grau> %d\n", G->individuo, degree);
        degree = 0;
    }
}

//5)
void graphMinMaxGrau(struct list *L)
{
    if(L == NULL || L->adj == NULL)
        return;

    struct graph **aux = L->adj, *G, *g;
    char min[L->size_vertex], max[L->size_vertex];
    int degrees[2], ref = 0, first_add = 0;
    int max_controller = 0, min_controller = 0;

    for(int n = 0; n < L->size_vertex; n++)
    {
        G = aux[n];
        g = G->next;
        while(g != NULL)
        {
            ref++;
            g = g->next;
        }

        if(first_add == 0)
        {
            degrees[0] = ref;   
            degrees[1] = ref;   
            first_add = 1;
        }

        if(ref < degrees[0])
            degrees[0] = ref;
        
        if(ref > degrees[1])
            degrees[1] = ref;
        
        ref = 0;
    }

    for(int n = 0; n < L->size_vertex; n++)
    {
        G = aux[n];
        g = G->next;
        while(g != NULL)
        {
            ref++;
            g = g->next;
        }

        if(ref == degrees[0])
        {
            min[min_controller] = G->individuo;
            min_controller++; 
        }

        if(ref == degrees[1])
        {
            max[max_controller] = G->individuo;
            max_controller++; 
        }                
    ref = 0;
    }

printf("Grau MIN: %d\tVertices: ", degrees[0]);
for(int n = 0; n < min_controller; n++)
    printf("[%c] ", min[n]);

printf("\n");

printf("Grau MAX: %d\tVertices: ", degrees[1]);
for(int n = 0; n < max_controller; n++)
    printf("[%c] ", max[n]);

printf("\n");
}

//6)
int graphOrdem(struct list *L)
{
    if(L == NULL || L->adj == NULL)
        return 0;

    return L->size_vertex;
}

//7)
bool graphSimples(struct list *L)
{
    if(L == NULL || L->adj == NULL)
        return 0; 

    if(graphParalelo(L) || graphLoop(L))
        return 0;

return 1;
}

//8)
bool graphMultigrafo(struct list *L)
{
    if(L == NULL || L->adj == NULL)
        return 0;

    if(graphParalelo(L) || graphLoop(L))
        return 1;

return 0;
}

//9)
bool graphCompleto(struct list *L)
{
    if(L == NULL || L->adj == NULL)
        return 0;

    if(graphSimples(L))
    {
        if(L->size_edge == fatorial(L->size_vertex)/(fatorial(L->size_vertex - 2)*fatorial(2)))
            return 1;
    }

return 0;
}

//10)
bool graphRegular(struct list *L)
{
    if(L == NULL || L->adj == NULL)
        return 0;

    struct graph **aux = L->adj, *G, *g;
    int ref = 0, ref2 = 0;

    for(int n = 0; n < L->size_vertex; n++)
    {
        G = aux[n];
        g = G->next;
        while(g != NULL)
        {
            ref++;
            g = g->next;
        }

        if(n == 0)
            ref2 = ref;

        if(ref != ref2)
            return 0;
        
        ref = 0;
    }

return 1;
}


int main(){

    char *name_arq = "completok4.dot";

    struct list *L = listInit(name_arq);
   
    graphPopulate(&L, name_arq);
   
    graphPrint(L);

    if(graphLoop(L))
        printf("\n- Possui Loop -\n");

    if(graphParalelo(L))
        printf("\n- Possui arestas paralelas -\n");
    
    graphGrau(L);
    
    graphMinMaxGrau(L);

    printf("\nOrdem do grafo> %d\n", graphOrdem(L));
    
    if(graphSimples(L))
        printf("\n - Grafo simples -\n");

    if(graphMultigrafo(L))
        printf("\n - Eh um multigrafo -\n");

    if(graphCompleto(L))
        printf("\n - Grafo Completo -\n");

    if(graphRegular(L))
        printf("\n - Grafo Regular -\n");


listFree(&L);
return 0;   
}