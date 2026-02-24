#include "prato.h"
#include "fornecedor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Produto do tipo prato: depende de ingredientes e precisa validar/consumir estoque ao vender.

struct Prato {
    char cod[64];
    char nome[128];
    char desc[256];
    float valor;

    char** ingredientes;
    int nIng;

    tSistema* sistema;
};

static char* dupStr(const char* s){
    if(!s) s = "";
    size_t n = strlen(s);
    char* out = (char*)malloc(n+1);
    if(!out) return NULL;
    strcpy(out, s);
    return out;
}

static char** dupLista(char** v, int n){
    if(n <= 0) return NULL;
    char** out = (char**)malloc(n * sizeof(char*));
    if(!out) return NULL;

    for(int i=0;i<n;i++){
        out[i] = dupStr(v[i]);
        if(!out[i]){
            for(int j=0;j<i;j++) free(out[j]);
            free(out);
            return NULL;
        }
    }
    return out;
}

/* regra: precisa existir UM fornecedor que tenha qtd para o ingrediente */
static int encontraFornecedorParaIngrediente(tSistema* s, const char* nomeIng, int qtd, tFornecedor** outF){
    int nforn = getNumFornecedoresSistema(s);
    for(int i=0;i<nforn;i++){
        tFornecedor* f = getFornecedorSistema(s, i);
        if(!f) continue;
        if(fornecedorTemIngredienteQtd(f, nomeIng, qtd)){
            if(outF) *outF = f;
            return 1;
        }
    }
    return 0;
}

tPrato* criaPrato(tSistema* sistema, char* cod, char* nome, char* desc, float valor,
                  int nIng, char** ingredientes)
{
    tPrato* p = (tPrato*)malloc(sizeof(*p));
    if(!p) return NULL;

    p->sistema = sistema;
    p->valor = valor;
    p->nIng = nIng;

    strncpy(p->cod, cod ? cod : "", sizeof(p->cod)-1);
    p->cod[sizeof(p->cod)-1] = '\0';

    strncpy(p->nome, nome ? nome : "", sizeof(p->nome)-1);
    p->nome[sizeof(p->nome)-1] = '\0';

    strncpy(p->desc, desc ? desc : "", sizeof(p->desc)-1);
    p->desc[sizeof(p->desc)-1] = '\0';

    p->ingredientes = dupLista(ingredientes, nIng);
    if(nIng > 0 && !p->ingredientes){
        free(p);
        return NULL;
    }

    return p;
}

/*
 * Libera a memória e recursos associados a liberaPrato.
 */
void liberaPrato(void* dado){
    tPrato* p = (tPrato*)dado;
    if(!p) return;
    for(int i=0;i<p->nIng;i++) free(p->ingredientes[i]);
    free(p->ingredientes);
    free(p);
}

/*
 * Acessor: retorna a informação pedida por getValorPrato.
 */
float getValorPrato(void* dado){
    tPrato* p = (tPrato*)dado;
    return p ? p->valor : 0.0f;
}

/*
 * Acessor: retorna a informação pedida por getCodPrato.
 */
char* getCodPrato(void* dado){
    tPrato* p = (tPrato*)dado;
    return p ? p->cod : NULL;
}

/*
 * Acessor: retorna a informação pedida por getNomePrato.
 */
char* getNomePrato(void* dado){
    tPrato* p = (tPrato*)dado;
    return p ? p->nome : NULL;
}

/*
 * Acessor: retorna a informação pedida por getTipoPrato.
 */
char getTipoPrato(void* dado){
    (void)dado;
    return 'P';
}

/*
 * Acessor: retorna a informação pedida por getDescPrato.
 */
char* getDescPrato(void* dado){
    tPrato* p = (tPrato*)dado;
    return p ? p->desc : NULL;
}

/*
 * Acessor: retorna a informação pedida por getDisponibilidadePrato.
 */
int getDisponibilidadePrato(void* dado, int qtd){
    tPrato* p = (tPrato*)dado;
    if(!p || !p->sistema || qtd <= 0) return 0;

    for(int i=0;i<p->nIng;i++){
        tFornecedor* fEscolhido = NULL;
        if(!encontraFornecedorParaIngrediente(p->sistema, p->ingredientes[i], qtd, &fEscolhido)){
            return 0;
        }
    }
    return 1;
}

/*
 * Consome ingredientes necessários para atender uma compra de prato (atualiza
 * estoques).
 */
void alteraDisponibilidadePrato(void* dado, int qtd){
    tPrato* p = (tPrato*)dado;
    if(!p || !p->sistema || qtd <= 0) return;

    for(int i=0;i<p->nIng;i++){
        tFornecedor* fEscolhido = NULL;
        if(encontraFornecedorParaIngrediente(p->sistema, p->ingredientes[i], qtd, &fEscolhido)){
            fornecedorConsomeIngredienteQtd(fEscolhido, p->ingredientes[i], qtd);
        }
    }
}

/*
 * Imprime informações relacionadas a imprimeFisicoPrato no formato esperado.
 */
void imprimeFisicoPrato(void* dado, int qtd){
    tPrato* p = (tPrato*)dado;
    if(!p) return;

    printf("IMPRESSAO NF FISICA DE PRATO:\n");
    printf("ID DO PRATO: %s\n", p->cod);
    printf("NOME DO PRATO: %s\n", p->nome);
    printf("DESCRICAO DO PRATO: %s\n", p->desc);
    printf("VALOR DO PRATO: %.2f\n", p->valor);
    printf("QUANTIDADE: %d\n", qtd);
    printf("VALOR FINAL DO PRATO: %.2f\n\n", p->valor * (float)qtd);
}

/*
 * Imprime informações relacionadas a imprimeDigitalPrato no formato esperado.
 */
void imprimeDigitalPrato(void* dado, char* email, int qtd){
    tPrato* p = (tPrato*)dado;
    if(!p) return;

    printf("IMPRESSAO NF DIGITAL DE PRATO:\n");
    printf("E-MAIL DE ENVIO: %s\n", email ? email : "");
    printf("ID DO PRATO: %s\n", p->cod);
    printf("NOME DO PRATO: %s\n", p->nome);
    printf("DESCRICAO DO PRATO: %s\n", p->desc);
    printf("VALOR DO PRATO: %.2f\n", p->valor);
    printf("QUANTIDADE: %d\n", qtd);
    printf("VALOR FINAL DO PRATO: %.2f\n", p->valor * (float)qtd);
}

/*
 * Imprime informações relacionadas a printaPrato no formato esperado.
 */
void printaPrato(void* dado){
    tPrato* p = (tPrato*)dado;
    if(!p) return;

    /* imprime o miolo (sem loja); produto.c completa com a avaliação média */
    printf("%s, %s, %s, %c, R$%.2f. INGREDIENTES: ",
           p->nome, p->cod, p->desc, 'P', p->valor);

    for(int i=0;i<p->nIng;i++){
        printf("%s", p->ingredientes[i]);
        if(i < p->nIng-1) printf(", ");
    }
    printf(".");
}