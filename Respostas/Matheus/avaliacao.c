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

static char* trimLocal(char *s) {
    if (!s) return s;
    while (*s && isspace((unsigned char)*s)) s++;

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

    char buffer[300];

    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            liberaAvaliacao(a);
            return NULL;
        }

        buffer[strcspn(buffer, "\n")] = '\0';
        char* linha = trimLocal(buffer);

        if(strcmp(linha, "OUT") == 0){
            liberaAvaliacao(a);
            return NULL; /* main interpreta como saída do sistema */
        }

        char *sep = strchr(linha, ';');
        if (sep == NULL) {
            printf("AVALIACAO INVALIDA! FAVOR INICIAR A AVALIACAO NOVAMENTE!\n");
            continue;
        }

        *sep = '\0';
        char *coment = trimLocal(linha);
        char *notaStr = trimLocal(sep + 1);

        if (coment[0] == '\0' || notaStr[0] == '\0'){
            printf("AVALIACAO INVALIDA! FAVOR INICIAR A AVALIACAO NOVAMENTE!\n");
            continue;
        }

        char *endptr = NULL;
        long nota = strtol(notaStr, &endptr, 10);
        endptr = trimLocal(endptr);

        if (*endptr != '\0' || nota < 1 || nota > 5){
            printf("AVALIACAO INVALIDA! FAVOR INICIAR A AVALIACAO NOVAMENTE!\n");
            continue;
        }

        strncpy(a->comentario, coment, sizeof(a->comentario) - 1);
        a->comentario[sizeof(a->comentario) - 1] = '\0';
        a->nota = (int)nota;

        break;
    }

    return a;
}

char * getComentarioAvaliacao(tAvaliacao * a){ return a ? a->comentario : NULL; }
int getNotaAvaliacao(tAvaliacao * a){ return a ? a->nota : 0; }
char * getCpfClienteAvaliacao(tAvaliacao * a){ return a ? a->cpf : NULL; }

void liberaAvaliacao(tAvaliacao * a){
    if (!a) return;
    free(a);
}