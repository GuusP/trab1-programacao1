#include "libfila.h"
#include <stdio.h>
#include <stdlib.h>



int main(){
    fila_t *fila = cria_fila();
    int i;
    for (i = 0; i < 5; i++)
    {
        insere_fila(fila, i);
    }

    imprime_fila(fila);
    printf("\n");

    int elem;
    retira_fila(fila, &elem);
    imprime_fila(fila);

    return 0;
}