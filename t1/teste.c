#include "libfila.h"
#include "liblef.h"
#include <stdio.h>
#include <stdlib.h>



int main(){
    /*fila_t *fila = cria_fila();
    int i;
    for (i = 0; i < 5; i++)
    {
        insere_fila(fila, i);
    }

    imprime_fila(fila);
    printf("\n");

    int elem;
    retira_fila(fila, &elem);
    imprime_fila(fila);*/

    lef_t *l;
    l = cria_lef();

    evento_t *e;
    e = malloc(sizeof(evento_t));
    e->dado1 = 1;
    e->dado2 = 2;
    e->tempo = 3;
    e->tipo = 4;
    evento_t *j;
    j = malloc(sizeof(evento_t));
    j->dado1 = 5;
    j->dado2 = 12;
    j->tempo = 7;
    j->tipo = 8;
    evento_t *p;
    p = malloc(sizeof(evento_t));
    p->dado1 = 9;
    p->dado2 = 10;
    p->tempo = 11;
    p->tipo = 12;


    adiciona_inicio_lef(l, e);
    adiciona_inicio_lef(l, j);
    adiciona_inicio_lef(l, p);

    while (l->Primeiro != NULL)
    {
        evento_t *primeiro;
        primeiro = obtem_primeiro_lef(l);
        printf("%d ", primeiro->dado1);
        printf("%d ", primeiro->dado2);
        printf("%d ", primeiro->tempo);
        printf("%d ", primeiro->tipo);
        printf("\n");
    }
    
    return 0;
}