#ifndef _PRATO_H
#define _PRATO_H

#include "sistema.h"

typedef struct Prato tPrato;

/* ASSINATURA QUE SUA MAIN PRECISA USAR:
   criaPrato(sistema, cod, nome, desc, preco, N, ingredientes)
*/
tPrato* criaPrato(tSistema* sistema, char* cod, char* nome, char* desc, float valor,
                  int nIng, char** ingredientes);

/* callbacks para tProduto */
void imprimeFisicoPrato(void* dado, int qtd);
void imprimeDigitalPrato(void* dado, char* email, int qtd);
void liberaPrato(void* dado);

float getValorPrato(void* dado);
char* getCodPrato(void* dado);
char* getNomePrato(void* dado);
char getTipoPrato(void* dado);
char* getDescPrato(void* dado);

/* NOMES que sua main chama */
int getDisponibilidadePrato(void* dado, int qtd);
void alteraDisponibilidadePrato(void* dado, int qtd);

void printaPrato(void* dado);

#endif