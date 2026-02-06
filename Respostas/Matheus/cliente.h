#ifndef _CLIENTE_H
#define _CLIENTE_H

typedef struct Cliente tCliente;

tCliente* criaCliente(char* nome, char* cpf, char* data, char* telefone, char* endereco, char* email, char* username, char* senha, char* cartao);

void liberaCliente(tCliente* c);

char* getCpfCliente();
char* getEmailCliente();
char* getNomeCliente();

#endif