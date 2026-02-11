#include "sacola.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    tProduto* p; /* NÃO é dono */
    int qtd;
} tItem;

struct Sacola {
    tItem* itens;
    int n;
};

static int findIdx(tSacola* s, const char* cod){
    if(!s || !cod || cod[0]=='\0') return -1;
    for(int i=0;i<s->n;i++){
        char* c = getCodProduto(s->itens[i].p);
        if(c && strcmp(c, cod) == 0) return i;
    }
    return -1;
}

tSacola* criaSacola(void){
    tSacola* s = (tSacola*)malloc(sizeof(*s));
    if(!s) return NULL;
    s->itens = NULL;
    s->n = 0;
    return s;
}

void liberaSacola(tSacola* s){
    if(!s) return;
    free(s->itens);
    free(s);
}

int sacolaGetNumItens(tSacola* s){ return s ? s->n : 0; }

tProduto* sacolaGetProduto(tSacola* s, int idx){
    if(!s || idx<0 || idx>=s->n) return NULL;
    return s->itens[idx].p;
}

int sacolaGetQtd(tSacola* s, int idx){
    if(!s || idx<0 || idx>=s->n) return 0;
    return s->itens[idx].qtd;
}

float sacolaTotal(tSacola* s){
    if(!s) return 0.0f;
    float total = 0.0f;
    for(int i=0;i<s->n;i++){
        total += getValorProduto(s->itens[i].p) * (float)s->itens[i].qtd;
    }
    return total;
}

void sacolaImprime(tSacola* s){
    printf("SACOLA DE COMPRAS:\n");
    if(!s || s->n == 0){
        printf("(TOTAL: R$ 0.00)\n");
        return;
    }

    for(int i=0;i<s->n;i++){
        char* cod = getCodProduto(s->itens[i].p);
        char* nome = getNomeProduto(s->itens[i].p);
        printf("PRODUTO %s, %s, %d UNIDADE(S)\n",
               cod ? cod : "",
               nome ? nome : "",
               s->itens[i].qtd);
    }
    printf("(TOTAL: R$ %.2f)\n", sacolaTotal(s));
}

void sacolaEsvazia(tSacola* s){
    if(!s) return;
    free(s->itens);
    s->itens = NULL;
    s->n = 0;
}

tStatusSacola sacolaAdicionaProduto(tSacola* s, tProduto* p, int qtd){
    if(!s || !p || qtd <= 0) return SACOLA_PARAM_INVALIDO;

    char* cod = getCodProduto(p);
    if(!cod || cod[0]=='\0') return SACOLA_PARAM_INVALIDO;

    int idx = findIdx(s, cod);
    if(idx >= 0){
        s->itens[idx].qtd += qtd;
        return SACOLA_OK;
    }

    tItem* novo = (tItem*)realloc(s->itens, (s->n + 1) * sizeof(tItem));
    if(!novo) return SACOLA_PARAM_INVALIDO;

    s->itens = novo;
    s->itens[s->n].p = p;
    s->itens[s->n].qtd = qtd;
    s->n++;
    return SACOLA_OK;
}

static tStatusSacola removeIndice(tSacola* s, int idx){
    if(!s || idx<0 || idx>=s->n) return SACOLA_PARAM_INVALIDO;

    for(int i=idx;i<s->n-1;i++){
        s->itens[i] = s->itens[i+1];
    }
    s->n--;

    if(s->n == 0){
        free(s->itens);
        s->itens = NULL;
        return SACOLA_OK;
    }

    tItem* novo = (tItem*)realloc(s->itens, s->n * sizeof(tItem));
    if(novo) s->itens = novo;
    return SACOLA_OK;
}

tStatusSacola sacolaRemoveProdutoPorCod(tSacola* s, const char* cod, int qtd){
    if(!s || !cod || cod[0]=='\0' || qtd <= 0) return SACOLA_PARAM_INVALIDO;

    int idx = findIdx(s, cod);
    if(idx < 0) return SACOLA_NAO_ENCONTRADO;

    if(qtd >= s->itens[idx].qtd){
        return removeIndice(s, idx);
    }

    s->itens[idx].qtd -= qtd;
    return SACOLA_OK;
}

/* alias usado no ECS */
tStatusSacola sacolaRemovePorCod(tSacola* s, const char* cod, int qtd){
    return sacolaRemoveProdutoPorCod(s, cod, qtd);
}