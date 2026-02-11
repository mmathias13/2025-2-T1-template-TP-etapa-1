#include "ingrediente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Ingrediente
{
    char nomeIngrediente[101];
    int codigoBarrasIngrediente;
    char tipoIngrediente;
    int quantidadeIngrediente;
};

tIngrediente* criaIngrediente(char* nome, int codigo, char tipo, int quantidade){
    tIngrediente *i = (tIngrediente*)malloc(sizeof(tIngrediente));
    if (i == NULL){
        printf("Erro de alocação em INGREDIENTE!\n");
        return NULL;
    }

    strcpy(i->nomeIngrediente, nome);
    i->tipoIngrediente = tipo;
    i->codigoBarrasIngrediente = codigo;
    i->quantidadeIngrediente = quantidade;

    return i;
}

void liberaIngrediente(tIngrediente* i){
    free(i);
}

void adicionaQuantidadeIngrediente(tIngrediente *i, int qtd){
    if(!i) return;
    i->quantidadeIngrediente += qtd;
}

void consomeQuantidadeIngrediente(tIngrediente* i, int qtd){
    if(!i) return;
    i->quantidadeIngrediente -= qtd;
    if(i->quantidadeIngrediente < 0) i->quantidadeIngrediente = 0;
}

char* getNomeIngrediente(tIngrediente* i){
    return i ? i->nomeIngrediente : NULL;
}

char getTipoIngrediente(tIngrediente* i){
    return i ? i->tipoIngrediente : '\0';
}

int getCodigoBarrasIngrediente(tIngrediente *i){
    return i ? i->codigoBarrasIngrediente : -1;
}

int getQuantidadeIngrediente(tIngrediente *i){
    return i ? i->quantidadeIngrediente : 0;
}