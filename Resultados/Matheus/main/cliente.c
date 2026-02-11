#include "cliente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void liberaCliente(tCliente* c){
    if(!c) return;
    liberaSacola(c->sacola);
    free(c);
}

char* getCpfCliente(tCliente* c){
    return c ? c->cpf : NULL;
}
char* getEmailCliente(tCliente* c){
    return c ? c->email : NULL;
}
char* getNomeCliente(tCliente* c){
    return c ? c->nomeCompleto : NULL;
}

tSacola* getSacolaCliente(tCliente* c){
    return c ? c->sacola : NULL;
}

void esvaziaSacolaCliente(tCliente* c){
    if(!c || !c->sacola) return;
    sacolaEsvazia(c->sacola);
}