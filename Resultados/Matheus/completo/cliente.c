#include "cliente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Módulo do Cliente: guarda dados pessoais, avaliações e a sacola de compras.

struct Cliente{
    char nomeCompleto[101];
    char cpf[15];
    char telefone[16];
    char endereco[151];
    char nomeUsuario[16];
    char senha[21];
    char dataNascimento[11];
    char numeroCartao[13];
    char email[51];

    int numeroProdutosComprados;
    int numeroAvaliacoesRealizadas;

    tSacola* sacola;
};

tCliente* criaCliente(char* nome, char* cpf, char* data, char* telefone, char* endereco,
                      char* email, char* username, char* senha, char* cartao)
{
    tCliente *c = (tCliente*)malloc(sizeof(tCliente));
    if (c == NULL){
        printf("Problema na alocação de memória de CLIENTE!");
        return NULL;
    }

    strcpy(c->nomeCompleto, nome);
    strcpy(c->cpf, cpf);
    strcpy(c->dataNascimento, data);
    strcpy(c->telefone, telefone);
    strcpy(c->endereco, endereco);
    strcpy(c->email, email);
    strcpy(c->nomeUsuario, username);
    strcpy(c->senha, senha);
    strcpy(c->numeroCartao, cartao);

    c->numeroAvaliacoesRealizadas = 0;
    c->numeroProdutosComprados = 0;

    c->sacola = criaSacola();
    if(!c->sacola){
        free(c);
        return NULL;
    }

    return c;
}

/*
 * Libera a memória e recursos associados a liberaCliente.
 */
void liberaCliente(tCliente* c){
    if(!c) return;
    liberaSacola(c->sacola);
    free(c);
}

char* getCpfCliente(tCliente* c)
{
    if (c) {
        return c->cpf;
    }
    return NULL;
}
char* getEmailCliente(tCliente* c)
{
    if (c) {
        return c->email;
    }
    return NULL;
}
char* getNomeCliente(tCliente* c)
{
    if (c) {
        return c->nomeCompleto;
    }
    return NULL;
}
tSacola* getSacolaCliente(tCliente* c)
{
    if (c) {
        return c->sacola;
    }
    return NULL;
}
/*
 * Esvazia/limpa uma estrutura (ex.: sacola), mantendo-a pronta para reuso.
 */
void esvaziaSacolaCliente(tCliente* c){
    if(!c || !c->sacola) return;
    sacolaEsvazia(c->sacola);
}