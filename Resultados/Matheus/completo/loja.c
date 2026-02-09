#include "loja.h"
#include "produto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Loja
{
    char nomeLoja[101];
    char cnpjLoja[19];
    char telefoneLoja[15];
    char enderecoLoja[151];
    char nomeUsuarioLoja[16];
    char senhaLoja[21];
};

tLoja* criaLoja(char* nome, char* cnpj, char* telefone, char* endereco, char* username, char* senha){
    tLoja *l = (tLoja*)malloc(sizeof(tLoja));
    if(l == NULL){
        printf("Problema na alocação de LOJA!\n");
        return NULL;
    }

    //Atribuições feitas com strcpy
    strcpy(l->nomeLoja, nome);
    strcpy(l->cnpjLoja, cnpj);
    strcpy(l->telefoneLoja, telefone);
    strcpy(l->enderecoLoja, endereco);
    strcpy(l->nomeUsuarioLoja, username);
    strcpy(l->senhaLoja, senha);


    return l;
}



void liberaLoja(tLoja *l){
    free(l);
}

char* getEnderecoLoja(tLoja *l){
    return l->enderecoLoja;
}
char* getTelefoneLoja(tLoja *l){
    return l->telefoneLoja;
}
char* getUsuarioLoja(tLoja *l){
    return l->nomeUsuarioLoja;
}
char* getNomeLoja(tLoja *l){
    return l->nomeLoja;
}
char* getCnpjLoja(tLoja *l){
    return l->cnpjLoja;
}