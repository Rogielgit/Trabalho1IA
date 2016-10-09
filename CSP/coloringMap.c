/************************************************************************
*						Trabalho 1: Coloração de Mapas					*
*			Sistema utilizado para desenvolvimento: Linux				*
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


// retorna o id do estado que possui menor possibilidade
int minorPoss(int *possibilitiesVector, int length)
{
    int i, menorID = 0, menor = 0;
    
	for(i=0; i<length; i++)
	{
    	if(possibilitiesVector[i] != length+1)
    	{
    		menor = possibilitiesVector[i];
    		menorID = i;
    		break;
    	}
	}

    for (i=0; i<length; i++) 
        if (possibilitiesVector[i] < menor && possibilitiesVector[i] != (length+1)) 
        {
            menor = possibilitiesVector[i];
            menorID = i;
        }

    return(menorID);
}


// retorna o id do estado que possui menor possibilidade e caso exista mais, o que tem mais ligantes adjacentes (> grau)
int minorPossMaxGrade(vetNode *graph, int *possibilitiesVector, int length)
{
    int menor, maiorID, maior, count = 0, *vetorEmpate, i;
    vetorEmpate = (int*)malloc(length*sizeof(int));
    for(i=0; i<length; i++)
    	vetorEmpate[i] = -1;

    menor = minorPoss(possibilitiesVector, length);

    for(i=0; i<length; i++)
    	if(possibilitiesVector[i] == possibilitiesVector[menor])
    		count++;

    if(count <= 1)
    	return(menor);


    for(i=0; i<length; i++)
    {
    	count = 0;
    	if(possibilitiesVector[i] == possibilitiesVector[menor])
    	{
    		listNode *aux = graph[i].ptr->first;
    		while(aux != NULL)
    		{
    			count++;
    			aux = aux->next;
    		}
    		vetorEmpate[i] = count;
    	}
    }

    for(i=0; i<length; i++)
    {
    	if(vetorEmpate[i] != -1)
    	{
    		maior = vetorEmpate[i];
    		maiorID = i;
    	}

    }

    for(i = 0; i<length; i++)
    {
    	if(vetorEmpate[i] > maior)
    	{
    		maior = vetorEmpate[i];
    		maiorID = i;
    	}
    }

    return(maiorID);
}


// verifica se eh possivel pintar o vertice vizinho
int isSafe(int v, vetNode *graph, int *vetColor, int color)
{
    listNode *aux;
    aux = graph[v].ptr->first;				// primeiro adjacente a v
    while(aux != NULL)						// enquanto existirem vertices
    {
        if(color == vetColor[aux->B])		// se o vizinho ja esta pintado
            return(0);						// retorna falso
        aux = aux->next;					// proximo vizinho
    }
    return(1);								//retorna verdadeiro
}

int graphColoringUtil(vetNode *graph, int *vetColor, int length, int v, int *possibilitiesVector)
{


    int i, ok=0, ID;
    listNode *aux, *aux2;

    if (v == length)
        return(1);
    
    for (i=0; i<NUMBER_OF_COLORS; i++) // busca com Verificação adiante,MVR e Grau de desempate
    {
   	                
        ID = minorPossMaxGrade(graph, possibilitiesVector, length);

        listNode *aux;
        aux = graph[ID].ptr->first;
        while (aux != NULL)
        {
            if(possibilitiesVector[aux->B] != (length+1))
            	possibilitiesVector[aux->B]--;
           	aux = aux->next;
        }

        if (isSafe(ID, graph, vetColor, i)) {
            possibilitiesVector[ID] = length+1;
            vetColor[ID] = i;
        
            if (graphColoringUtil(graph, vetColor, length, v+1, possibilitiesVector))
                return (1);
        
            vetColor[ID] = NO_COLOR;
        }
    }  
        
    return(0);
}

int graphColoring(vetNode *graph, int length, int *vetColor)
{
    int i;
    int possibilitiesVector[length];												// vetor de possibilidades de um vertice
    for(i=0; i<length; i++)															
    {
        possibilitiesVector[i] = NUMBER_OF_COLORS;									// inicia com todas as possibilidades
    }
    
   	if(graphColoringUtil(graph, vetColor, length, 0, possibilitiesVector))	// chama funcao recursiva
        return(0);																	// nao eh possivel achar uma solucao
    return(1);
}

/*----------------------------------M A I N------------------------------------------
------------------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	int i, *vetColor;
	Grafo *graph = readGraph(argc, argv);
    vetColor = (int*)malloc(graph->n*sizeof(int));		// crio vetor de cores
    for(i=0; i<graph->n; i++)
        vetColor[i] = NO_COLOR;							// atribuo cor nenhuma a todos

	graphColoring(graph->vertices, graph->n, vetColor);	
	printVetColor(vetColor,graph->n);							// imprime saida

	return(1);
}