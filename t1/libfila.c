#include <stdlib.h>
#include "libfila.h"
#include <stdio.h>
/*struct nodo_f {
    int elem;
    struct nodo_f *prox;
};
typedef struct nodo_f nodo_f_t;

struct fila {
    nodo_f_t *ini;
    nodo_f_t *fim;
    int tamanho;  numero de elementos na fila 
};
typedef struct fila fila_t;*/


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

    if(!(fila = malloc(sizeof(fila_t))))
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
    return f->ini == NULL;
}

/*
 * Retorna o tamanho da fila, isto eh, o numero de elementos presentes nela.
 */
int tamanho_fila (fila_t *f){
    if(!f)
        return 0;

    return f->tamanho;
}

/*
 * Cria um nodo e retorna o endereço do nodo caso sucesso e NULL caso contrário
 */
nodo_f_t *criar_nodo(){

    nodo_f_t *novo_nodo;

    if(!(novo_nodo = malloc(sizeof(nodo_f_t)))){
        return NULL;
    }
        
    novo_nodo->prox = NULL;
    return novo_nodo;
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
    if(!f)
        return 0;

    if(vazia_fila(f)){
        if(!(f->ini = criar_nodo()))
            return 0;

        f->fim = f->ini;
    }
    else{
        if(!(f->fim->prox = criar_nodo()))
            return 0;
    
        f->fim = f->fim->prox;
    }

    f->tamanho += 1;
    f->fim->elem = elemento;

    return 1;
}

/*
 * Remove o elemento do inicio da fila (politica FIFO) e o retorna.
 * Retorna 1 se a operacao foi bem sucedida e 0 caso contrario.
 */
int retira_fila (fila_t *f, int *elemento){
    if(!f || f->tamanho < 1)
        return 0;
    
    *elemento = f->ini->elem;
    nodo_f_t *novo_inicio = f->ini->prox;
    destroir_nodo(f->ini);
    f->ini = novo_inicio;
    f->tamanho -= 1;
    
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
