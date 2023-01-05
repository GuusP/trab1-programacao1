#include <stdio.h>
#include <stdlib.h>
#include "libconjunto.h"
#define MAX 100


int busca_binaria(int *v, int x, int tam){
    int inicio = 0;
    int fim = tam - 1;
    int meio;

    while (fim >= inicio)
    {
        
        meio = (fim + inicio) / 2;

        if (v[meio] == x)
            return meio;

        if (v[meio] > x)
            fim = meio - 1;
        else
            inicio = meio + 1;

    }

    return -1;
    
}

void ordenar_vetor(int *v, int tam){
    int i;
    int aux;

    for (i = 0; i < tam; i = i + 1){
        aux = v[i];

        int j = i - 1;
        while(j >= 0 && v[j] > aux){
            v[j+1] = v[j];
            j = j - 1;
        }

        v[j+1] = aux;
    }
}

/*
 * Remove todos os elementos do conjunto, libera espaco e devolve NULL.
 */
conjunto_t *destroi_cjt (conjunto_t *c){
    free(c->v);
    c->v = NULL;
    free(c);
    return NULL;
}

/*
 * Cria um conjunto vazio e o retorna, se falhar retorna NULL.
 * max eh o tamanho maximo do conjunto, isto eh, o tamanho maximo do vetor
 */
conjunto_t *cria_cjt (int max){
    conjunto_t *c;
    if (!(c = malloc(sizeof(conjunto_t))))
        return NULL;
    else
    {
        if (!(c->v = malloc(sizeof(int)*max))){
            c = destroi_cjt(c);
            return NULL;
        }
        c->card = 0;
        c->max = max;
        c->ptr = 0;
        return c;
    }
    
}



/*
 * Retorna 1 se o conjunto esta vazio e 0 caso contrario.
 */
int vazio_cjt (conjunto_t *c){
    if (c->card == 0)
        return 1;
    
    return 0;
}

/*
 * Retorna a cardinalidade do conjunto, isto eh, o numero de elementos presentes nele.
 */
int cardinalidade_cjt (conjunto_t *c){
    return c->card;
}

/*
 * Insere o elemento no conjunto, garante que nao existam repeticoes.
 * Retorna 1 se a operacao foi bem sucedida. Se tentar inserir elemento existente,
 * nao faz nada e retorna 1 tambem. Retorna 0 em caso de falha por falta de espaco.
 */
int insere_cjt (conjunto_t *c, int elemento){
    if(c == NULL)
        return 0;
    if(busca_binaria(c->v, elemento, c->card) != -1)
        return 1;
    if(c->card == c->max)
        return 0;
    
    int i = c->card - 1;
    while(i >= 0 && c->v[i] > elemento){
        c->v[i+1] = c->v[i];
        i = i - 1;
    }
    c->v[i+1] = elemento;
    c->card += 1;
    return 1;
}

/*
 * Remove o elemento 'elemento' do conjunto caso ele exista.
 * Retorna 1 se a operacao foi bem sucedida e 0 se o elemento nao existe.
 */
int retira_cjt (conjunto_t *c, int elemento){
    int pos = busca_binaria(c->v, elemento, c->card);
    if(pos == -1)
        return 0;

    int i;
    for (i = pos; i < c->card - 1; i += 1)
        c->v[i] = c->v[i+1];

    c->card -= 1;

    return 1;
}

/*
 * Retorna 1 se o elemento pertence ao conjunto e 0 caso contrario.
 */
int pertence_cjt (conjunto_t *c, int elemento){
    if(c->card == 0)
        return 0;

    if(busca_binaria(c->v, elemento, c->card) == -1)
        return 0;
    
    return 1;

}

/*
 * Retorna 1 se o conjunto c1 esta contido no conjunto c2 e 0 caso contrario.
 */
int contido_cjt (conjunto_t *c1, conjunto_t *c2){
    int i = 0;
    int contido = 1;
    for(i = 0; i < c1->card; i += 1){
        if(!pertence_cjt(c2, c1->v[i]))
            contido = 0;
    }

    return contido;
}

/*
 * Retorna 1 se o conjunto c1 eh igual ao conjunto c2 e 0 caso contrario.
 */
int sao_iguais_cjt (conjunto_t *c1, conjunto_t *c2){
    if(c1->card != c2->card)
        return 0;

    int i;
    for(i = 0; i < c1->card; i += 1)
        if (c1->v[i] != c2->v[i])
            return 0;
        
    return 1;
}

/*
 * Cria e retorna o conjunto diferenca entre c1 e c2, nesta ordem.
 * Retorna NULL se a operacao falhou.
 */
conjunto_t *diferenca_cjt (conjunto_t *c1, conjunto_t *c2){
    conjunto_t *dif;
    dif = cria_cjt(MAX);
    if (dif == NULL)
        return NULL;
    int i = 0;
    int j = 0;
    int k = 0;

    while(i < c1->card && j < c2->card){
        if(c1->v[i] < c2->v[j]){
            dif->v[k] = c1->v[i];
            i += 1;
            k += 1;
        }
        else if(c1->v[i] > c2->v[j]){
            j += 1;
        }
        else{
            i += 1;
            j += 1; 
        }
    }
    for(i = i; i < c1->card; i += 1){
        dif->v[k] = c1->v[i];
        k += 1;
    }
    dif->card = k;

    return dif;
}

/*
 * Cria e retorna o conjunto interseccao entre os conjuntos c1 e c2.
 * Retorna NULL se a operacao falhou.
 */
conjunto_t *interseccao_cjt (conjunto_t *c1, conjunto_t *c2){
    conjunto_t *interseccao;
    interseccao = cria_cjt(MAX);
    if (interseccao == NULL)
        return NULL;
    int i = 0;
    int j = 0;
    int k = 0;

    while(i < c1->card && j < c2->card){
        if(c1->v[i] < c2->v[j])
            i += 1;
        else if (c1->v[i] > c2->v[j])
            j += 1;
        else{
            interseccao->v[k] = c1->v[i];
            i += 1;
            j += 1;
            k += 1;
        }
    }
    interseccao->card = k;

    return interseccao;
}



/*
 * Cria e retorna uma copia do conjunto c e NULL em caso de falha.
 */
conjunto_t *copia_cjt (conjunto_t *c){
    conjunto_t *copia;
    copia = cria_cjt(MAX);
    if(copia == NULL)
        return NULL;

    copia->card = c->card;
    int i;
    for(i = 0; i < c->card; i += 1)
        copia->v[i] = c->v[i]; 

    return copia;
}

/*
 * Cria e retorna o conjunto uniao entre os conjunto c1 e c2.
 * Retorna NULL se a operacao falhou.
 */

conjunto_t *uniao_cjt (conjunto_t *c1, conjunto_t *c2){
    conjunto_t *uniao;
    uniao = cria_cjt(MAX*2);
    if(uniao == NULL)
        return NULL;

    int i = 0;
    int j = 0;
    int k = 0;

    while(i < c1->card && j < c2->card){
        if(c1->v[i] < c2->v[j]){
            uniao->v[k] = c1->v[i];
            i += 1;
        }
        else if(c1->v[i] > c2->v[j]){
            uniao->v[k] = c2->v[j];
            j += 1;
        }else{
            uniao->v[k] = c1->v[i];
            i += 1;
            j += 1;
        }
        k += 1;
    }

    for(i=i; i < c1->card; i+= 1){
        uniao->v[k] = c1->v[i];
        k += 1;
    }
    for (j=j; j < c2->card; j += 1)
    {
        uniao->v[k] = c2->v[j];
        k += 1;
    }
    
    uniao->card = k;
    return uniao;
}



/*
 * Cria e retorna um subconjunto com elementos aleatorios do conjunto c.
 * Se o conjunto for vazio, retorna um subconjunto vazio.
 * Se n >= cardinalidade (c) entao retorna o proprio conjunto c.
 * Supoe que a funcao srand () tenha sido chamada antes.
 */
conjunto_t *cria_subcjt_cjt (conjunto_t *c, int n){
    conjunto_t *subc;
    if(c->card == 0){
        subc = cria_cjt(MAX);
        return subc;
    }
    if(n >= c->card){
        subc = copia_cjt(c);
        return subc;
    }

    subc = cria_cjt(MAX);
    int count;
    int aux;
    for(count = 0; count < n; count += 1){
        do /*Checa se o elemento já não está no subconjunto para não ter elementos repitidos*/
        {
            aux = c->v[rand()%c->card];
        } while (pertence_cjt(subc, aux));
        insere_cjt(subc, aux);
        
    }

    ordenar_vetor(subc->v, subc->card);

    return subc;
}

/*
 * Imprime os elementos do conjunto sempre em ordem crescente,
 * mesmo que a estrutura interna nao garanta isso.
 * Na impressao os elementos sao separados por um unico espaco
 * em branco entre os elementos, sendo que apos o ultimo nao
 * deve haver espacos em branco. Ao final imprime um \n.
 */
void imprime_cjt (conjunto_t *c){
    ordenar_vetor(c->v, c->card);
    int i;
    for(i = 0; i < c->card; i += 1)
        printf("%d ", c->v[i]);
    printf("\n");
}

/*
 * As funcoes abaixo implementam um iterador que vao permitir
 * percorrer os elementos do conjunto.
 * O ponteiro ptr da struct (iterador) pode ser inicializado para apontar 
 * para o primeiro elemento e incrementado ate' o ultimo elemento 
 * do conjunto.
 */

/*
 * Inicializa ptr usado na funcao incrementa_iterador 
 */
void inicia_iterador_cjt (conjunto_t *c){
    c->ptr = 0;
}

/*
 * Devolve no parametro ret_iterador o elemento apontado e incrementa o iterador.
 * A funcao retorna 0 caso o iterador ultrapasse o ultimo elemento, ou retorna
 * 1 caso o iterador aponte para um elemento valido (dentro do conjunto).
 */
int incrementa_iterador_cjt (conjunto_t *c, int *ret_iterador){
    if(c->ptr < c->card){
        *ret_iterador = c->v[c->ptr];
        c->ptr += 1;
        return 1;
    }
    return 0;
}

/*
 * Escolhe um elemento qualquer do conjunto para ser removido, o remove e
 * o retorna.
 * Nao faz teste se conjunto eh vazio, deixa para main fazer isso       
 */
int retira_um_elemento_cjt (conjunto_t *c){
    c->card -= 1;
    return c->v[c->card];
}
