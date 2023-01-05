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
#define FIM_DO_MUNDO 34944
#define N_MISSOES FIM_DO_MUNDO/100
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





int main(){
    srand(1);
    mundo *meu_mundo;
    if(!(meu_mundo = malloc(sizeof(mundo))))
        exit(0);
    
    meu_mundo->tempo_atual = 0;
    meu_mundo->num_herois = N_HEROIS;
    meu_mundo->num_locais = N_LOCAIS;
    meu_mundo->tam_mundo = N_TAMANHO_MUNDO;

    if(!(meu_mundo->eventos_futuros = cria_lef())){
        printf("Erro ao alocar lista de eventos futuros");
        exit(0);
    }
        
    if(!(meu_mundo->cj_habilidades = cria_cjt(N_HABILIDADES))){
        printf("Erro ao alocar conjunto de habilidades");
        exit(0);
    }
        
    int i;
    for(i = 0; i < N_HABILIDADES; i++){
        if(!insere_cjt(meu_mundo->cj_habilidades, i)){
            printf("Erro ao inserir habilidades");
            exit(0);
        }
        
    }

    if(!insere_herois(meu_mundo->herois, N_HEROIS, meu_mundo->cj_habilidades)){
        printf("Erro ao inserir herois no vetor");
        exit(0);
    }

    if(!insere_locais(meu_mundo->locais, N_LOCAIS, N_TAMANHO_MUNDO)){
        printf("Erro ao inserir os locais no vetor");
        exit(0);
    }
        
    
    for (i = 0; i < N_HEROIS; i++)
    {
        adiciona_ordem_lef(meu_mundo->eventos_futuros, criar_evento(i, rand() % N_LOCAIS, rand() % (96 * 7 + 1), 0));
    }
    while (meu_mundo->eventos_futuros->Primeiro != NULL)
    {
        evento_t *primeiro;
        primeiro = obtem_primeiro_lef(meu_mundo->eventos_futuros);
        printf("%d ", primeiro->dado1);
        printf("%d ", primeiro->dado2);
        printf("%d ", primeiro->tempo);
        printf("%d ", primeiro->tipo);
        printf("\n");
    }
    return 0;
}
