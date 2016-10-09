/************************************************************************
*                       Trabalho1: Coloração de Mapas                         *
*           Sistema utilizado para desenvolvimento: Linux               *
*************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Adj_List.h"
#include "utils.h"

#define NUMBER_OF_COLORS 4
#define NO_COLOR -1

/*---------------------------------Funcoes--------------------------------------------
------------------------------------------------------------------------------------*/
int graphColoringUtil(vetNode *graph, int *vetColor, int length, int v,int *possibilitiesVector)
{
    int i, ok=0, ID;
    listNode *aux, *aux2;

    if (v == length)
        return(1);
    
    for (i=0; i<NUMBER_OF_COLORS; i++) 
    {
    
        
        vetColor[v] = i;
        ok = 1;
        
        listNode *aux;
        aux = graph[v].ptr->first;  // primeiro adjacente a v
        while(aux != NULL)  // enquanto existirem vertices
        {
            if (i == vetColor[aux->B])
            {
                ok = 0;
                break;                  
            }
            aux = aux->next;
        }
        /* chama funcao recursivamente para o proximo*/
        if (ok && graphColoringUtil(graph, vetColor, length, v+1,possibilitiesVector)) 
            return (1);
        
        vetColor[v] = NO_COLOR;  // senao, despinte o vertice
    
   
    }    
    return(0);
}

int graphColoring(vetNode *graph, int length, int *vetColor)
{
    int i;
    int possibilitiesVector[length];  // vetor de possibilidades de um vertice
    for(i=0; i<length; i++)                                                         
    {
        possibilitiesVector[i] = NUMBER_OF_COLORS; // inicia com todas as possibilidades
    }
    
    if(graphColoringUtil(graph, vetColor, length, 0,possibilitiesVector))    // chama funcao recursiva
        return(0); // nao eh possivel achar uma solucao
    return(1);
}

/*----------------------------------M A I N------------------------------------------
------------------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    int i, *vetColor;
    Grafo *graph = readGraph(argc, argv);
    vetColor = (int*)malloc(graph->n*sizeof(int)); // crio vetor de cores
    for(i=0; i<graph->n; i++)
        vetColor[i] = NO_COLOR;  //atribuo cor nenhuma a todos

    graphColoring(graph->vertices, graph->n, vetColor);    // B A C K T R A C K I G 
    printVetColor(vetColor,graph->n);                           // imprime saida

    return(1);
}