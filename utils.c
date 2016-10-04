#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

TranscTable *vectorTable;
char *idState[TAMSTATE];
int ID = 0;


// imprime resultado conforme vetor traduzido idState
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