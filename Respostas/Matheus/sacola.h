#ifndef _SACOLA_H
#define _SACOLA_H

#include "produto.h"

typedef struct Sacola tSacola;

typedef enum {
    SACOLA_OK = 0,
    SACOLA_PARAM_INVALIDO = 1,
    SACOLA_NAO_ENCONTRADO = 2
} tStatusSacola;

tSacola* criaSacola(void);
void liberaSacola(tSacola* s);

int sacolaGetNumItens(tSacola* s);
tProduto* sacolaGetProduto(tSacola* s, int idx);
int sacolaGetQtd(tSacola* s, int idx);

float sacolaTotal(tSacola* s);
void sacolaImprime(tSacola* s);

void sacolaEsvazia(tSacola* s);

/* NOMES que sua main usa */
tStatusSacola sacolaAdicionaProduto(tSacola* s, tProduto* p, int qtd);
tStatusSacola sacolaRemoveProdutoPorCod(tSacola* s, const char* cod, int qtd);

/* usado no ECS */
tStatusSacola sacolaRemovePorCod(tSacola* s, const char* cod, int qtd);

#endif