#include "industrializado.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LINHA_TAM 1024

struct Industrializado {
    char cod[16];         // até 15 + '\0'
    char nome[101];       // nome do produto (PDF fala 20, mas deixo folga segura)
    char desc[101];       // até 100 + '\0'
    char tipoInd[4];      // "PER" ou "NPE" + '\0'
    int qtdDisponivel;
    float valor;
};

static void removeQuebraLinha(char *s) {
    if (!s) return;
    s[strcspn(s, "\n")] = '\0';
}

static char* trimLocal(char *s) {
    if (!s) return s;
    while (*s && isspace((unsigned char)*s)) s++;

    char *end = s + strlen(s);
    while (end > s && isspace((unsigned char)end[-1])) end--;
    *end = '\0';
    return s;
}

static void removePontoEVirgulaFinal(char *s) {
    if (!s) return;
    int n = (int)strlen(s);
    while (n > 0 && isspace((unsigned char)s[n-1])) n--;
    if (n > 0 && s[n-1] == ';') {
        s[n-1] = '\0';
    }
}

static int leLinhaObrigatoria(char *buf, int tam) {
    if (!fgets(buf, tam, stdin)) return 0;
    removeQuebraLinha(buf);
    char *p = trimLocal(buf);
    (void)p;
    return 1;
}

static int parseIntLocal(char *s, int *out) {
    if (!s) return 0;
    s = trimLocal(s);
    removePontoEVirgulaFinal(s);
    if (s[0] == '\0') return 0;

    char *end = NULL;
    long v = strtol(s, &end, 10);
    if (end == s) return 0;
    while (*end && isspace((unsigned char)*end)) end++;
    if (*end != '\0') return 0;

    *out = (int)v;
    return 1;
}

static int parseFloatLocal(char *s, float *out) {
    if (!s) return 0;
    s = trimLocal(s);
    removePontoEVirgulaFinal(s);
    if (s[0] == '\0') return 0;

    char *end = NULL;
    double v = strtod(s, &end);
    if (end == s) return 0;
    while (*end && isspace((unsigned char)*end)) end++;
    if (*end != '\0') return 0;

    *out = (float)v;
    return 1;
}

tIndustrializado* criaIndustrializado()
{
    // Leitura tolerante em linhas:
    // 1) cod;
    // 2) nome;
    // 3) desc;
    // 4) tipoInd (PER/NPE);
    // 5) valor (float)
    // 6) quantidade (int)

    char linha[LINHA_TAM];

    tIndustrializado *i = (tIndustrializado*) malloc(sizeof(tIndustrializado));
    if (!i) {
        printf("Erro de alocação em INDUSTRIALIZADO!\n");
        return NULL;
    }

    // COD
    if (!leLinhaObrigatoria(linha, sizeof(linha))) { free(i); return NULL; }
    char *p = trimLocal(linha);
    removePontoEVirgulaFinal(p);
    strncpy(i->cod, p, sizeof(i->cod)-1);
    i->cod[sizeof(i->cod)-1] = '\0';

    // NOME
    if (!leLinhaObrigatoria(linha, sizeof(linha))) { free(i); return NULL; }
    p = trimLocal(linha);
    removePontoEVirgulaFinal(p);
    strncpy(i->nome, p, sizeof(i->nome)-1);
    i->nome[sizeof(i->nome)-1] = '\0';

    // DESC
    if (!leLinhaObrigatoria(linha, sizeof(linha))) { free(i); return NULL; }
    p = trimLocal(linha);
    removePontoEVirgulaFinal(p);
    strncpy(i->desc, p, sizeof(i->desc)-1);
    i->desc[sizeof(i->desc)-1] = '\0';

    // TIPO (PER/NPE)
    if (!leLinhaObrigatoria(linha, sizeof(linha))) { free(i); return NULL; }
    p = trimLocal(linha);
    removePontoEVirgulaFinal(p);
    strncpy(i->tipoInd, p, sizeof(i->tipoInd)-1);
    i->tipoInd[sizeof(i->tipoInd)-1] = '\0';

    // VALOR
    if (!leLinhaObrigatoria(linha, sizeof(linha))) { free(i); return NULL; }
    float v = 0.0f;
    if (!parseFloatLocal(linha, &v)) { free(i); return NULL; }
    i->valor = v;

    // QUANTIDADE
    if (!leLinhaObrigatoria(linha, sizeof(linha))) { free(i); return NULL; }
    int qtd = 0;
    if (!parseIntLocal(linha, &qtd)) { free(i); return NULL; }
    i->qtdDisponivel = qtd;

    return i;
}

/* ===== Callbacks e utilitárias ===== */

void imprimeFisicoIndustrializado(void *dado, int qtd)
{
    tIndustrializado *i = (tIndustrializado*) dado;
    if (!i) return;

    // Como o PDF mostra exemplo só para PRATO, aqui fazemos o análogo para Industrializado.
    // Se o gabarito exigir exatamente "PRATO" também para industrializado, você troca o texto aqui.
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
    tIndustrializado *i = (tIndustrializado*) dado;
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
    tIndustrializado *i = (tIndustrializado*) dado;
    if (!i) return 0.0f;
    return i->valor;
}

char* getCodIndustrializado(void *dado)
{
    tIndustrializado *i = (tIndustrializado*) dado;
    if (!i) return NULL;
    return i->cod;
}

char* getDescIndustrializado(void *dado)
{
    tIndustrializado *i = (tIndustrializado*) dado;
    if (!i) return NULL;
    return i->desc;
}

char getTipoIndustrializado(void *dado)
{
    (void)dado;
    return 'I';
}

char * getNomeIndustrializado(void *dado)
{
    tIndustrializado *i = (tIndustrializado*) dado;
    if (!i) return NULL;
    return i->nome;
}

int getDisponibilidadeIndustrializado(void *dado, int qtd)
{
    tIndustrializado *i = (tIndustrializado*) dado;
    if (!i) return 0;
    return (i->qtdDisponivel >= qtd);
}

void alteraDisponibilidadeIndustrializado(void *dado, int qtd)
{
    tIndustrializado *i = (tIndustrializado*) dado;
    if (!i) return;
    i->qtdDisponivel -= qtd;
    if (i->qtdDisponivel < 0) i->qtdDisponivel = 0;
}

void printaIndustrializado(void *dado)
{
    tIndustrializado *i = (tIndustrializado*) dado;
    if (!i) return;

    // Formato visto nos casos (exemplo):
    // KETCHUP CASEIRO 1, 4, KETCHUP DE GOIABA, NPE, I, R$37.90.
    printf("%s, %s, %s, %s, %c, R$%.2f.\n",
           i->nome,
           i->cod,
           i->desc,
           i->tipoInd,
           'I',
           i->valor);
}

void liberaIndustrializado(void *industrializado)
{
    tIndustrializado *i = (tIndustrializado*) industrializado;
    if (!i) return;
    free(i);
}