#include "avaliacao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Avaliacao{

    char cpf[15];
    char comentario[201];
    int nota;
};


/*
Função que cria uma nova avaliação a ser associada a um produto cadastrado por uma loja.
    Deve-se criar um ponteiro do tipo tAvaliacao, ler os atributos de uma avaliação (comentário
    e nota) e deve-se associar o CPF passado como parâmetro à avaliação criada. Retorna-se o 
    ponteiro de tAvaliacao criado.

@param char *cpf: CPF do usuário criando a avaliação.
@return tAvaliacao*: ponteiro para o tipo tAvaliacao criado.
*/
tAvaliacao * criaAvaliacao(char * cpfUsuario){
    tAvaliacao *a = (tAvaliacao*)malloc(sizeof(tAvaliacao));
    if (a == NULL){
        printf("Erro de alocação na AVALIAÇÃO!");
        return NULL;
    }
    strcpy(a->cpf, cpfUsuario);
    a->comentario[0] = '\0';
    a->nota = 0;

    //Loop para leitura e processamento dos comentários
    char buffer[300];

    while (1) {
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        // EOF/erro: não dá pra continuar lendo avaliação
        liberaAvaliacao(a);
        return NULL;
    }

    // remove '\n' se existir
    buffer[strcspn(buffer, "\n")] = '\0';

    // valida existência de ';'
    char *sep = strchr(buffer, ';');
    if (sep == NULL) {
        printf("AVALIACAO INVALIDA! FAVOR INICIAR A AVALIACAO NOVAMENTE!\n");
        continue;
    }

    // daqui pra frente você vai separar comentario/nota e validar
    break;
}
    

    return a;
}

/*
Função que retorna a string comentario da Avaliação passada como parâmetro. 

@param tAvaliacao *a: ponteiro do tipo tAvaliacao que deseja-se recuperar o comentário.
*/
char * getComentarioAvaliacao(tAvaliacao * a){
    return a->comentario;
}


/*
Função que retorna a nota da Avaliação passada como parâmetro.

@param tAvaliacao *a: ponteiro do tipo tAvaliacao que deseja-se recuperar a nota.
*/
int getNotaAvaliacao(tAvaliacao * a){
    return a->nota;
}

/*
Função que retorna o CPF do cliente que realizou a avaliação.

@param tAvaliacao *a: ponteiro do tipo tAvaliacao que deseja-se recuperar o CPF do cliente.
*/
char * getCpfClienteAvaliacao(tAvaliacao * a){
    return a->cpf;
}

/*
Função que libera espaço de memória alocado para uma avaliação. Se a avaliação for diferente de NULL,
    deve-se liberar o espaço alocado para ela.

@param tAvaliacao *a: ponteiro do tipo tAvaliacao a ser liberado.
*/
void liberaAvaliacao(tAvaliacao * a){
    free(a);
}