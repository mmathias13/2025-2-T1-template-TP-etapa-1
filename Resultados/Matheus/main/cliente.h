#ifndef _CLIENTE_H
#define _CLIENTE_H

#include "sacola.h"

typedef struct Cliente tCliente;

tCliente* criaCliente(char* nome, char* cpf, char* data, char* telefone, char* endereco,
                      char* email, char* username, char* senha, char* cartao);

void liberaCliente(tCliente* c);

char* getCpfCliente(tCliente* c);
char* getEmailCliente(tCliente* c);
char* getNomeCliente(tCliente* c);

/* sacola */
tSacola* getSacolaCliente(tCliente* c);
void esvaziaSacolaCliente(tCliente* c);

#endif