#ifndef _PRATO_H
#define _PRATO_H

typedef struct Prato tPrato;
typedef struct Sistema tSistema;

tPrato* criaPrato(tSistema* s, char* cod, char* nome, float preco, int nIng, char** nomesIng);
void liberaPrato(void* dado);

/* callbacks compatíveis com produto.h */
void imprimeFisicoPrato(void* dado, int qtd);
void imprimeDigitalPrato(void* dado, char* email, int qtd);

float getValorPrato(void* dado);
char* getCodPrato(void* dado);
char* getNomePrato(void* dado);
char  getTipoPrato(void* dado);
char* getDescPrato(void* dado);

int   getDisponibilidadePrato(void* dado, int qtd);
void  alteraDisponibilidadePrato(void* dado, int qtd);

void  printaPrato(void* dado);

#endif
