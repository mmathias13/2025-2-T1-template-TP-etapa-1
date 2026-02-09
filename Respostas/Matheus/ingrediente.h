#ifndef _INGREDIENTE_H
#define _INGREDIENTE_H

typedef struct Ingrediente tIngrediente;

tIngrediente* criaIngrediente(char* nome, int codigo, char tipo, int quantidade);

void liberaIngrediente(tIngrediente* i);

char* getNomeIngrediente(tIngrediente* i);
char getTipoIngrediente(tIngrediente* i);
int getCodigoBarrasIngrediente(tIngrediente *i);
int getQuantidadeIngrediente(tIngrediente *i);



#endif