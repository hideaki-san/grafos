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
char buffer_vertex[10];
int size_lines;
int size_vertex;
}list;


struct list *listInit(int lines);
struct graph *graphAdd(char individuo);
int num_lines(char *name_arq);
void graphPopulate(struct list **L, FILE *arq);
void graphPrint(struct list *L);
void listFree(struct list **L);

//bool graphConexo(struct list *L);
bool graphLoop(struct list *L);
void graphDegree(struct list *L);
bool graphParalel(struct list *L);
void graphMinMaxDegree(struct list *L);
int graphOrdem(struct list *L);
bool graphSimples(struct list *L);


struct list *listInit(int lines)
{
    struct list *L =(struct list *)calloc(1, sizeof(struct list));
    L->adj = (struct graph **)malloc(lines * sizeof(struct graph *));

    for(int n = 0; n < lines; n++)
        L->adj[n] = NULL;
    
    L->size_lines = lines;

return L;
}

struct graph *graphAdd(char individuo)
{
    struct graph *G =(struct graph *)malloc(sizeof(struct graph));
    G->individuo = individuo;
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

void graphPopulate(struct list **L, FILE *arq)
{    
    if(L == NULL || (*L)->adj == NULL)
        return;
    
    struct graph **aux = (*L)->adj, *ref;
    
    int controller = 0, line = 0, line_controller = 0;
    int vertex_controller = 0, vertex_add = 0;
    char c;
        
    do{
        c = fgetc(arq);
    }while(c != '{');

    do{
        c = fgetc(arq);

        if(c == '\n')
        {
            controller = 0;
            c =fgetc(arq);
            
            if(line_controller == 1)
            {
                line++;
                line_controller = 0;
            }
        }
        
        if((c != ' ') && (c && '-') && (c != '-') && (c != ';') && (c != '{') && (c != '}') && (c != '\n'))
        {
            if(controller == 1)
            {
                ref->next = graphAdd(c);
                ref = ref->next;
            }

           if(controller == 0)
            {
                aux[line] = graphAdd(c);
                ref = aux[line];
                controller = 1;
                line_controller++;
            }

            for(int n = 0; n <= vertex_controller; n++)
            {
                if(c == (*L)->buffer_vertex[n]){
                    vertex_add = 1;
                    n = vertex_controller;
                }
            }

            if(vertex_add != 1)
            {
                (*L)->buffer_vertex[vertex_controller] = c;
                (*L)->size_vertex++;
                vertex_controller++;
            }

            vertex_add = 0;
        }

    }while(c != '}');

}

void graphPrint(struct list *L)
{
    if(L == NULL || L->adj == NULL)
        return;

    struct graph *g;

    printf("Vertices> ");
    for(int n = 0; n < L->size_vertex; n++)
    {
        printf("[%c] ", L->buffer_vertex[n]);
    }

    printf("\n\n");
    
    for(int n = 0; n < L->size_lines; n++)
    {
        g = L->adj[n];

        while(g != NULL)
        {
            printf("\t%c ", g->individuo);
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
    struct graph *ref = (*L)->adj[0], *ref2;

    for(int n=0; n < (*L)->size_lines; n++)
    {
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

//2)
bool graphLoop(struct list *L)
{ 
    if(L == NULL || L->adj == NULL)
        return 0;

    struct graph **aux = L->adj, *G, *g;

    for(int n = 0; n < L->size_lines; n++)
    {
        G = aux[n];
        g = G->next;
        if(G->individuo == g->individuo){
            return 1;
        }
    }
return 0;
}

//3)
bool graphParalel(struct list *L)
{
    if(L == NULL || L->adj == NULL)
        return 0;

    struct graph **aux = L->adj, *G, *g;
    char ref[2];
    int paralel = 0;

    for(int line = 0; line < L->size_lines; line++)
    {
        G = aux[line];
        g = G->next;
        ref[0] = G->individuo;
        ref[1] = g->individuo;

        for(int ln = (line+1); ln < L->size_lines; ln++)
        {
            G = aux[ln];
            g = G->next;

            if((ref[0] == G->individuo && ref[1] == g->individuo) || (ref[0] == g->individuo && ref[1] == G->individuo))
                return 1;
        }
    }
return 0;
}

//4)
void graphDegree(struct list *L)
{
    if(L == NULL || L->adj == NULL)
        return;

    struct graph **aux = L->adj, *G, *g;
    char matriz[L->size_vertex][2];

    for(int line = 0; line < L->size_vertex; line++)
    {
        matriz[line][0] = L->buffer_vertex[line];
        matriz[line][1] = '0';
    }

    for(int line_v = 0; line_v < L->size_vertex; line_v++)
    {
        for(int line_g = 0; line_g < L->size_lines; line_g++)
        {
            G = aux[line_g];
            g = G->next;
            
            if(matriz[line_v][0] == G->individuo || matriz[line_v][0] == g->individuo)
            {
                matriz[line_v][1]++;
                
                if(G->individuo == g->individuo)
                    matriz[line_v][1]++;
            }
        }
    }
    for(int n = 0; n < L->size_vertex; n++)
        printf("VERTICE[%c] - GRAU[%d]\n", matriz[n][0], matriz[n][1] - 48);
}

//5)
void graphMinMaxDegree(struct list *L)
{
    if(L == NULL || L->adj == NULL)
        return;

    struct graph **aux = L->adj, *G, *g;
    char matriz[L->size_vertex][2];
    char min, max, position[2];

    for(int line = 0; line < L->size_vertex; line++)
    {
        matriz[line][0] = L->buffer_vertex[line];
        matriz[line][1] = '0';
    }

    for(int line_v = 0; line_v < L->size_vertex; line_v++)
    {
        for(int line_g = 0; line_g < L->size_lines; line_g++)
        {
            G = aux[line_g];
            g = G->next;
            
            if(matriz[line_v][0] == G->individuo || matriz[line_v][0] == g->individuo)
            {
                matriz[line_v][1]++;
                
                if(G->individuo == g->individuo)
                    matriz[line_v][1]++;
            }
        }
    }

    min = matriz[1][1];
    max = matriz[1][1];

    for(int line = 0; line < L->size_vertex; line++)
    {
        if(matriz[line][1] < min)
        {
            min = matriz[line][1];
            position[0] = line;
        }

        if(matriz[line][1] > max)
        {
            max = matriz[line][1];
            position[1] = line;
        }
    }

printf("Min> [%c] - [%d]\nMax> [%c] - [%d]\n", matriz[position[0]][0], matriz[position[0]][1]-48, matriz[position[1]][0], matriz[position[1]][1]-48);
}

//6)
int graphOrdem(struct list *L)
{
    return L->size_vertex;
}

//7)
bool graphSimples(struct list *L)
{
    if((graphParalel(L) != 0) && (graphLoop(L) != 0))
        return 1;

return 0;
}



int main(){

    int lines = num_lines("completok4.dot");

    struct list *L = listInit(lines);
    
    FILE *arq = fopen("completok4.dot", "r");
    
    if(arq == NULL){
        printf("erro.\n");
        return 1;
    }

    graphPopulate(&L, arq);
    fclose(arq);
    graphPrint(L);

    //2)
    if(graphLoop(L))
        printf("\n- Possui Loop -\n");

    //3)
    if(graphParalel(L))
        printf("\n- Possui arestas paralelas -\n");
    
    //4)
    graphDegree(L);
    
    //5)
    graphMinMaxDegree(L);

    //6)
    printf("\nOrdem do grafo> %d\n", graphOrdem(L));
    
    //7)
    if(graphSimples(L))
        printf("\n - Grafo simples -\n");


    listFree(&L);

return 0;   
}