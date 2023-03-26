#include "liblef.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 
 * Cria uma LEF vazia 
 */
lef_t *cria_lef (){
    lef_t *lista;

    if (!(lista = malloc(sizeof(lef_t))))
        return NULL;

    lista->Primeiro = NULL;
    return lista;
    
}

/* 
 * Destroi a LEF e retorna NULL. 
 */ 
lef_t *destroi_lef (lef_t *l){
    if(!l)
        return NULL;

    while (l->Primeiro)
    {
        nodo_lef_t *prox = l->Primeiro->prox;
        free(l->Primeiro->evento);
        l->Primeiro->evento = NULL;
        l->Primeiro->prox = NULL;
        free(l->Primeiro);
        l->Primeiro = prox;
    }

    free(l);
    return NULL;
}

nodo_lef_t *cria_nodo_lef(){
    nodo_lef_t *nodo;
    
    if(!(nodo = malloc(sizeof(nodo_lef_t))))
        return NULL;

    if(!(nodo->evento = malloc(sizeof(evento_t)))){
        free(nodo);
        nodo = NULL;
        return NULL;
    }

    nodo->prox = NULL;

    return nodo;
}

/* 
 * Adiciona um evento na primeira posicao da LEF. Uma nova copia
 * da estrutura evento sera feita. 
 * Retorna 1 em caso de sucesso, 0 caso contrario.
 */ 
int adiciona_inicio_lef (lef_t *l, evento_t *evento){
    if(l == NULL)
        return 0;
    
    nodo_lef_t *novo_nodo;
    if(!(novo_nodo = cria_nodo_lef()))
        return 0;

    memcpy(novo_nodo->evento, evento, sizeof(evento_t));
    novo_nodo->prox = l->Primeiro;
    l->Primeiro = novo_nodo;
   
    return 1;
}

/* 
 * Adiciona um evento de acordo com o valor evento->tempo na LEF. 
 * Uma nova copia da estrutura evento sera feita 
 * Retorna 1 em caso de sucesso, 0 caso contrario.
 */
int adiciona_ordem_lef (lef_t *l, evento_t *evento){
    if(l == NULL)
        return 0;

    if(l->Primeiro == NULL || evento->tempo < l->Primeiro->evento->tempo)
        return adiciona_inicio_lef(l, evento);
    
    nodo_lef_t *novo_nodo;
   
    if(!(novo_nodo = cria_nodo_lef()))
        return 0;

    memcpy(novo_nodo->evento, evento, sizeof(evento_t));
    novo_nodo->prox = NULL;

    

    nodo_lef_t *atual = l->Primeiro;
    while (atual->prox != NULL && atual->prox->evento->tempo <= novo_nodo->evento->tempo)
        atual = atual->prox;

    nodo_lef_t *temp = atual->prox;
    atual->prox = novo_nodo;
    novo_nodo->prox = temp;

    return 1;
}

/* 
 * Retorna e retira o primeiro evento da LEF. 
 * A responsabilidade por desalocar
 * a memoria associada eh de quem chama essa funcao.
 */
evento_t *obtem_primeiro_lef (lef_t *l){
    if(!l || !l->Primeiro)
        return NULL;
        
    nodo_lef_t *primeiro;
    evento_t *ev;

    primeiro = l->Primeiro;
    ev = primeiro->evento;
    l->Primeiro = l->Primeiro->prox;
    free(primeiro);
    return ev;
}
