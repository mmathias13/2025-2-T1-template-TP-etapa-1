#include <stdio.h>
#include "ingrediente.h"

int main(void) {
    tIngrediente *i = criaIngrediente("TOMATE", 123456, 'F', 50);
    if (!i) {
        printf("Erro ao criar ingrediente.\n");
        return 1;
    }

    printf("=== Ingrediente ===\n");
    printf("Nome: %s\n", getNomeIngrediente(i));
    printf("Codigo: %d\n", getCodigoBarrasIngrediente(i));
    printf("Tipo: %c\n", getTipoIngrediente(i));
    printf("Quantidade: %d\n", getQuantidadeIngrediente(i));

    liberaIngrediente(i);
    return 0;
}