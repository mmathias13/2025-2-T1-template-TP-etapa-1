#include "ingrediente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Ingrediente: estrutura simples com nome/código/tipo/quantidade e operações de estoque.

struct Ingrediente
{
    char nomeIngrediente[101];
    int codigoBarrasIngrediente;
    char tipoIngrediente;
    int quantidadeIngrediente;
};

/*
 * Cria e inicializa a estrutura relacionada a ingrediente.
 */
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

/*
 * Libera a memória e recursos associados a liberaIngrediente.
 */
void liberaIngrediente(tIngrediente* i){
    free(i);
}

/*
 * Adiciona um item/registro, cuidando de realocação quando necessário.
 */
void adicionaQuantidadeIngrediente(tIngrediente *i, int qtd){
    if(!i) return;
    i->quantidadeIngrediente += qtd;
}

void consomeQuantidadeIngrediente(tIngrediente* i, int qtd){
    if(!i) return;
    i->quantidadeIngrediente -= qtd;
    if(i->quantidadeIngrediente < 0) i->quantidadeIngrediente = 0;
}

char* getNomeIngrediente(tIngrediente* i)
{
    if (i) {
        return i->nomeIngrediente;
    }
    return NULL;
}
char getTipoIngrediente(tIngrediente* i)
{
    if (i) {
        return i->tipoIngrediente;
    }
    return '\0';
}
int getCodigoBarrasIngrediente(tIngrediente *i)
{
    if (i) {
        return i->codigoBarrasIngrediente;
    }
    return -1;
}
int getQuantidadeIngrediente(tIngrediente *i)
{
    if (i) {
        return i->quantidadeIngrediente;
    }
    return 0;
}