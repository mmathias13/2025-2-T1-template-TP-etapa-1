#include "fornecedor.h"
#include "ingrediente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Módulo do Fornecedor: cadastro e controle de estoque de ingredientes.

struct Fornecedor {
    char nomeFornecedor[101];
    char cnpjFornecedor[19];
    char telefoneFornecedor[15];
    char enderecoFornecedor[151];
    char nomeUsuarioFornecedor[16];
    char senhaFornecedor[21];

    int numIngredientes;
    tIngrediente **ingredientes;
};

tFornecedor* criaFornecedor(char* nome, char* cnpj, char* telefone,
                            char* endereco, char* username, char* senha) {
    tFornecedor *f = (tFornecedor*)malloc(sizeof(tFornecedor));
    if (!f) {
        printf("Problema na alocação de FORNECEDOR!\n");
        return NULL;
    }

    strcpy(f->nomeFornecedor, nome);
    strcpy(f->cnpjFornecedor, cnpj);
    strcpy(f->telefoneFornecedor, telefone);
    strcpy(f->enderecoFornecedor, endereco);
    strcpy(f->nomeUsuarioFornecedor, username);
    strcpy(f->senhaFornecedor, senha);

    f->numIngredientes = 0;
    f->ingredientes = NULL;

    return f;
}

/*
 * Libera a memória e recursos associados a liberaFornecedor.
 */
void liberaFornecedor(tFornecedor *f) {
    if (!f) return;

    for (int i = 0; i < f->numIngredientes; i++) {
        liberaIngrediente(f->ingredientes[i]);
    }
    free(f->ingredientes);
    free(f);
}

/*
 * Função de busca: localiza um item a partir de uma chave (ex.: id/nome).
 */
int buscaIngredienteFornecedor(tFornecedor *f, int codigoBarras) {
    if (!f) return -1;

    for (int i = 0; i < f->numIngredientes; i++) {
        if (getCodigoBarrasIngrediente(f->ingredientes[i]) == codigoBarras) {
            return i;
        }
    }
    return -1;
}

/*
 * Função cadastraOuAtualizaIngredienteFornecedor.
 */
void cadastraOuAtualizaIngredienteFornecedor(tFornecedor *f, tIngrediente *ing) {
    if (!f || !ing) return;

    int codigo = getCodigoBarrasIngrediente(ing);
    int idx = buscaIngredienteFornecedor(f, codigo);

    if (idx != -1) {
        int qtd = getQuantidadeIngrediente(ing);
        adicionaQuantidadeIngrediente(f->ingredientes[idx], qtd);
        liberaIngrediente(ing);
        return;
    }

    tIngrediente **novo = realloc(
        f->ingredientes,
        (f->numIngredientes + 1) * sizeof(tIngrediente*)
    );

    if (!novo) {
        liberaIngrediente(ing);
        return;
    }

    f->ingredientes = novo;
    f->ingredientes[f->numIngredientes] = ing;
    f->numIngredientes++;
}

/* ===== NOVOS HELPERS POR NOME ===== */

tIngrediente* buscaIngredienteFornecedorPorNome(tFornecedor* f, const char* nome){
    if(!f || !nome || nome[0]=='\0') return NULL;
    for(int i=0;i<f->numIngredientes;i++){
        tIngrediente* ing = f->ingredientes[i];
        if(ing && strcmp(getNomeIngrediente(ing), nome) == 0) return ing;
    }
    return NULL;
}

/*
 * Função fornecedorTemIngredienteQtd.
 */
int fornecedorTemIngredienteQtd(tFornecedor* f, const char* nome, int qtd){
    if(!f || !nome || qtd <= 0) return 0;
    tIngrediente* ing = buscaIngredienteFornecedorPorNome(f, nome);
    if(!ing) return 0;
    return getQuantidadeIngrediente(ing) >= qtd;
}

/*
 * Função fornecedorConsomeIngredienteQtd.
 */
int fornecedorConsomeIngredienteQtd(tFornecedor* f, const char* nome, int qtd){
    if(!fornecedorTemIngredienteQtd(f, nome, qtd)) return 0;
    tIngrediente* ing = buscaIngredienteFornecedorPorNome(f, nome);
    consomeQuantidadeIngrediente(ing, qtd);
    return 1;
}

/* ===== Getters ===== */

char* getEnderecoFornecedor(tFornecedor *f) { return f->enderecoFornecedor; }
/*
 * Acessor: retorna a informação pedida por getTelefoneFornecedor.
 */
char* getTelefoneFornecedor(tFornecedor *f) { return f->telefoneFornecedor; }
/*
 * Acessor: retorna a informação pedida por getUsuarioFornecedor.
 */
char* getUsuarioFornecedor(tFornecedor *f) { return f->nomeUsuarioFornecedor; }
/*
 * Acessor: retorna a informação pedida por getNomeFornecedor.
 */
char* getNomeFornecedor(tFornecedor *f) { return f->nomeFornecedor; }
/*
 * Acessor: retorna a informação pedida por getCnpjFornecedor.
 */
char* getCnpjFornecedor(tFornecedor *f) { return f->cnpjFornecedor; }

int getNumIngredientesFornecedor(tFornecedor *f)
{
    if (f) {
        return f->numIngredientes;
    }
    return 0;
}
/*
 * Acessor: retorna a informação pedida por getIngredientePorIndiceFornecedor.
 */
tIngrediente* getIngredientePorIndiceFornecedor(tFornecedor *f, int idx) {
    if (!f || idx < 0 || idx >= f->numIngredientes) return NULL;
    return f->ingredientes[idx];
}