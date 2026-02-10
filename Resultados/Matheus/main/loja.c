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
    tProduto **produtos;
    int numProdutos;
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

    l->produtos = NULL;
    l->numProdutos = 0;


    return l;
}



void liberaLoja(tLoja* l){
    if(!l) return;

    for(int i=0;i<l->numProdutos;i++){
        if(l->produtos[i]) liberaProduto(l->produtos[i]);
    }
    free(l->produtos);
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
int getNumProdutosLoja(tLoja *l){
    return l->numProdutos;
}
tProduto* getProdutoLoja(tLoja* l, int idx){
    if(!l || idx<0 || idx>=l->numProdutos) return NULL;
    return l->produtos[idx];
}

tProduto* buscaProdutoLojaPorId(tLoja* l, char* cod){
    if(!l || !cod || cod[0]=='\0') return NULL;
    for(int i=0;i<l->numProdutos;i++){
        if(l->produtos[i] && strcmp(getCodProduto(l->produtos[i]), cod) == 0)
            return l->produtos[i];
    }
    return NULL;
}

int adicionaProdutoLoja(tLoja* l, tProduto* p){
    if(!l || !p) return 0;

    char* cod = getCodProduto(p);
    if(!cod || cod[0]=='\0') return 0;

    if(buscaProdutoLojaPorId(l, cod) != NULL) return 0;

    tProduto** novo = realloc(l->produtos, (l->numProdutos+1) * sizeof(tProduto*));
    if(!novo) return 0;

    l->produtos = novo;
    l->produtos[l->numProdutos] = p;
    l->numProdutos++;
    return 1;
}
