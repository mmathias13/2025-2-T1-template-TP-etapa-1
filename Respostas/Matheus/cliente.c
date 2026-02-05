#include "cliente.h"

struct cliente{

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
    tProduto **produtos;
};