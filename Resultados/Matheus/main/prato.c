#include "prato.h"
#include "sistema.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Prato{
    tSistema* s;            // NÃO é dono
    char cod[16];
    char nome[101];
    float preco;

    char** ingredientes;    // nomes (dup)
    int nIng;

    char descVazia[1];      // pra getDescPrato devolver algo não-NULL
};

static char* dupstr(const char* s){
    if(!s) s = "";
    size_t n = strlen(s);
    char* d = (char*)malloc(n+1);
    if(!d) return NULL;
    memcpy(d, s, n+1);
    return d;
}

tPrato* criaPrato(tSistema* s, char* cod, char* nome, float preco, int nIng, char** nomesIng){
    if(!s || !cod || !nome || nIng <= 0 || !nomesIng) return NULL;

    tPrato* p = (tPrato*)malloc(sizeof *p);
    if(!p) return NULL;

    p->s = s;
    snprintf(p->cod, sizeof p->cod, "%s", cod);
    snprintf(p->nome, sizeof p->nome, "%s", nome);
    p->preco = preco;

    p->ingredientes = (char**)malloc(nIng * sizeof(char*));
    if(!p->ingredientes){ free(p); return NULL; }

    p->nIng = nIng;
    for(int i=0;i<nIng;i++){
        p->ingredientes[i] = dupstr(nomesIng[i]);
        if(!p->ingredientes[i]){
            for(int j=0;j<i;j++) free(p->ingredientes[j]);
            free(p->ingredientes);
            free(p);
            return NULL;
        }
    }

    p->descVazia[0] = '\0';
    return p;
}

void liberaPrato(void* dado){
    tPrato* p = (tPrato*)dado;
    if(!p) return;

    for(int i=0;i<p->nIng;i++) free(p->ingredientes[i]);
    free(p->ingredientes);
    free(p);
}

/* ===== callbacks ===== */
float getValorPrato(void* dado){ tPrato* p=(tPrato*)dado; return p? p->preco : 0.0f; }
char* getCodPrato(void* dado){ tPrato* p=(tPrato*)dado; return p? p->cod : NULL; }
char* getNomePrato(void* dado){ tPrato* p=(tPrato*)dado; return p? p->nome : NULL; }
char  getTipoPrato(void* dado){ (void)dado; return 'P'; }
char* getDescPrato(void* dado){
    tPrato* p=(tPrato*)dado;
    return p? p->descVazia : NULL;
}

int getDisponibilidadePrato(void* dado, int qtd){
    tPrato* p = (tPrato*)dado;
    if(!p || qtd <= 0) return 0;

    // comprar "qtd" pratos consome "qtd" unidades de CADA ingrediente
    for(int i=0;i<p->nIng;i++){
        if(!buscaIngredienteDisponivelPorNome(p->s, p->ingredientes[i], qtd)){
            return 0;
        }
    }
    return 1;
}

void alteraDisponibilidadePrato(void* dado, int qtd){
    tPrato* p = (tPrato*)dado;
    if(!p || qtd <= 0) return;

    // debita determinístico (primeiro fornecedor que tem)
    for(int i=0;i<p->nIng;i++){
        consomeIngredientePorNome(p->s, p->ingredientes[i], qtd);
    }
}

/* impressões (ajuste o texto quando você rodar os casos do professor) */
void imprimeFisicoPrato(void* dado, int qtd){
    tPrato* p = (tPrato*)dado;
    if(!p) return;

    printf("IMPRESSAO NF FISICA DE PRATO:\n");
    printf("ID DO PRATO: %s\n", p->cod);
    printf("NOME DO PRATO: %s\n", p->nome);
    printf("VALOR DO PRATO: %.2f\n", p->preco);
    printf("QUANTIDADE: %d\n", qtd);
    printf("VALOR FINAL DO PRATO: %.2f\n\n", p->preco * (float)qtd);
}

void imprimeDigitalPrato(void* dado, char* email, int qtd){
    tPrato* p = (tPrato*)dado;
    if(!p) return;

    printf("IMPRESSAO NF DIGITAL DE PRATO:\n");
    printf("E-MAIL DE ENVIO: %s\n", email ? email : "");
    printf("ID DO PRATO: %s\n", p->cod);
    printf("NOME DO PRATO: %s\n", p->nome);
    printf("VALOR DO PRATO: %.2f\n", p->preco);
    printf("QUANTIDADE: %d\n", qtd);
    printf("VALOR FINAL DO PRATO: %.2f\n", p->preco * (float)qtd);
}

void printaPrato(void* dado){
    tPrato* p = (tPrato*)dado;
    if(!p) return;

    // Formato de COP geralmente é rígido. Ajuste quando rodar diff.
    printf("%s, %s, %d INGREDIENTE(S), %c, R$%.2f.\n",
           p->nome, p->cod, p->nIng, 'P', p->preco);
}
