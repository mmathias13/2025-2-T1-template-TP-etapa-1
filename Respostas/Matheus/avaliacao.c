#include "avaliacao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Avaliacao{
    char cpf[15];
    char comentario[201];
    int nota;
};

static char* trim(char *s) {
    if (!s) return s;

    // trim esquerda
    while (*s && isspace((unsigned char)*s)) s++;

    // trim direita
    char *end = s + strlen(s);
    while (end > s && isspace((unsigned char)end[-1])) end--;
    *end = '\0';

    return s;
}

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

        *sep = '\0';
        char *coment = trim(buffer);
        char *notaStr = trim(sep + 1);

        if (coment[0] == '\0' || notaStr[0] == '\0'){
            printf("AVALIACAO INVALIDA! FAVOR INICIAR A AVALIACAO NOVAMENTE!\n");
            continue;
        }

        //converte nota e valida formato
        char *endptr = NULL;
        long nota = strtol(notaStr, &endptr, 10);

        //endptr deve parar no fim da string (sem lixo)
        endptr = trim(endptr);
        if (*endptr != '\0' || nota < 1 || nota > 5){
            printf("AVALIACAO INVALIDA! FAVOR INICIAR A AVALIACAO NOVAMENTE!\n");
            continue;
        }

        // copia para a struct (arrays fixos)
        strncpy(a->comentario, coment, sizeof(a->comentario) - 1);
        a->comentario[sizeof(a->comentario) - 1] = '\0';
        a->nota = (int)nota;

        break;
    }

    return a;
}

char * getComentarioAvaliacao(tAvaliacao * a){
    return a->comentario;
}

int getNotaAvaliacao(tAvaliacao * a){
    return a->nota;
}

char * getCpfClienteAvaliacao(tAvaliacao * a){
    return a->cpf;
}

void liberaAvaliacao(tAvaliacao * a){
    if (!a) return;
    free(a);
}