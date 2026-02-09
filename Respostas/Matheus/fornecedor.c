#include "fornecedor.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct Fornecedor
{
    char nomeFornecedor[101];
    char cnpjFornecedor[19];
    char telefoneFornecedor[15];
    char enderecoFornecedor[151];
    char nomeUsuarioFornecedor[16];
    char senhaFornecedor[21];
};

tFornecedor* criaFornecedor(char* nome, char* cnpj, char* telefone, char* endereco, char* username, char* senha){
    tFornecedor *f = (tFornecedor*)malloc(sizeof(tFornecedor));
    if(f == NULL){
        printf("Problema na alocação de FORNECEDOR!\n");
        return NULL;
    }

    //Atribuições feitas com strcpy
    strcpy(f->nomeFornecedor, nome);
    strcpy(f->cnpjFornecedor, cnpj);
    strcpy(f->telefoneFornecedor, telefone);
    strcpy(f->enderecoFornecedor, endereco);
    strcpy(f->nomeUsuarioFornecedor, username);
    strcpy(f->senhaFornecedor, senha);


    return f;
}



void liberaFornecedor(tFornecedor *f){
    free(f);
}

char* getEnderecoFornecedor(tFornecedor *f){
    return f->enderecoFornecedor;
}
char* getTelefoneFornecedor(tFornecedor *f){
    return f->telefoneFornecedor;
}
char* getUsuarioFornecedor(tFornecedor *f){
    return f->nomeUsuarioFornecedor;
}
char* getNomeFornecedor(tFornecedor *f){
    return f->nomeFornecedor;
}
char* getCnpjFornecedor(tFornecedor *f){
    return f->cnpjFornecedor;
}
