#ifndef _FORNECEDOR_H
#define _FORNECEDOR_H

typedef struct Fornecedor tFornecedor;
typedef struct Ingrediente tIngrediente;

/* Criação e destruição */
tFornecedor* criaFornecedor(char* nome, char* cnpj, char* telefone,
                            char* endereco, char* username, char* senha);
void liberaFornecedor(tFornecedor *f);

/* Ingredientes */
int buscaIngredienteFornecedor(tFornecedor *f, int codigoBarras);
void cadastraOuAtualizaIngredienteFornecedor(tFornecedor *f, tIngrediente *ing);

/* Getters básicos */
char* getEnderecoFornecedor(tFornecedor *f);
char* getTelefoneFornecedor(tFornecedor *f);
char* getUsuarioFornecedor(tFornecedor *f);
char* getNomeFornecedor(tFornecedor *f);
char* getCnpjFornecedor(tFornecedor *f);

/* Getters para testes / iteração (sem quebrar TAD) */
int getNumIngredientesFornecedor(tFornecedor *f);
tIngrediente* getIngredientePorIndiceFornecedor(tFornecedor *f, int idx);

#endif