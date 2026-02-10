#ifndef _SISTEMA_H
#define _SISTEMA_H

#include "cliente.h"
#include "loja.h"
#include "fornecedor.h"

typedef struct Sistema tSistema;

typedef enum {
    SISTEMA_OK = 0,
    SISTEMA_ERRO_ALOC = 1,
    SISTEMA_DUPLICADO = 2,
    SISTEMA_PARAM_INVALIDO = 3
} tStatusSistema;

/* Ciclo de vida */
tSistema* criaSistema(void);
void liberaSistema(tSistema* s);

/* ======== CLIENTES ======== */
tCliente* buscaClientePorCPF(tSistema* s, char* cpf);

/* Adiciona: sistema passa a ser dono do ponteiro e deve destruí-lo no final.
   Se retornar DUPLICADO/ERRO_ALOC, o chamador continua dono e deve liberar. */
tStatusSistema adicionaCliente(tSistema* s, tCliente* c);

int getNumClientesSistema(tSistema* s);
tCliente* getClienteSistema(tSistema* s, int idx);

/* ======== LOJAS ======== */
tLoja* buscaLojaPorCNPJ(tSistema* s, char* cnpj);
tStatusSistema adicionaLoja(tSistema* s, tLoja* l);

int getNumLojasSistema(tSistema* s);
tLoja* getLojaSistema(tSistema* s, int idx);

/* ======== FORNECEDORES ======== */
tFornecedor* buscaFornecedorPorCNPJ(tSistema* s, char* cnpj);
tStatusSistema adicionaFornecedor(tSistema* s, tFornecedor* f);

int getNumFornecedoresSistema(tSistema* s);
tFornecedor* getFornecedorSistema(tSistema* s, int idx);

tProduto* buscaProdutoSistemaPorCod(tSistema* s, char* cod, tLoja** lojaDona);

// INgredientes
tIngrediente* buscaIngredienteDisponivelPorNome(tSistema* s, char* nome, int qtdNecessaria);
void consomeIngredientePorNome(tSistema* s, char* nome, int qtd);



#endif