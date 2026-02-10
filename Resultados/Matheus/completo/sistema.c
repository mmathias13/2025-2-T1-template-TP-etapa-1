#include "sistema.h"
#include "ingrediente.h"
#include <stdlib.h>
#include <string.h>

struct Sistema
{
    tCliente **clientes;
    int numClientes;

    tLoja **lojas;
    int numLojas;

    tFornecedor **fornecedores;
    int numFornecedores;
};

static int stringVazia(char* s){
    return (s == NULL || s[0] == '\0');
}

//Ciclo de vida
tSistema* criaSistema(void){
    tSistema *s = (tSistema*)malloc(sizeof(tSistema));
    if (s == NULL){
        return NULL;
    }

    s->clientes = NULL;
    s->lojas = NULL;
    s->fornecedores = NULL;

    s->numClientes = 0;
    s->numLojas = 0;
    s->numFornecedores = 0;

    return s;
}

void liberaSistema(tSistema* s){
    if (s == NULL) return;

    //Liberar clientes
    for (int i = 0; i < s->numClientes; i++){
        if (s->clientes[i] != NULL) liberaCliente(s->clientes[i]);
    }
    free(s->clientes);

    //Liberar lojas
    for (int i = 0; i < s->numLojas; i++){
        if (s->lojas[i] != NULL) liberaLoja(s->lojas[i]);
    }
    free(s->lojas);

    //Liberar fornecedores
    for (int i = 0; i < s->numFornecedores; i++){
        if (s->fornecedores[i] != NULL) liberaFornecedor(s->fornecedores[i]);
    }
    free(s->fornecedores);

    free(s);
}

//CLIENTES
tCliente* buscaClientePorCPF(tSistema* s, char* cpf){
    if (s == NULL || stringVazia(cpf)) return NULL;

    for (int i = 0; i < s->numClientes; i++){
        char* cpfAtual = getCpfCliente(s->clientes[i]);
        if (cpfAtual != NULL && strcmp(cpfAtual, cpf) == 0){
            return s->clientes[i];
        }
    }
    return NULL;
}

tStatusSistema adicionaCliente(tSistema* s, tCliente* c){
    if (s == NULL || c == NULL) return SISTEMA_PARAM_INVALIDO;

    char* cpf = getCpfCliente(c);
    if (stringVazia(cpf)) return SISTEMA_PARAM_INVALIDO;

    if (buscaClientePorCPF(s, cpf) != NULL) return SISTEMA_DUPLICADO;

    tCliente** novo = (tCliente**)realloc(s->clientes, (s->numClientes + 1) * sizeof(tCliente*));
    if (novo == NULL) return SISTEMA_ERRO_ALOC;

    s->clientes = novo;
    s->clientes[s->numClientes] = c;
    s->numClientes++;

    return SISTEMA_OK;
}

int getNumClientesSistema(tSistema* s){
    if (s == NULL) return 0;
    return s->numClientes;
}

tCliente* getClienteSistema(tSistema* s, int idx){
    if (s == NULL) return NULL;
    if (idx < 0 || idx >= s->numClientes) return NULL;
    return s->clientes[idx];
}

//LOJAS
tLoja* buscaLojaPorCNPJ(tSistema* s, char* cnpj){
    if (s == NULL || stringVazia(cnpj)) return NULL;

    for (int i = 0; i < s->numLojas; i++){
        char* cnpjAtual = getCnpjLoja(s->lojas[i]);
        if (cnpjAtual != NULL && strcmp(cnpjAtual, cnpj) == 0){
            return s->lojas[i];
        }
    }
    return NULL;
}

tStatusSistema adicionaLoja(tSistema* s, tLoja* l){
    if (s == NULL || l == NULL) return SISTEMA_PARAM_INVALIDO;

    char* cnpj = getCnpjLoja(l);
    if (stringVazia(cnpj)) return SISTEMA_PARAM_INVALIDO;

    if (buscaLojaPorCNPJ(s, cnpj) != NULL) return SISTEMA_DUPLICADO;

    tLoja** novo = (tLoja**)realloc(s->lojas, (s->numLojas + 1) * sizeof(tLoja*));
    if (novo == NULL) return SISTEMA_ERRO_ALOC;

    s->lojas = novo;
    s->lojas[s->numLojas] = l;
    s->numLojas++;

    return SISTEMA_OK;
}

int getNumLojasSistema(tSistema* s){
    if (s == NULL) return 0;
    return s->numLojas;
}

tLoja* getLojaSistema(tSistema* s, int idx){
    if (s == NULL) return NULL;
    if (idx < 0 || idx >= s->numLojas) return NULL;
    return s->lojas[idx];
}

//FORNECEDORES
tFornecedor* buscaFornecedorPorCNPJ(tSistema* s, char* cnpj){
    if (s == NULL || stringVazia(cnpj)) return NULL;

    for(int i = 0; i < s->numFornecedores; i++){
        char* cnpjAtual = getCnpjFornecedor(s->fornecedores[i]);
        if (cnpjAtual != NULL && strcmp(cnpjAtual, cnpj) == 0){
            return s->fornecedores[i];
        }
    }
    return NULL;
}

tStatusSistema adicionaFornecedor(tSistema* s, tFornecedor* f){
    if (s == NULL || f == NULL) return SISTEMA_PARAM_INVALIDO;

    char* cnpj = getCnpjFornecedor(f);
    if (stringVazia(cnpj)) return SISTEMA_PARAM_INVALIDO;

    if (buscaFornecedorPorCNPJ(s, cnpj) != NULL) return SISTEMA_DUPLICADO;

    tFornecedor** novo = (tFornecedor**)realloc(s->fornecedores, (s->numFornecedores + 1) * sizeof(tFornecedor*));
    if (novo == NULL) return SISTEMA_ERRO_ALOC;

    s->fornecedores = novo;
    s->fornecedores[s->numFornecedores] = f;
    s->numFornecedores++;

    return SISTEMA_OK;
}

int getNumFornecedoresSistema(tSistema* s){
    if (s == NULL) return 0;
    return s->numFornecedores;
}

tFornecedor* getFornecedorSistema(tSistema* s, int idx){
    if (s == NULL) return NULL;
    if (idx < 0 || idx >= s->numFornecedores) return NULL;
    return s->fornecedores[idx];
}

tProduto* buscaProdutoSistemaPorCod(tSistema* s, char* cod, tLoja** lojaDona){
    if(!s || !cod || cod[0]=='\0') return NULL;

    for(int i=0;i<s->numLojas;i++){
        tLoja* l = s->lojas[i];
        tProduto* p = buscaProdutoLojaPorId(l, cod);
        if(p){
            if(lojaDona) *lojaDona = l;
            return p;
        }
    }
    return NULL;
}

tIngrediente* buscaIngredienteDisponivelPorNome(tSistema* s, char* nome, int qtdNecessaria){
    if(!s || !nome || nome[0]=='\0' || qtdNecessaria <= 0) return NULL;

    int nforn = getNumFornecedoresSistema(s);
    for(int k=0;k<nforn;k++){
        tFornecedor* f = getFornecedorSistema(s, k);
        if(!f) continue;

        int ning = getNumIngredientesFornecedor(f);
        for(int i=0;i<ning;i++){
            tIngrediente* ing = getIngredientePorIndiceFornecedor(f, i);
            if(!ing) continue;

            if(strcmp(getNomeIngrediente(ing), nome) == 0){
                if(getQuantidadeIngrediente(ing) >= qtdNecessaria){
                    return ing; // determinístico: primeiro que achar
                }
            }
        }
    }
    return NULL;
}

void consomeIngredientePorNome(tSistema* s, char* nome, int qtd){
    tIngrediente* ing = buscaIngredienteDisponivelPorNome(s, nome, qtd);
    if(!ing) return;
    adicionaQuantidadeIngrediente(ing, -qtd); // você pode permitir qtd negativa no seu ingrediente.c
}
