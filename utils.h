#ifndef UTILS
#define UTILS

#include "Adj_List.h"

#define BLUE 0
#define RED 1
#define YELLOW 2
#define GREEN 3

#define TAMSTATE 	50

/*---------------------------------Estruturas-----------------------------------------
-------------------------------------------------------------------------------------*/
typedef struct transcTable 
{
	int id;						// identificacao do nome
	char state[TAMSTATE];		// nome
	struct transcTable *ptr;	// ponteiro para outro no (caso colisoes)	
}TranscTable;

/*----------------------------Variaveis Globais---------------------------------------
------------------------------------------------------------------------------------*/
extern TranscTable *vectorTable;		// vetor que transcrevera nomes para numeros (id`s)
extern char *idState[TAMSTATE];		// vetor que contem os estados, seu indice eh seu id
extern int ID;						// ids de atribuicao

/*---------------------------------Funcoes--------------------------------------------
------------------------------------------------------------------------------------*/
void printVetColor(const int *vetColor, const int length);
Grafo *readGraph(int argc, char **argv);
#endif