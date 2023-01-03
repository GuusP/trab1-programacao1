#include "libfila.h"
#include <stdio.h>
int main(){
    fila_t *fila = cria_fila();
    int i;
    for (i = 0; i < 5; i++)
    {
        insere_fila(fila, i);
    }

    nodo_f_t *nodo = fila->ini;
    while (nodo != NULL)
    {
        printf("%d\n", nodo->chave);
        nodo = nodo->prox;
    }
    
    
    return 0;
}