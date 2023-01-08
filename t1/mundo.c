#include <stdio.h>
#include <stdlib.h>
#include "libconjunto.h"
#include "libfila.h"
#include "liblef.h"
#include <time.h>
#include <math.h>
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


/* CHEGADA: dado1 = idHeroi; dado2 = idLocal
*  SAÍDA: dado1 = idHeroi; dado2 = idLocal
*  MI
*/
evento_t *criar_evento(int dado1, int dado2, int tempo, int tipo){
    evento_t *ev;
    
    if(!(ev = malloc(sizeof(evento_t))))
        return NULL;

    ev->dado1 = dado1;
    ev->dado2 = dado2;
    ev->tempo = tempo;
    ev->tipo = tipo;
    return ev;
}

evento_t *editar_evento(evento_t *ev, int dado1, int dado2, int tempo, int tipo){
    if(!ev)
        return NULL;

    ev->dado1 = dado1;
    ev->dado2 = dado2;
    ev->tempo = tempo;
    ev->tipo = tipo;

    return ev;
}

evento_t *destroi_evento(evento_t *ev){
    if(!ev)
        return NULL;
        
    free(ev);
    return NULL;
}


void imprime_lef(lef_t *l){
        nodo_lef_t *primeiro;
        primeiro = l->Primeiro;
    while (primeiro != NULL)
    {
        
        printf("%d ", primeiro->evento->dado1);
        printf("%d ", primeiro->evento->dado2);
        printf("%d ", primeiro->evento->tempo);
        printf("%d ", primeiro->evento->tipo);
        printf("\n");
        primeiro = primeiro->prox;
    }
}

/* Os métodos a seguir implementam um MergeSort para ordenar a LEF 
 * Foi escolhido ele porque tem um custo mais barato que outros quando
 * se tem muitos elementos e ele tem um custo baixo de memória ao ser usado em
 * listas lincadas. (vimos esse algoritmo para ordenar vetores em alg2. pesquisei como adaptar para
 * listas lincadas).
*/

nodo_lef_t *obter_meio_lef(nodo_lef_t *primeiro){
   
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

nodo_lef_t *merge_lef(nodo_lef_t *lista1, nodo_lef_t *lista2){
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

nodo_lef_t *merge_sort_lef(nodo_lef_t *primeiro){
    if(primeiro == NULL || primeiro->prox == NULL)
        return primeiro;

    nodo_lef_t *esquerda;
    nodo_lef_t *direita;
    nodo_lef_t *meio;

    esquerda = primeiro;
    meio = obter_meio_lef(primeiro);
    direita = meio->prox;
    meio->prox = NULL;
   
    esquerda = merge_sort_lef(esquerda);
    direita = merge_sort_lef(direita);

    return merge_lef(esquerda, direita);
}

int max(int a, int b){
    if(a > b)
        return a;
    else
        return b;
}

int  distancia_pontos(localizacao ponto_a, localizacao ponto_b){
    return sqrt(pow(ponto_b.x - ponto_a.x, 2) + pow(ponto_b.y - ponto_a.y, 2));
}
/* CHEGADA: dado1 = idHeroi; dado2 = idLocal
*  SAÍDA: dado1 = idHeroi; dado2 = idLocal*/

/*Funções para odenar o array de locais*/

void invocar_evento(mundo *meu_mundo, evento_t *ev){
    meu_mundo->tempo_atual = ev->tempo;
    int id_heroi;
    int id_local;
    

    switch (ev->tipo)
    {
        case 0: {
            id_heroi = ev->dado1;
            id_local = ev->dado2;
            local local_atual = meu_mundo->locais[id_local];
            heroi heroi_atual = meu_mundo->herois[id_heroi];

            printf("%6d:CHEGA HEROI %2d Local %d ( %d/%d), ", meu_mundo->tempo_atual, id_heroi, id_local, local_atual.publico->card, local_atual.lotacao_max);
            if(cardinalidade_cjt(local_atual.publico) >= local_atual.lotacao_max){
                if (((heroi_atual.paciencia / 4) - tamanho_fila(local_atual.fila_entrada)) > 0){
                    insere_fila(local_atual.fila_entrada, id_heroi); 
                    printf("FILA %d", tamanho_fila(local_atual.fila_entrada));
                }
                
                else{
                    adiciona_inicio_lef(meu_mundo->eventos_futuros, editar_evento(ev, id_heroi, id_local, meu_mundo->tempo_atual, 1));
                    printf("DESISTE");
                    meu_mundo->eventos_futuros->Primeiro = merge_sort_lef(meu_mundo->eventos_futuros->Primeiro);
                }                   
            }
            else{
                printf("ENTRA");

                int tpl = max(1, heroi_atual.paciencia / 10 + (rand() % 8 - 2));

                insere_cjt(local_atual.publico, id_heroi);
                adiciona_ordem_lef(meu_mundo->eventos_futuros, editar_evento(ev, id_heroi, id_local, meu_mundo->tempo_atual + tpl, 1));
                meu_mundo->eventos_futuros->Primeiro = merge_sort_lef(meu_mundo->eventos_futuros->Primeiro);
            }
            printf("\n");
            ev = destroi_evento(ev);
            break;
        }

        case 1: {
            id_heroi = ev->dado1;
            id_local = ev->dado2;
            local local_atual = meu_mundo->locais[id_local];
            int primeiro_fila;
            int id_destino = rand() % meu_mundo->num_locais;
            int tdl = distancia_pontos(local_atual.ponto, meu_mundo->locais[id_destino].ponto) / (100-max(0, meu_mundo->herois[id_heroi].idade - 40));

            printf("%6d:SAIDA HEROI %2d Local %d ( %d/%d)", meu_mundo->tempo_atual, id_heroi, id_local, local_atual.publico->card, local_atual.lotacao_max);
            
            adiciona_inicio_lef(meu_mundo->eventos_futuros, editar_evento(ev, id_heroi, id_destino, meu_mundo->tempo_atual + tdl/15, 0));
            meu_mundo->eventos_futuros->Primeiro = merge_sort_lef(meu_mundo->eventos_futuros->Primeiro);
            if(pertence_cjt(local_atual.publico, id_heroi)){
                imprime_cjt(local_atual.publico);
                if(retira_fila(local_atual.fila_entrada, &primeiro_fila)){
                    adiciona_inicio_lef(meu_mundo->eventos_futuros, editar_evento(ev, primeiro_fila, id_local, meu_mundo->tempo_atual, 0));
                    printf(", REMOVE FILA HEROI %2d", primeiro_fila);
                }
                
                retira_cjt(local_atual.publico, id_heroi);
            }
            
            printf("\n");
            ev = destroi_evento(ev);
            break;
        }

        case 2:{    
   
            int num_locais = meu_mundo->num_locais;
            conjunto_t *missao;
            int id_missao = ev->dado1;
            missao = cria_subcjt_cjt(meu_mundo->cj_habilidades, rand()%(6-3) + 3 );
            int i;
            int id_equipe = -1;
            conjunto_t *equipe;
            conjunto_t *uniao;
            equipe = NULL;

            printf("%6d:MISSAO %d HAB_REQ ", meu_mundo->tempo_atual, id_missao);
            imprime_cjt(missao);
            printf("\n");
            for (i = 0; i < num_locais; i++)
            {
                id_local = i;
                int j;
                
                uniao = cria_cjt(meu_mundo->num_habilidades);
                inicia_iterador_cjt(meu_mundo->locais[i].publico);
                for (j = 0; j < cardinalidade_cjt(meu_mundo->locais[i].publico); j++)
                {
                    conjunto_t *aux;
                    incrementa_iterador_cjt(meu_mundo->locais[i].publico, &id_heroi);
                    aux = uniao;
                    uniao = uniao_cjt(uniao, meu_mundo->herois[id_heroi].habilidades);
                    destroi_cjt(aux);
                }

                printf("%6d:MISSAO %d HAB_EQL %d:", meu_mundo->tempo_atual, id_missao, id_local);
                imprime_cjt(uniao);
                
                printf("\n");

                if(contido_cjt(missao, uniao) && (id_equipe == -1 || cardinalidade_cjt(meu_mundo->locais[i].publico) < cardinalidade_cjt(equipe))){
                    conjunto_t *aux;
                    id_equipe = i;
                    aux = equipe;
                    equipe = copia_cjt(meu_mundo->locais[i].publico);
                    
                    if(aux)
                        destroi_cjt(aux);
                }
                
                uniao = destroi_cjt(uniao);
                
            }
            
            if(id_equipe == -1){
                printf("%6d:MISSAO %d IMPOSSIVEL\n", meu_mundo->tempo_atual, id_missao);
                adiciona_ordem_lef(meu_mundo->eventos_futuros, editar_evento(ev, ev->dado1, ev->dado2, rand()%((meu_mundo->fim_do_mundo-1)- meu_mundo->tempo_atual) + meu_mundo->tempo_atual, 2));
            }
            else{
                printf("%6d:MISSAO %d HER_EQS %d:", meu_mundo->tempo_atual, id_missao, id_equipe);
                imprime_cjt(equipe);
                printf("\n");

                while(cardinalidade_cjt(equipe)){
                    meu_mundo->herois[retira_um_elemento_cjt(equipe)].exp += 1;
                }

                equipe = destroi_cjt(equipe);
            }

            destroi_cjt(missao);
            destroi_evento(ev);
            break;
        }

        case 3:{
            printf("%6d:FIM\n", meu_mundo->tempo_atual);
            ev = destroi_evento(ev);
            evento_t *prox_evento;
            while ((prox_evento = obtem_primeiro_lef(meu_mundo->eventos_futuros)))
                destroi_evento(prox_evento);
            meu_mundo->eventos_futuros = destroi_lef(meu_mundo->eventos_futuros);

            break;
        }
    
        default:
            break;

    }
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
        
    evento_t *ev = criar_evento(0, 0, 0, 0);
    for (i = 0; i < meu_mundo->num_herois; i++){
        if(!(ev = editar_evento(ev, i, rand() % meu_mundo->num_locais, rand() % (96 * 7 + 1), 0))){
            printf("Falha ao criar EVENTO chegadas");
            exit(0);
        }
        if(!adiciona_ordem_lef(meu_mundo->eventos_futuros, ev)){
            printf("Falha ao adicionar EVENTO chegadas");
            exit(0);
        }
        
    }
        
    
    for (i = 0; i < meu_mundo->num_missoes; i++){
        if(!(ev = editar_evento(ev, i, 0, rand() % (meu_mundo->fim_do_mundo + 1), 2))){
            printf("Falha ao criar EVENTO chegadas");
            exit(0);
        }
        if(!adiciona_ordem_lef(meu_mundo->eventos_futuros, ev)){
            printf("Falha ao adicionar EVENTO missoes");
            exit(0);
        }
        
    }
    

    adiciona_ordem_lef(meu_mundo->eventos_futuros, editar_evento(ev, 0, 0, meu_mundo->fim_do_mundo, 3));
    ev = destroi_evento(ev);
    imprime_lef(meu_mundo->eventos_futuros);
    meu_mundo->eventos_futuros->Primeiro = merge_sort_lef(meu_mundo->eventos_futuros->Primeiro);
    evento_t *prox_evento;
    while ((prox_evento = obtem_primeiro_lef(meu_mundo->eventos_futuros)))
    {
        invocar_evento(meu_mundo, prox_evento);

    }

    for (i = 0; i < meu_mundo->num_herois; i++)
    {
        printf("HEROI %2d EXPERIENCIA %d\n", meu_mundo->herois[i].id, meu_mundo->herois[i].exp);
    }
    

    for (i = 0; i < meu_mundo->num_herois; i++)
       meu_mundo->herois[i].habilidades = destroi_cjt(meu_mundo->herois[i].habilidades);
    
    for (i = 0; i < meu_mundo->num_locais; i++){
        meu_mundo->locais[i].fila_entrada = destroi_fila(meu_mundo->locais[i].fila_entrada);
        meu_mundo->locais[i].publico = destroi_cjt(meu_mundo->locais[i].publico);
    }
        
    meu_mundo->cj_habilidades = destroi_cjt(meu_mundo->cj_habilidades);
    free(meu_mundo);
    meu_mundo = NULL;

    
    return 0;
}
