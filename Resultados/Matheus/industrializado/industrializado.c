#include "industrializado.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LINHA_TAM 1024

struct Industrializado {
    char cod[16];
    char nome[151];
    char desc[151];
    char tipoInd[4]; /* "PER" ou "NPE" */
    int qtdDisponivel;
    float valor;
};

static void removeQuebraLinha(char *s)
{
    if (!s) return;
    s[strcspn(s, "\n")] = '\0';
}

static char *trimLocal(char *s)
{
    if (!s) return s;
    while (*s && isspace((unsigned char)*s)) s++;

    char *end = s + strlen(s);
    while (end > s && isspace((unsigned char)end[-1])) end--;
    *end = '\0';
    return s;
}

static int splitPorPontoEVirgula(char *linha, char **campos, int maxCampos)
{
    int n = 0;
    char *tok = strtok(linha, ";");
    while (tok && n < maxCampos) {
        campos[n++] = trimLocal(tok);
        tok = strtok(NULL, ";");
    }
    return n;
}

static int camposValidos(char **campos, int n)
{
    for (int i = 0; i < n; i++) {
        if (!campos[i] || campos[i][0] == '\0') return 0;
    }
    return 1;
}

static int parseIntLocal(char *s, int *out)
{
    if (!s) return 0;
    s = trimLocal(s);
    if (s[0] == '\0') return 0;

    char *end = NULL;
    long v = strtol(s, &end, 10);
    if (end == s) return 0;
    while (*end && isspace((unsigned char)*end)) end++;
    if (*end != '\0') return 0;

    *out = (int)v;
    return 1;
}

static int parseFloatLocal(char *s, float *out)
{
    if (!s) return 0;
    s = trimLocal(s);
    if (s[0] == '\0') return 0;

    char *end = NULL;
    double v = strtod(s, &end);
    if (end == s) return 0;
    while (*end && isspace((unsigned char)*end)) end++;
    if (*end != '\0') return 0;

    *out = (float)v;
    return 1;
}

/* Le 1 linha no formato:
   ID; NOME; DESCRICAO; VALOR; TIPO_IND(PER/NPE); QUANTIDADE
*/
tIndustrializado *criaIndustrializado()
{
    char linha[LINHA_TAM];
    if (!fgets(linha, sizeof(linha), stdin)) return NULL;
    removeQuebraLinha(linha);

    char *p = trimLocal(linha);
    if (strcmp(p, "OUT") == 0) return NULL;

    char buf[LINHA_TAM];
    snprintf(buf, sizeof(buf), "%s", p);

    char *campos[8];
    int n = splitPorPontoEVirgula(buf, campos, 8);
    if (n < 6 || !camposValidos(campos, 6)) return NULL;

    float valor = 0.0f;
    int qtd = 0;
    if (!parseFloatLocal(campos[3], &valor) || !parseIntLocal(campos[5], &qtd)) return NULL;

    tIndustrializado *i = (tIndustrializado *)malloc(sizeof(tIndustrializado));
    if (!i) return NULL;

    strncpy(i->cod, campos[0], sizeof(i->cod) - 1);
    i->cod[sizeof(i->cod) - 1] = '\0';

    strncpy(i->nome, campos[1], sizeof(i->nome) - 1);
    i->nome[sizeof(i->nome) - 1] = '\0';

    strncpy(i->desc, campos[2], sizeof(i->desc) - 1);
    i->desc[sizeof(i->desc) - 1] = '\0';

    strncpy(i->tipoInd, campos[4], sizeof(i->tipoInd) - 1);
    i->tipoInd[sizeof(i->tipoInd) - 1] = '\0';

    i->valor = valor;
    i->qtdDisponivel = qtd;

    return i;
}

/* ===== Callbacks e utilitarias ===== */

void imprimeFisicoIndustrializado(void *dado, int qtd)
{
    tIndustrializado *i = (tIndustrializado *)dado;
    if (!i) return;

    printf("IMPRESSAO NF FISICA DE INDUSTRIALIZADO:\n");
    printf("ID DO INDUSTRIALIZADO: %s\n", i->cod);
    printf("NOME DO INDUSTRIALIZADO: %s\n", i->nome);
    printf("DESCRICAO DO INDUSTRIALIZADO: %s\n", i->desc);
    printf("VALOR DO INDUSTRIALIZADO: %.2f\n", i->valor);
    printf("QUANTIDADE: %d\n", qtd);
    printf("VALOR FINAL DO INDUSTRIALIZADO: %.2f\n\n", i->valor * (float)qtd);
}

void imprimeDigitalIndustrializado(void *dado, char *email, int qtd)
{
    tIndustrializado *i = (tIndustrializado *)dado;
    if (!i) return;

    printf("IMPRESSAO NF DIGITAL DE INDUSTRIALIZADO:\n");
    printf("E-MAIL DE ENVIO: %s\n", email ? email : "");
    printf("ID DO INDUSTRIALIZADO: %s\n", i->cod);
    printf("NOME DO INDUSTRIALIZADO: %s\n", i->nome);
    printf("DESCRICAO DO INDUSTRIALIZADO: %s\n", i->desc);
    printf("VALOR DO INDUSTRIALIZADO: %.2f\n", i->valor);
    printf("QUANTIDADE: %d\n", qtd);
    printf("VALOR FINAL DO INDUSTRIALIZADO: %.2f\n", i->valor * (float)qtd);
}

float getValorIndustrializado(void *dado)
{
    tIndustrializado *i = (tIndustrializado *)dado;
    if (!i) return 0.0f;
    return i->valor;
}

char *getCodIndustrializado(void *dado)
{
    tIndustrializado *i = (tIndustrializado *)dado;
    if (!i) return NULL;
    return i->cod;
}

char *getDescIndustrializado(void *dado)
{
    tIndustrializado *i = (tIndustrializado *)dado;
    if (!i) return NULL;
    return i->desc;
}

char getTipoIndustrializado(void *dado)
{
    (void)dado;
    return 'I';
}

char *getNomeIndustrializado(void *dado)
{
    tIndustrializado *i = (tIndustrializado *)dado;
    if (!i) return NULL;
    return i->nome;
}

int getDisponibilidadeIndustrializado(void *dado, int qtd)
{
    tIndustrializado *i = (tIndustrializado *)dado;
    if (!i) return 0;
    return (i->qtdDisponivel >= qtd);
}

void alteraDisponibilidadeIndustrializado(void *dado, int qtd)
{
    tIndustrializado *i = (tIndustrializado *)dado;
    if (!i) return;
    i->qtdDisponivel -= qtd;
    if (i->qtdDisponivel < 0) i->qtdDisponivel = 0;
}

void printaIndustrializado(void *dado)
{
    tIndustrializado *i = (tIndustrializado *)dado;
    if (!i) return;

    /* produto.c anexa a média de avaliações */
    printf("%s, %s, %s, %s, %c, R$%.2f.",
           i->nome,
           i->cod,
           i->desc,
           i->tipoInd,
           'I',
           i->valor);
}

void liberaIndustrializado(void *industrializado)
{
    tIndustrializado *i = (tIndustrializado *)industrializado;
    if (!i) return;
    free(i);
}
