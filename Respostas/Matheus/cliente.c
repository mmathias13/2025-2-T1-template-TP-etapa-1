#include "cliente.h"

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
};

tCliente* criaCliente(char* nome, char* cpf, char* data, char* telefone, char* endereco, char* email, char* username, char* senha, char* cartao){

    tCliente *c = (tCliente*)malloc(sizeof(tCliente));

    c->nomeCompleto = nome;

    return c;
}

void liberaCliente(tCliente* c){
    free(c);
}

char* getCpfCliente(tCliente* c){
    return c->cpf;
}
char* getEmailCliente(tCliente* c){
    return c->email;
}

char* getNomeCliente(tCliente* c){
    return c->nomeCompleto;
}