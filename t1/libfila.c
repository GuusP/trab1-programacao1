#include <stdlib.h>
#include "libfila.h"
#include <stdio.h>
/*struct nodo_f {
    int chave;           lista de numeros inteiros               
    struct nodo_f *prox;   ponteiro para o proximo (obrigatorio)   
    struct nodo_f *prev;   ponteiro para o anterior (uso opcional) 
};
typedef struct nodo_f nodo_f_t;

struct fila {
    nodo_f_t *ini;        ponteiro para o inicio da lista (obrigatorio)       
    nodo_f_t *fim;         ponteiro para o fim da lista (uso opcional)         
    int tamanho;       tamanho da lista (numero de elementos na lista)     
};
typedef struct fila fila_t;
*/


/*
 * Remove todos os elementos da fila, libera espaco e devolve NULL.
 */
fila_t * destroi_fila (fila_t *f){
    if(f == NULL)
        return NULL;

    while(f->ini != NULL){
        nodo_f_t *aux = f->ini->prox;
        f->ini->prox = NULL;
        free(f->ini);
        f->ini = aux;
    }

    free(f);
    return NULL;
}

/*
 * Cria uma fila vazia e a retorna, se falhar retorna NULL.
 */
fila_t * cria_fila (){
    fila_t *fila;

    if(!(fila = malloc(sizeof(fila))))
        return NULL;
    
    fila->ini = NULL;
    fila->fim = NULL;
    fila->tamanho = 0;

    return fila;
}



/*
 * Retorna 1 se a fila esta vazia e 0 caso contrario.
 */
int vazia_fila (fila_t *f){
    if(f->tamanho == 0)
        return 1;

    return 0;
}

/*
 * Retorna o tamanho da fila, isto eh, o numero de elementos presentes nela.
 */
int tamanho_fila (fila_t *f){
    return f->tamanho;
}

/*
 * Cria um nodo e retorna o endereço do nodo caso sucesso e NULL caso contrário
 */
nodo_f_t *criar_nodo(nodo_f_t *fim_fila, int elemento){
    nodo_f_t *aux;

    aux = fim_fila;
    if(!(fim_fila = malloc(sizeof(nodo_f_t))))
        return NULL;

    if(aux != NULL)
        aux->prox = fim_fila;

    fim_fila->prox = NULL;
    fim_fila->elem = elemento;

    return fim_fila;
}

nodo_f_t *destroir_nodo(nodo_f_t *nodo){
    if(nodo == NULL)
        return NULL;

    free(nodo);

    return NULL;
}

/*
 * Insere o elemento no final da fila (politica FIFO).
 * Retorna 1 se a operacao foi bem sucedida e 0 caso contrario.
 */
int insere_fila (fila_t *f, int elemento){
    if(f->ini == NULL){
        if(!(f->ini = criar_nodo(f->fim, elemento)))
            return 0;

        f->fim = f->ini;
        
        return 1;
    }

    if(!(f->fim = criar_nodo(f->fim, elemento)))
        return 0;

    return 1;
}

/*
 * Remove o elemento do inicio da fila (politica FIFO) e o retorna.
 * Retorna 1 se a operacao foi bem sucedida e 0 caso contrario.
 */
int retira_fila (fila_t *f, int *elemento){
    if(!f->ini)
        return 0;
    
    *elemento = f->ini->elem;
    nodo_f_t *novo_inicio = f->ini->prox;
    destroir_nodo(f->ini);
    f->ini = novo_inicio;
    
    return 1;
}

void imprime_fila(fila_t *fila){
    if(fila == NULL)
        return;
        
    nodo_f_t *nodo = fila->ini;
    while (nodo != NULL)
    {
        printf("%d ", nodo->elem);
        nodo = nodo->prox;
    }
}
