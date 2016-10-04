#include <stdio.h>
#include <stdlib.h>
#include "Adj_List.h"

/*--Inicializa lista ligada--*/
linkedList *createList()
{
	linkedList *L;
	L = (linkedList*)malloc(sizeof(linkedList));
	
	L->first = NULL;
	
	return(L);
}

/*--Inicializa Lista de Adjacencia (vetor de tamanho @lin ligados a listas)--*/
vetNode* createAdjList(int A)
{
	int i=0;
	vetNode *vet;

	
	//aloca o vetor
	vet = MALLOC(vetNode,A);
		
	for(i=0; i<A; i++)
		vet[i].ptr = createList();
	
	return(vet);
}
Grafo* createGraph(int A){
	Grafo *grafo = MALLOC(Grafo, 1);
	grafo->vertices = createAdjList(A);
	grafo->n = A;
	return grafo;
}

/*--Insere na lista ligada ordenadamente--*/
void insertListSorted(linkedList *L, int B, int weight)
{
	listNode *curr, *prv, *p;
	p = (listNode*)malloc(sizeof(listNode));

	p->weight = weight;
	p->B = B;
	if((L->first == NULL)||(B<L->first->B))
	{
		p->next = L->first;
		L->first = p;
	}	
	else
	{
		prv = NULL;
		curr = L->first;
		while((curr != NULL)&&(curr->B <= B))
		{
			if(curr->B == B)
			{
				curr->B = B;
				curr->weight = weight;
				return;
			}
			prv = curr;
			curr = curr->next;	
		}
		
		p->next = curr;
		prv->next = p;
	}
}

/*--remove elemento, usufruindo da ordenacao--*/
void removeList(linkedList *L, int B)
{
	listNode *p, *ant;

	if(B == L->first->B)
	{
		p = L->first;
		L->first = L->first->next;
		free(p);
	}
	else
	{
		ant = NULL;
		p = L->first;
		while((p != NULL)&&(p->B < B))
		{
			ant = p;
			p = p->next;
		}
		
		if((p != NULL)&&(p->B == B))
		{
			ant->next = p->next;
			free(p);
		}
	}
}

/*--Adiciona aresta entre @pA e @pB, com peso @weight--*/
void AA_List(vetNode *vet, int A, int B, int weight, char flag)		//flag: D-direcionada ou G-grafo
{
	insertListSorted(vet[A].ptr, B, weight);
	
	if (flag == 'G')
		insertListSorted(vet[B].ptr, A, weight);
	
}

/*--Imprime vertices adjacentes ao vertice @pvertex respeitando a flag D,G--*/
void printAdjList(vetNode *vet, int len)
{
	int i;
	listNode *p;
	
	for(i=0; i<len; i++)
	{
		printf("%d. ",i);
		
		p = vet[i].ptr->first;		
		while(p != NULL)
		{
			//printf("%d(%d) ", p->B,p->weight);
			printf("%d, ", p->B);
			p = p->next;
		}
			printf("\n");
	}
}

/*--BubbleSort - alg de ordenacao--*/
void bubbleSort(int* vetor, int tam)
{
   int i, j, temp;
 
   for (i = 1; i < tam; i++)
   {
      for (j = 0; j < tam - 1; j++)
	  {
         if (vetor[j] > vetor[j + 1])
		 {
            temp = vetor[j];
            vetor[j] = vetor[j + 1];
            vetor[j + 1] = temp;
         }
      }
   }
}

/*--Procura vertice @pv no na posicao @pos do vetor e retorna verdadeiro ou falso--*/
int searchVertex_AdjList(vetNode *vet, int pos, int v)
{
	listNode *p;
	if(vet[pos].ptr->first == NULL)
		return(FALSE);
	
	p = vet[pos].ptr->first;
	while(p != NULL)
	{
		if(p->B == v)
			return(TRUE);
	}
	
	return(FALSE);
}

/*--Imprime vertices adjacentes ao vertice @pvertex respeitando a flag D,G--*/
void printAVertex_AdjList(vetNode *vet, int len, int vertex, char flag)
{
	listNode *p;
	int i,j=0, count=0;
	int *vector;
	
	if(flag == 'D')
	{
		if(vet[vertex].ptr->first != NULL)
		{
			p = vet[vertex].ptr->first;
			while(p != NULL)
			{
				printf("%d ",p->B);
				p = p->next;
			}
		}
		
		printf("\n");
	}
	
	else
	{	
		for(i=0; i<len; i++)
		{
			if(searchVertex_AdjList(vet,i,vertex))
				count++;
		}
		
		vector = MALLOC(int,count);

		for(i=0; i<len; i++)
		{
			if(searchVertex_AdjList(vet,i,vertex))
			{
				vector[j] = i;
				j++;
			}		
		}
		
		bubbleSort(vector,count);		//ordenacao
		for(i=0; i<count; i++)
			printf("%d ",vector[i]);
	
		printf("\n");
	}
}

/*--Remove aresta entre @pA e @pB da matriz adjacente respeitando a flag D,G--*/
void RA_AdjList(vetNode *vet, int A, int B, int len, char flag)
{
	removeList(vet[A].ptr,B);		
	if(flag == 'G')
		removeList(vet[B].ptr,A);
}

/*--Imprime Lista Ajacente TRANSPOSTA--*/
void printTransposed_List(vetNode *vet, int len)
{
	int i;
	vetNode *aux;
	listNode *p;
	aux = createAdjList(len);
	
	for(i=0; i<len; i++)
	{
		p = vet[i].ptr->first;
		while(p != NULL)
		{
			AA_List(aux,p->B,i,p->weight,'D');
			p = p->next;	
		}
	}
	
	printAdjList(aux,len);
}

/*--Encontra menor peso em uma lista adjacente--*/
void minor_AdjList(vetNode *vet, int len, int *min)
{
	listNode *p;
	int minor, a, b, i;
	
	minor = vet[0].ptr->first->weight;
	a = 0;
	b = vet[0].ptr->first->B;
	
	for(i=0; i<len; i++)
	{
		p = vet[i].ptr->first;
		while(p != NULL)
		{
			if(p->weight < minor)
			{
				minor = vet[i].ptr->first->weight;
				a = i;
				b = vet[i].ptr->first->B;
			}		
		}
	}	
	
	min[0] = a;
	min[1] = b;
}
