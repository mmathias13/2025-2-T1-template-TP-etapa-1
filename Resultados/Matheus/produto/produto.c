#include "produto.h"
#include <stdio.h>
#include <stdlib.h>

struct Produto {
    void *dado;

    func_ptr_imprimeFisico imprimeFisicoCb;
    func_ptr_imprimeDigital imprimeDigitalCb;
    func_ptr_libera liberaCb;
    func_ptr_valor valorCb;
    func_ptr_codProduto codProdutoCb;
    func_ptr_nomeProduto nomeProdutoCb;
    func_ptr_tipoProduto tipoProdutoCb;
    func_ptr_descProduto descProdutoCb;
    func_ptr_disponibilidade disponibilidadeCb;
    func_ptr_atualizaDisponibilidade atualizaDisponibilidadeCb;
    func_ptr_printaProduto printaProdutoCb;

    tAvaliacao **avaliacoes;
    int numAvaliacoes;
};

tProduto *criaProduto(void *dado,
                      func_ptr_imprimeFisico imprimeFisico,
                      func_ptr_imprimeDigital ImprimeDigital,
                      func_ptr_libera libera,
                      func_ptr_valor valor,
                      func_ptr_codProduto codProduto,
                      func_ptr_nomeProduto nomeProduto,
                      func_ptr_tipoProduto tipoProduto,
                      func_ptr_descProduto descProduto,
                      func_ptr_disponibilidade disponibilidade,
                      func_ptr_atualizaDisponibilidade atualizaDisponibilidade,
                      func_ptr_printaProduto printaProduto)
{
    tProduto *p = (tProduto*) malloc(sizeof(tProduto));
    if (!p) {
        printf("Erro de alocação em PRODUTO!\n");
        return NULL;
    }

    p->dado = dado;

    p->imprimeFisicoCb = imprimeFisico;
    p->imprimeDigitalCb = ImprimeDigital;
    p->liberaCb = libera;
    p->valorCb = valor;
    p->codProdutoCb = codProduto;
    p->nomeProdutoCb = nomeProduto;
    p->tipoProdutoCb = tipoProduto;
    p->descProdutoCb = descProduto;
    p->disponibilidadeCb = disponibilidade;
    p->atualizaDisponibilidadeCb = atualizaDisponibilidade;
    p->printaProdutoCb = printaProduto;

    p->avaliacoes = NULL;
    p->numAvaliacoes = 0;

    return p;
}

void liberaProduto(tProduto *prod)
{
    if (!prod) return;

    if (prod->liberaCb && prod->dado) {
        prod->liberaCb(prod->dado);
    }

    if (prod->avaliacoes) {
        for (int i = 0; i < prod->numAvaliacoes; i++) {
            liberaAvaliacao(prod->avaliacoes[i]);
        }
        free(prod->avaliacoes);
    }

    free(prod);
}

void insereAvaliacaoProduto(tProduto *prod, tAvaliacao *avaliacao)
{
    if (!prod || !avaliacao) return;

    tAvaliacao **novo = (tAvaliacao**) realloc(prod->avaliacoes,
                           (prod->numAvaliacoes + 1) * sizeof(tAvaliacao*));
    if (!novo) {
        // Como a especificação não define mensagem aqui, só evita leak:
        liberaAvaliacao(avaliacao);
        return;
    }

    prod->avaliacoes = novo;
    prod->avaliacoes[prod->numAvaliacoes] = avaliacao;
    prod->numAvaliacoes++;
}

void imprimeFisico(tProduto *prod, int qtd)
{
    if (!prod || !prod->imprimeFisicoCb) return;
    prod->imprimeFisicoCb(prod->dado, qtd);
}

void imprimeDigital(tProduto *prod, char *email, int qtd)
{
    if (!prod || !prod->imprimeDigitalCb) return;
    prod->imprimeDigitalCb(prod->dado, email, qtd);
}

void *getItemProduto(tProduto *prod)
{
    if (!prod) return NULL;
    return prod->dado;
}

float getValorProduto(tProduto *prod)
{
    if (!prod || !prod->valorCb) return 0.0f;
    return prod->valorCb(prod->dado);
}

char *getCodProduto(tProduto *prod)
{
    if (!prod || !prod->codProdutoCb) return NULL;
    return prod->codProdutoCb(prod->dado);
}

char *getNomeProduto(tProduto *prod)
{
    if (!prod || !prod->nomeProdutoCb) return NULL;
    return prod->nomeProdutoCb(prod->dado);
}

char *getDescProduto(tProduto *prod)
{
    if (!prod || !prod->descProdutoCb) return NULL;
    return prod->descProdutoCb(prod->dado);
}

char getTipoProduto(tProduto *prod)
{
    if (!prod || !prod->tipoProdutoCb) return '\0';
    return prod->tipoProdutoCb(prod->dado);
}

int getDisponibilidadeProduto(tProduto *prod, int qtd)
{
    if (!prod || !prod->disponibilidadeCb) return 0;
    return prod->disponibilidadeCb(prod->dado, qtd);
}

void alteraDisponibilidadeProduto(tProduto *prod, int qtd)
{
    if (!prod || !prod->atualizaDisponibilidadeCb) return;
    prod->atualizaDisponibilidadeCb(prod->dado, qtd);
}

void printaAvaliacoesProduto(tProduto *produto)
{
    if (!produto || produto->numAvaliacoes <= 0 || !produto->avaliacoes) {
        printf("AVALIACOES INEXISTENTES PARA O PRODUTO CONSULTADO!\n");
        return;
    }

    float soma = 0.0f;
    for (int i = 0; i < produto->numAvaliacoes; i++) {
        tAvaliacao *a = produto->avaliacoes[i];
        int nota = getNotaAvaliacao(a);
        char *coment = getComentarioAvaliacao(a);

        printf("AVALIACAO #%d: %s\n", i + 1, coment ? coment : "");
        soma += (float)nota;
    }

    printf("AVALIACAO MEDIA: %.2f.\n", soma / (float)produto->numAvaliacoes);
}

void printaProduto(tProduto *prod)
{
    if (!prod || !prod->printaProdutoCb) return;
    prod->printaProdutoCb(prod->dado);
}