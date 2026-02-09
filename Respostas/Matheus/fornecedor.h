#ifndef _FORNECEDOR_H
#define _FORNECEDOR_H

typedef struct Fornecedor tFornecedor;

tFornecedor* criaFornecedor(char* nome, char* cnpj, char* telefone, char* endereco, char* username, char* senha);

void liberaFornecedor(tFornecedor *f);

char* getEnderecoFornecedor(tFornecedor *f);
char* getTelefoneFornecedor(tFornecedor *f);
char* getUsuarioFornecedor(tFornecedor *f);
char* getNomeFornecedor(tFornecedor *f);
char* getCnpjFornecedor(tFornecedor *f);

#endif