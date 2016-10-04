/************************************************************************
 *			  Aluno: Frederico de Oliveira Sampaio     8922100			*
 *                               										*
 *               			Trabalho 1: TAD Grafo              			*
 *            Sistema utilizado para desenvolvimento: Windows			*
 *                          Disciplina SCC0216							*
 *************************************************************************/
#ifndef LIST_ADJ
#define LIST_ADJ

#define MALLOC(a,b) (a*)malloc(b*sizeof(a))

#define TRUE 1
#define FALSE 0

/*---------------------------------------------------*/
/*			        Lista Adjacente                  */
/*---------------------------------------------------*/

/*-----No de Lista ligada-----*/
typedef struct listNode
{
	int B;					//vertice B
	int weight;				//peso da aresta
	struct listNode *next;	
}
listNode;
/*-----------------------------*/

/*-----Lista ligada-----*/
typedef struct
{
	listNode *first;
}
linkedList;
/*----------------------*/

/*-----No do vetor de vertices A-----*/
typedef struct vetNode
{
	linkedList *ptr;
}
vetNode;
/*-----------------------------------*/


/*---------------------------------------------------*/
/*			        Prototipos                       */
/*---------------------------------------------------*/

/*--Inicializa lista ligada--*/
linkedList *createList();

/*--Inicializa Lista de Adjacencia (vetor de tamanho @lin ligados a listas)--*/
vetNode* createAdjList(int A);

/*--Insere na lista ligada ordenadamente--*/
void insertListSorted(linkedList *L, int B, int weight);

/*--remove elemento, usufruindo da ordenacao--*/
void removeList(linkedList *L, int B);

/*--Adiciona aresta entre @pA e @pB, com peso @weight--*/
void AA_List(vetNode *vet, int A, int B, int weight, char flag);

/*--Imprime vertices adjacentes ao vertice @pvertex respeitando a flag D,G--*/
void printAdjList(vetNode *vet, int len);

/*--BubbleSort - alg de ordenacao--*/
void bubbleSort(int* vetor, int tam);

/*--Procura vertice @pv no na posicao @pos do vetor e retorna verdadeiro ou falso--*/
int searchVertex_AdjList(vetNode *vet, int pos, int v);

/*--Imprime vertices adjacentes ao vertice @pvertex respeitando a flag D,G--*/
void printAVertex_AdjList(vetNode *vet, int len, int vertex, char flag);

/*--Remove aresta entre @pA e @pB da matriz adjacente respeitando a flag D,G--*/
void RA_AdjList(vetNode *vet, int A, int B, int len, char flag);

/*--Imprime Lista Ajacente TRANSPOSTA--*/
void printTransposed_List(vetNode *vet, int len);

/*--Encontra menor peso em uma lista adjacente--*/
void minor_AdjList(vetNode *vet, int len, int *min);

#endif
