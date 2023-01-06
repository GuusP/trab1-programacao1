#include "liblef.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Implementa o TAD Lista de Eventos Futuros (LEF) 
 * Feito em C para a disciplina CI1001 - Programacao 1 
 * Autor: Luis C. E. Bona 
 *
 * Versao 1.0.0 10/11/2021
 * Versao 1.0.1 16/08/2022
 * Versao 2.0.0 14/12/2022
 */

/* A  LEF é uma lista que armazena eventos ordenados pelo membro tempo 
 * do tipo evento_t. Cada evento pode ter um tipo e armazenar em dados 
 * diferentes estruturas de dados que permitem descrever cada evento. */

/*
 * evento_t possui os dados que descrevem os eventos 
 */
/*typedef  struct evento {
    int tempo;   tempo logico do evento 
    int tipo;    inteiro para diferenciar os diferentes tipos de evento 
    int dado1;   um inteiro para guardar dados do evento 
    int dado2;  um inteiro para guardar dados do evento 
} evento_t;
  nodo_t eh uma estrutura interna que representa cada nodo da LEF.
  Externamente o usuario da biblioteca nao vai receber essa estrutura para 
  evitar acesso direto ao ponteiro prox.
 
typedef struct nodo_lef {
    evento_t *evento;
    struct nodo_lef *prox;
} nodo_lef_t;
  Representacao da LEF 
 
typedef struct lef {
    nodo_lef_t *Primeiro;
} lef_t;
*/
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
        
    if(l->Primeiro == NULL)
        return adiciona_inicio_lef(l, evento);
        
    nodo_lef_t *ultimo = l->Primeiro;
    while (ultimo->prox != NULL)
        ultimo = ultimo->prox;

    nodo_lef_t *novo_nodo;
    if(!(novo_nodo = cria_nodo_lef()))
        return 0;
    
    memcpy(novo_nodo->evento, evento, sizeof(evento_t));
    novo_nodo->prox = NULL;
    ultimo->prox = novo_nodo;

    return 1;
}


/*TODO: liberar memória*/
/* 
 * Retorna e retira o primeiro evento da LEF. 
 * A responsabilidade por desalocar
 * a memoria associada eh de quem chama essa funcao.
 */
evento_t *obtem_primeiro_lef (lef_t *l){
    nodo_lef_t *primeiro = l->Primeiro;
    l->Primeiro = l->Primeiro->prox;

    return primeiro->evento;
}
