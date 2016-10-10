#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Adj_List.h"

TranscTable *vectorTable;
char *idState[TAMSTATE];
int ID = 0;

#define COEF 3


//imprime resultado conforme vetor traduzido idState
void printVetColor(const int *vetColor, const int length)
{
    int i;
    for(i=0; i<length; i++)
    {
        printf("%s:", idState[i]);
        if(vetColor[i] == BLUE)
            printf(" Azul.\n");
        else if(vetColor[i] == RED)
            printf(" Vermelho.\n");
        else if(vetColor[i] == YELLOW)
            printf(" Amarelo.\n");
        else if(vetColor[i] == GREEN)
            printf(" Verde.\n");
    }
}
// funcao hashing para termos acesso direto aos nomes
int hashing(char *word, int length)     
{
    int i, sum=0;
    for(i=0; i<strlen(word); i++)
    {
        sum = sum + word[i];
    }
    return(sum%length);
}

// inicializa tabela de transcricao
void initTable(TranscTable *table, int length)
{
    int i;
    
    for(i=0; i<length; i++)
    {
        table[i].id = -1;                       
        table[i].ptr = NULL;
    }
}

// insere um elemento na tabela de transcricao, atribuindo seu
// ID e passando para o vetor traduzido idState
void insertTable(TranscTable *table, char *state, int length)           
{                                                                       
    TranscTable *aux = &table[hashing(state, length)];

    if(aux->id == -1)
    {
        strcpy(aux->state, state);
        aux->id = ID++; 

        strcpy(idState[aux->id],state); 
    }

    else
    {
        TranscTable *node = (TranscTable*)malloc(sizeof(TranscTable));
        strcpy(node->state, state);
        node->id = ID++;
        node->ptr = NULL;

        while(aux->ptr != NULL)
        {
            aux = aux->ptr; 
        }

        aux->ptr = node;

        strcpy(idState[node->id],state);
    }
}

// elimina espaco inicial da string
void shiftLeft(char *name)                                              
{
    int i;
    for(i=1; i<strlen(name); i++)
        name[i-1] = name[i];
    name[strlen(name)-1] = '\0';
}

// retorna o ID de certa palavra
int returnID(TranscTable *table, char *state, int length)               
{       
    TranscTable *aux = &table[hashing(state, length)];
    if(!strcmp(state,aux->state))
        return(aux->id);
    
    while(aux->ptr && strcmp(state,aux->state))
        aux = aux->ptr;
    
    return(aux->id);
}

Grafo *readGraph(int argc, char **argv){
    char heuristFlag, buffer[2048], lixo, *token;
    int n, i, vertexA, vertexB;


    if(argc == 1)               // nenhum argumento
    {
        printf("O programa precisa do nome do arquivo de entrada como parametro\n");
        return(0);
    }
    if(argc > 2)                // varios argumentos
    {
        printf("O programa aceita um arquivo por vez\n");
        return(0);
    }

    FILE *fileIN = fopen(argv[1], "rt");

    if(!fileIN)                 // caso arquivo nao abra
    {
        printf("Parametro incorreto ou arquivo inexistente!\n");
        return(0);
    }


    fscanf(fileIN, "%d %c", &n, &heuristFlag);                      // le os vertices e a heuristica
    // printf("n = %d\nflag = %c\n", n, heuristFlag);


    vectorTable = (TranscTable*)malloc((n*COEF)*sizeof(TranscTable));       // aloca o vetor de transcricao
    for(i=0; i<n; i++)          
        idState[i] = (char*)malloc(n*sizeof(char));                 // aloca o vetor das palavras

    Grafo *restrictionGraph = createGraph(n);                        // cria grafo de restricoes

    
    fscanf(fileIN, "%c", &lixo);                                    // limpa lixo teclado

    initTable(vectorTable, n*COEF);                                 // inicializa 
    /* ATRIBUI IDs A CADA ESTADO (vetor idState ~ sumario) */
    while(fgets(buffer, 2048*sizeof(char), fileIN) != NULL)         
    {
        if(buffer[strlen(buffer)-1] == '\n')
            buffer[strlen(buffer)-1] = '\0';

        token = strtok(buffer,":");

        insertTable(vectorTable, token, n*COEF);                    // insere paralra inicial ("vertice A") na tabela
    }
        
    rewind(fileIN);                                                 // volta ao inicio do arquivo
    fscanf(fileIN, "%d %c", &n, &heuristFlag);                      // joga primeira linha fora
    fscanf(fileIN, "%c", &lixo);                                    // joga lixo
    /* MONTA GRAFO DE RESTRICOES */
    while(fgets(buffer, 2048*sizeof(char), fileIN) != NULL)         
    {
        if(buffer[strlen(buffer)-1] == '\n')                        // caso pegue o enter
            buffer[strlen(buffer)-1] = '\0';                        // elimine o
        
        token = strtok(buffer,":");

        vertexA = returnID(vectorTable, token, n*COEF);             // VERTICE 'A' DO GRAFO

        while(token != NULL)                                        // pego todas as palavras relacionadas a este vertice A
        {
            token = strtok(NULL, ",");
            if(token == NULL)
                break;
            if(token[0] == ' ')
                shiftLeft(token);
            if(token[strlen(token)-1] == '.')
                token[strlen(token)-1] = '\0';

            vertexB = returnID(vectorTable, token, n*COEF);         //VERTICE 'B' DO GRAFO

            AA_List(restrictionGraph->vertices, vertexA, vertexB, 1, 'G');     // ADICIONO NO GRAFO DE RESTRICA

        }
    }

    //printAdjList(restritionGraph, n);                     // imprime grafo (em ids);
    //printAdjListState(restritionGraph, n);                // imprime grafo (em nomes)

    
    return restrictionGraph;
}