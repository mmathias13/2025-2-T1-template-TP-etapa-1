#ifndef _LOJA_H
#define _LOJA_H

#include "produto.h"

typedef struct Loja tLoja;

tLoja* criaLoja(char* nome, char* cnpj, char* telefone, char* endereco, char* username, char* senha);
void liberaLoja(tLoja *l);

char* getEnderecoLoja(tLoja *l);
char* getTelefoneLoja(tLoja *l);
char* getUsuarioLoja(tLoja *l);
char* getNomeLoja(tLoja *l);
char* getCnpjLoja(tLoja *l);
int getNumProdutosLoja(tLoja* l);

tProduto* getProdutoLoja(tLoja* l, int idx);

/* retorna ponteiro pro produto ou NULL */
tProduto* buscaProdutoLojaPorId(tLoja* l, char* cod);

/* adiciona produto (loja passa a ser dona). Retorna 1 ok, 0 falha/duplicado */
int adicionaProdutoLoja(tLoja* l, tProduto* p);

#endif
