/************************************************************************
*						Projeto 1: Coloring Map								*
*			Sistema utilizado para desenvolvimento: Windows				*
*						Disciplina SCC0218								*
*************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Adj_List.h"
#include "utils.h"

#define NUMBER_OF_COLORS 4
#define NO_COLOR -1


#define COEF		3

/*---------------------------------Funcoes--------------------------------------------
------------------------------------------------------------------------------------*/
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

int graphColoringUtil(vetNode *graph, int *vetColor, int length, int v,  char flag, int *possibilitiesVector)
{
    int i, ok=0, ID;
    if (v == length)
        return(1);
    
    for (i=0; i<NUMBER_OF_COLORS; i++) 
    {
        if (flag == 'a')							// sem poda, sem heuristica
        { 
            vetColor[v] = i;
            ok = 1;
            
            listNode *aux;
            aux = graph[v].ptr->first;				// primeiro adjacente a v
            while(aux != NULL)						// enquanto existirem vertices
            {
                if (i == vetColor[aux->B])
                {
 					ok = 0;
                    break;                	
                }
                aux = aux->next;
            }
            										// chama funcao recursivamente para o proximo
            if (ok && graphColoringUtil(graph, vetColor, length, v+1, flag, possibilitiesVector)) 
                return (1);
            
            vetColor[v] = NO_COLOR;					// senao, despinte o vertice
        }
        
        
        else if (flag == 'b' && isSafe(v, graph, vetColor, i))		// verficacao adiante
        {	
            vetColor[v] = i;
            
            if (graphColoringUtil(graph, vetColor, length, v+1, flag, possibilitiesVector)) {
                return (1);
            }
            
            vetColor[v] = NO_COLOR;
        }
    
        
        else if (flag == 'c' || flag == 'd')					//mvr ou maxGrau(se empate)
        {
            if (flag == 'c')
                ID = minorPoss(possibilitiesVector, length);
            
            
            else if (flag == 'd')
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
                
                if (graphColoringUtil(graph, vetColor, length, v+1, flag, possibilitiesVector))
                    return (1);
                
                vetColor[ID] = NO_COLOR;
            }
        }  
    }
    
    return(0);
}

int graphColoring(vetNode *graph, int length, int *vetColor, char flag)
{
    int i;
    int possibilitiesVector[length];												// vetor de possibilidades de um vertice
    for(i=0; i<length; i++)															
    {
        possibilitiesVector[i] = NUMBER_OF_COLORS;									// inicia com todas as possibilidades
    }
    
   	if(graphColoringUtil(graph, vetColor, length, 0, flag, possibilitiesVector))	// chama funcao recursiva
        return(0);																	// nao eh possivel achar uma solucao
    return(1);
}

/*----------------------------------M A I N------------------------------------------
------------------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	char heuristFlag, buffer[2048], lixo, *token;
	int n, i, vertexA, vertexB;


	if(argc == 1)				// nenhum argumento
	{
		printf("O programa precisa do nome do arquivo de entrada como parametro\n");
		return(0);
	}
	if(argc > 2)				// varios argumentos
	{
		printf("O programa aceita um arquivo por vez\n");
		return(0);
	}

	FILE *fileIN = fopen(argv[1], "rt");

	if(!fileIN)					// caso arquivo nao abra
	{
		printf("Parametro incorreto ou arquivo inexistente!\n");
		return(0);
	}


	fscanf(fileIN, "%d %c", &n, &heuristFlag);						// le os vertices e a heuristica
	// printf("n = %d\nflag = %c\n", n, heuristFlag);


	vectorTable = (TranscTable*)malloc((n*COEF)*sizeof(TranscTable));		// aloca o vetor de transcricao
	for(i=0; i<n; i++)			
		idState[i] = (char*)malloc(n*sizeof(char));					// aloca o vetor das palavras

	vetNode *restritionGraph = createAdjList(n);						// cria grafo de restricoes

	
	fscanf(fileIN, "%c", &lixo);									// limpa lixo teclado

	initTable(vectorTable, n*COEF);									// inicializa 
	/* ATRIBUI IDs A CADA ESTADO (vetor idState ~ sumario) */
	while(fgets(buffer, 2048*sizeof(char), fileIN) != NULL)			
	{
		if(buffer[strlen(buffer)-1] == '\n')
			buffer[strlen(buffer)-1] = '\0';

		token = strtok(buffer,":");

		insertTable(vectorTable, token, n*COEF);					// insere paralra inicial ("vertice A") na tabela
	}
		
	rewind(fileIN);													// volta ao inicio do arquivo
	fscanf(fileIN, "%d %c", &n, &heuristFlag);						// joga primeira linha fora
	fscanf(fileIN, "%c", &lixo);									// joga lixo
	/* MONTA GRAFO DE RESTRICOES */
	while(fgets(buffer, 2048*sizeof(char), fileIN) != NULL)			
	{
		if(buffer[strlen(buffer)-1] == '\n')						// caso pegue o enter
			buffer[strlen(buffer)-1] = '\0';						// elimine o
		
		token = strtok(buffer,":");

		vertexA = returnID(vectorTable, token, n*COEF);				// VERTICE 'A' DO GRAFO

		while(token != NULL)										// pego todas as palavras relacionadas a este vertice A
		{
			token = strtok(NULL, ",");
			if(token == NULL)
				break;
			if(token[0] == ' ')
				shiftLeft(token);
			if(token[strlen(token)-1] == '.')
				token[strlen(token)-1] = '\0';

			vertexB = returnID(vectorTable, token, n*COEF);			//VERTICE 'B' DO GRAFO

			AA_List(restritionGraph, vertexA, vertexB, 1, 'G');		// ADICIONO NO GRAFO DE RESTRICA

		}
	}

	//printAdjList(restritionGraph, n);						// imprime grafo (em ids);
	//printAdjListState(restritionGraph, n);				// imprime grafo (em nomes)

	int *vetColor = (int*)malloc(n*sizeof(int));			// crio vetor de cores
	for(i=0; i<n; i++)
		vetColor[i] = NO_COLOR;								// atribuo cor nenhuma a todos

	graphColoring(restritionGraph, n, vetColor, 'd');		// B A C K T R A C K I G 
	printVetColor(vetColor,n);						// imprime saida

	return(1);
}