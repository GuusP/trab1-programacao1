#include <stdio.h>
#include <stdlib.h>
#include "libconjunto.h"
#include "libfila.h"
#include "liblef.h"
#include <time.h>
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS N_HABILIDADES*5
#define N_LOCAIS N_HEROIS/6
#define N_FIM_DO_MUNDO 34944
#define N_MISSOES N_FIM_DO_MUNDO/100


typedef struct heroi
{
    int id; 
    int paciencia;
    int idade;
    int exp;
    conjunto_t *habilidades;
} heroi;

typedef struct localizacao
{
    int x, y;
} localizacao;


typedef struct local
{
    int id;
    int lotacao_max;
    conjunto_t *publico;
    fila_t *fila_entrada;
    localizacao ponto;
    
} local;


typedef struct mundo
{
    int tempo_atual;
    lef_t *eventos_futuros;
    int num_locais;
    int num_herois;
    int tam_mundo;
    int num_habilidades;
    int fim_do_mundo;
    int num_missoes;
    heroi herois[N_HEROIS];
    local locais[N_LOCAIS];
    conjunto_t *cj_habilidades;
    
} mundo;

typedef struct missao
{
    int id;
    conjunto_t hab_necessarias;

} missao;


int insere_herois(heroi herois[], int n, conjunto_t *habilidades){
    int i;
    for (i = 0; i < n; i++)
    {
        herois[i].id = i;
        herois[i].exp = 0;
        herois[i].idade = (rand() % 83) + 18;
        herois[i].paciencia = rand() % 101;
        if(!(herois[i].habilidades = cria_subcjt_cjt(habilidades, (rand() % 4) + 2)))
            return 0;
    }
    return 1;
}

int insere_locais(local locais[], int n, int tamanho_mundo){
    int i;
    for (i = 0; i < n; i++)
    {
        locais[i].id = i;
        locais[i].lotacao_max = (rand() % 26) + 5;
        locais[i].ponto.x = rand() % tamanho_mundo;
        locais[i].ponto.y = rand() % tamanho_mundo;
        if(!(locais[i].fila_entrada = cria_fila()))
            return 0;
        if(!(locais[i].publico = cria_cjt(locais[i].lotacao_max))){
            destroi_fila(locais[i].fila_entrada);
            return 0;
        }
    }

    return 1;
} 

/*int insere_missoes(missao missoes[], conjunto_t cj_habilidades, int n){
    int i;
    for (i = 0; i < n; i++)
    {
        if(!(missoes[i].hab_necessarias = cria_subcjt_cjt(cj_habilidades)))
    }
    
}*/
/* CHEGADA: dado1 = idHeroi; dado2 = idLocal
*  SAÍDA: dado1 = idHeroi; dado2 = idLocal
*  MI
*/
evento_t *criar_evento(int dado1, int dado2, int tempo, int tipo){
    evento_t *ev;
    
    if(!(ev = malloc(sizeof(ev))))
        return NULL;

    ev->dado1 = dado1;
    ev->dado2 = dado2;
    ev->tempo = tempo;
    ev->tipo = tipo;
    return ev;
}

void imprime_lef(lef_t *l){
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
}

/* Os métodos a seguir implementam um MergeSort para ordenar a LEF 
 * Foi escolhido ele porque tem um custo mais barato que outros quando
 * se tem muitos elementos e ele tem um custo baixo de memória ao ser usado em
 * listas lincadas. (vimos esse algoritmo para ordenar vetores em alg2. pesquisei como adaptar para
 * listas lincadas).
*/

nodo_lef_t *obter_meio(nodo_lef_t *primeiro){
   
    nodo_lef_t *fast;
    nodo_lef_t *slow;
    fast = primeiro->prox;
    slow = primeiro;

    while (fast != NULL && fast->prox != NULL)
    {
        fast = fast->prox->prox; 
        slow = slow->prox;
    }

    return slow;
}

nodo_lef_t *merge(nodo_lef_t *lista1, nodo_lef_t *lista2){
    nodo_lef_t *inicio_lista;
    nodo_lef_t *fim_lista;
    inicio_lista = NULL;

    while (lista1 != NULL && lista2 != NULL)
    {
        nodo_lef_t *novo_nodo;
        if(lista1->evento->tempo <= lista2->evento->tempo){
            novo_nodo = lista1; 
            lista1 = lista1->prox;
        }else{
            novo_nodo = lista2;
            lista2 = lista2->prox;
        }

        if(inicio_lista == NULL){
            inicio_lista = novo_nodo;
            fim_lista = novo_nodo;
            inicio_lista->prox = NULL;
        }else{
            fim_lista->prox = novo_nodo;
            fim_lista = fim_lista->prox;
        }
        
    }
    
    if(lista1 != NULL)
        fim_lista->prox = lista1;
    else
        fim_lista->prox = lista2;

    return inicio_lista;
}

nodo_lef_t *merge_sort(nodo_lef_t *primeiro){
    if(primeiro == NULL || primeiro->prox == NULL)
        return primeiro;

    nodo_lef_t *esquerda;
    nodo_lef_t *direita;
    nodo_lef_t *meio;

    esquerda = primeiro;
    meio = obter_meio(primeiro);
    direita = meio->prox;
    meio->prox = NULL;
   
    esquerda = merge_sort(esquerda);
    direita = merge_sort(direita);

    return merge(esquerda, direita);
}


/*TODO: remover printfs de debug*/
int main(){

    srand(1);
    mundo *meu_mundo;
    if(!(meu_mundo = malloc(sizeof(mundo))))
        exit(0);
    
    meu_mundo->tempo_atual = 0;
    meu_mundo->num_herois = N_HEROIS;
    meu_mundo->num_locais = N_LOCAIS;
    meu_mundo->tam_mundo = N_TAMANHO_MUNDO;
    meu_mundo->num_habilidades = N_HABILIDADES;
    meu_mundo->fim_do_mundo = N_FIM_DO_MUNDO;
    meu_mundo->num_missoes = N_MISSOES;
    missao mission;

    if(!(meu_mundo->eventos_futuros = cria_lef())){
        printf("Erro ao alocar lista de eventos futuros");
        exit(0);
    }
        
    if(!(meu_mundo->cj_habilidades = cria_cjt(meu_mundo->num_habilidades))){
        printf("Erro ao alocar conjunto de habilidades");
        exit(0);
    }
        
    int i;
    for(i = 0; i < meu_mundo->num_habilidades; i++){
        if(!insere_cjt(meu_mundo->cj_habilidades, i)){
            printf("Erro ao inserir habilidades");
            exit(0);
        }
        
    }

    if(!insere_herois(meu_mundo->herois, meu_mundo->num_herois, meu_mundo->cj_habilidades)){
        printf("Erro ao inserir herois no vetor");
        exit(0);
    }

    if(!insere_locais(meu_mundo->locais, meu_mundo->num_locais, meu_mundo->tam_mundo)){
        printf("Erro ao inserir os locais no vetor");
        exit(0);
    }
        
    for (i = 0; i < meu_mundo->num_herois; i++)
        adiciona_ordem_lef(meu_mundo->eventos_futuros, criar_evento(i, rand() % meu_mundo->num_locais, rand() % (96 * 7 + 1), 0));

    for (i = 0; i < meu_mundo->num_missoes; i++)
        adiciona_ordem_lef(meu_mundo->eventos_futuros, criar_evento(i, 0, rand() % (meu_mundo->fim_do_mundo + 1), 3));
    


    meu_mundo->eventos_futuros->Primeiro = merge_sort(meu_mundo->eventos_futuros->Primeiro);
    imprime_lef(meu_mundo->eventos_futuros);

    return 0;
}
