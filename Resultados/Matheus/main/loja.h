#ifndef _LOJA_H
#define _LOJA_H

typedef struct Loja tLoja;

tLoja* criaLoja(char* nome, char* cnpj, char* telefone, char* endereco, char* username, char* senha);

void liberaLoja(tLoja *l);

char* getEnderecoLoja(tLoja *l);
char* getTelefoneLoja(tLoja *l);
char* getUsuarioLoja(tLoja *l);
char* getNomeLoja(tLoja *l);
char* getCnpjLoja(tLoja *l);

#endif
