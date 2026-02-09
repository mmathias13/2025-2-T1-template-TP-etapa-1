// main.c  (até aqui: CAT, CAI, COI implementados; demais comandos ignorados)
// Compilar (exemplo):
// gcc -Wall -Wextra -std=c11 main.c sistema.c cliente.c loja.c fornecedor.c ingrediente.c avaliacao.c utils.c produto.c industrializado.c -o prog

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "sistema.h"
#include "cliente.h"
#include "loja.h"
#include "fornecedor.h"
#include "ingrediente.h"
#include "utils.h"   // trim()

#define LINHA_TAM 1024

/* Protótipos */
static int processaCat(tSistema *s, char *linhaCmd);
static int processaCai(tSistema *s, char *linhaCmd);
static int processaCoi(tSistema *s, char *linhaCmd);

static void removeQuebraLinha(char *s) {
    if (!s) return;
    s[strcspn(s, "\n")] = '\0';
}

/* Split por ';' aplicando trim em cada campo.
   Retorna quantos campos conseguiu colocar em campos[]. */
static int splitPorPontoEVirgula(char *linha, char **campos, int maxCampos) {
    int n = 0;
    char *tok = strtok(linha, ";");
    while (tok && n < maxCampos) {
        tok = trim(tok);
        campos[n++] = tok;
        tok = strtok(NULL, ";");
    }
    return n;
}

/* Retorna 1 se todos os primeiros n campos existem e não são vazios. */
static int camposValidos(char **campos, int n) {
    for (int i = 0; i < n; i++) {
        if (!campos[i] || campos[i][0] == '\0') return 0;
    }
    return 1;
}

/* Converte string para int com validação básica.
   Retorna 1 se ok e escreve em *out; senão 0. */
static int parseInt(char *s, int *out) {
    if (!s) return 0;
    s = trim(s);
    if (s[0] == '\0') return 0;

    char *end = NULL;
    long v = strtol(s, &end, 10);
    if (end == s) return 0;          // não converteu nada
    while (*end && isspace((unsigned char)*end)) end++;
    if (*end != '\0') return 0;      // sobrou lixo

    *out = (int)v;
    return 1;
}

/* Retorna 1 se deve encerrar (EOF), 0 se continua */
static int leLinha(char *buf, int tam) {
    if (!fgets(buf, tam, stdin)) return 1; // EOF
    removeQuebraLinha(buf);
    return 0;
}

/* ===================== CAT ===================== */
/* processa CAT. Retorna 1 se deve encerrar (OUT/EOF), 0 se continua */
static int processaCat(tSistema *s, char *linhaCmd) {
    char *tipo = trim(linhaCmd + 3); // depois de "CAT"

    int esperado = 0;
    enum { CAT_CLIENTE, CAT_LOJA, CAT_FORNECEDOR } modo;

    if (strcmp(tipo, "CLIENTE") == 0) {
        esperado = 9;
        modo = CAT_CLIENTE;
    } else if (strcmp(tipo, "LOJA") == 0) {
        esperado = 6;
        modo = CAT_LOJA;
    } else if (strcmp(tipo, "FORNECEDOR") == 0) {
        esperado = 6;
        modo = CAT_FORNECEDOR;
    } else {
        return 0; // ignora tipo desconhecido
    }

    char linhaAttr[LINHA_TAM];

    while (1) {
        if (leLinha(linhaAttr, sizeof(linhaAttr))) return 1; // EOF
        char *p = trim(linhaAttr);
        if (strcmp(p, "OUT") == 0) return 1;

        char *campos[16];
        int n = splitPorPontoEVirgula(p, campos, 16);

        if (n < esperado || !camposValidos(campos, esperado)) {
            printf("ATRIBUTO(S) FALTANTE(S), FAVOR INICIAR O CADASTRO NOVAMENTE.\n");
            continue;
        }

        if (modo == CAT_CLIENTE) {
            // ENTRADA: nome; cpf; data; telefone; endereco; username; cartao; senha; email
            // criaCliente: nome, cpf, data, telefone, endereco, email, username, senha, cartao
            tCliente *c = criaCliente(
                campos[0], campos[1], campos[2], campos[3], campos[4],
                campos[8], campos[5], campos[7], campos[6]
            );
            if (!c) continue;

            tStatusSistema st = adicionaCliente(s, c);
            if (st == SISTEMA_OK) {
                printf("CLIENTE CADASTRADO COM SUCESSO!\n");
            } else if (st == SISTEMA_DUPLICADO) {
                printf("CPF JA EXISTENTE! OPERACAO NAO PERMITIDA!\n");
                liberaCliente(c);
            } else {
                liberaCliente(c);
            }
            return 0;
        }

        if (modo == CAT_LOJA) {
            tLoja *l = criaLoja(campos[0], campos[1], campos[2], campos[3], campos[4], campos[5]);
            if (!l) continue;

            tStatusSistema st = adicionaLoja(s, l);
            if (st == SISTEMA_OK) {
                printf("LOJA CADASTRADA COM SUCESSO!\n");
            } else if (st == SISTEMA_DUPLICADO) {
                printf("CNPJ JA EXISTENTE! OPERACAO NAO PERMITIDA!\n");
                liberaLoja(l);
            } else {
                liberaLoja(l);
            }
            return 0;
        }

        // CAT_FORNECEDOR
        {
            tFornecedor *f = criaFornecedor(campos[0], campos[1], campos[2], campos[3], campos[4], campos[5]);
            if (!f) continue;

            tStatusSistema st = adicionaFornecedor(s, f);
            if (st == SISTEMA_OK) {
                printf("FORNECEDOR CADASTRADO COM SUCESSO!\n");
            } else if (st == SISTEMA_DUPLICADO) {
                printf("CNPJ JA EXISTENTE! OPERACAO NAO PERMITIDA!\n");
                liberaFornecedor(f);
            } else {
                liberaFornecedor(f);
            }
            return 0;
        }
    }
}

/* ===================== CAI ===================== */
/* processa CAI <cnpj>. Retorna 1 se deve encerrar (OUT/EOF), 0 se continua */
static int processaCai(tSistema *s, char *linhaCmd) {
    char *args = trim(linhaCmd + 3);
    if (args[0] == '\0') return 0;

    char *cnpj = args;

    tFornecedor *f = buscaFornecedorPorCNPJ(s, cnpj);
    if (!f) {
        printf("FORNECEDOR SEM CADASTRO!\n");
        return 0;
    }

    while (1) {
        char linhaIng[LINHA_TAM];
        if (leLinha(linhaIng, sizeof(linhaIng))) return 1;

        char *p = trim(linhaIng);
        if (strcmp(p, "OUT") == 0) return 1;

        char *campos[8];
        int n = splitPorPontoEVirgula(p, campos, 8);

        if (n < 4 || !camposValidos(campos, 4)) {
            printf("ATRIBUTO(S) FALTANTE(S), FAVOR INICIAR O CADASTRO NOVAMENTE.\n");
            continue;
        }

        int codigo = 0, qtd = 0;
        if (!parseInt(campos[1], &codigo) || !parseInt(campos[3], &qtd)) {
            printf("ATRIBUTO(S) FALTANTE(S), FAVOR INICIAR O CADASTRO NOVAMENTE.\n");
            continue;
        }

        char *t = trim(campos[2]);
        if (t[0] == '\0') {
            printf("ATRIBUTO(S) FALTANTE(S), FAVOR INICIAR O CADASTRO NOVAMENTE.\n");
            continue;
        }
        char tipo = t[0];

        int idx = buscaIngredienteFornecedor(f, codigo);
        if (idx >= 0) {
            printf("INGREDIENTE JA CADASTRADO, ATUALIZANDO QUANTIDADE DISPONIVEL!\n");
        }

        tIngrediente *ing = criaIngrediente(campos[0], codigo, tipo, qtd);
        if (!ing) continue;

        cadastraOuAtualizaIngredienteFornecedor(f, ing);

        if (idx < 0) {
            printf("INGREDIENTE CADASTRADO COM SUCESSO!\n");
        }
        return 0;
    }
}

/* ===================== COI ===================== */
/* COI NOME_FORNECEDOR <nome>  | COI ID_FORNECEDOR <cnpj> | COI NOME_INGREDIENTE <nome> */
static int processaCoi(tSistema *s, char *linhaCmd) {
    char *args = trim(linhaCmd + 3);
    if (args[0] == '\0') return 0;

    // pega o tipo (até o próximo espaço)
    char *tipo = strtok(args, " ");
    char *chave = strtok(NULL, ""); // resto da linha (pode ter espaços)
    if (!tipo) return 0;
    if (chave) chave = trim(chave);
    else chave = (char*)"";

    if (strcmp(tipo, "ID_FORNECEDOR") == 0) {
        tFornecedor *f = buscaFornecedorPorCNPJ(s, chave);
        if (!f) {
            printf("FORNECEDOR SEM CADASTRO!\n");
            return 0;
        }

        printf("FORNECEDOR: %s (%s)\n", getNomeFornecedor(f), getCnpjFornecedor(f));

        int num = getNumIngredientesFornecedor(f);
        for (int i = 0; i < num; i++) {
            tIngrediente *ing = getIngredientePorIndiceFornecedor(f, i);
            if (!ing) continue;

            printf("%d - %s, %d, %c, QUANTIDADE DISPONIVEL: %d UNIDADE(S).\n",
                   i + 1,
                   getNomeIngrediente(ing),
                   getCodigoBarrasIngrediente(ing),
                   getTipoIngrediente(ing),
                   getQuantidadeIngrediente(ing));
        }
        return 0;
    }

    if (strcmp(tipo, "NOME_FORNECEDOR") == 0) {
        // busca por substring (PERIM casa com "SUPERMERCADO PERIM")
        int achou = 0;

        int nforn = getNumFornecedoresSistema(s);
        for (int k = 0; k < nforn; k++) {
            tFornecedor *f = getFornecedorSistema(s, k);
            if (!f) continue;

            if (strstr(getNomeFornecedor(f), chave) == NULL) continue;

            achou = 1;
            printf("FORNECEDOR: %s (%s)\n", getNomeFornecedor(f), getCnpjFornecedor(f));

            int num = getNumIngredientesFornecedor(f);
            for (int i = 0; i < num; i++) {
                tIngrediente *ing = getIngredientePorIndiceFornecedor(f, i);
                if (!ing) continue;

                printf("%d - %s, %d, %c, QUANTIDADE DISPONIVEL: %d UNIDADE(S).\n",
                       i + 1,
                       getNomeIngrediente(ing),
                       getCodigoBarrasIngrediente(ing),
                       getTipoIngrediente(ing),
                       getQuantidadeIngrediente(ing));
            }
        }

        if (!achou) {
            printf("FORNECEDOR SEM CADASTRO!\n");
        }
        return 0;
    }

    if (strcmp(tipo, "NOME_INGREDIENTE") == 0) {
    int nforn = getNumFornecedoresSistema(s);
    int kMatch = 1; // numeração GLOBAL dos ingredientes encontrados

    for (int k = 0; k < nforn; k++) {
        tFornecedor *f = getFornecedorSistema(s, k);
        if (!f) continue;

        int num = getNumIngredientesFornecedor(f);

        // verifica se este fornecedor tem algum ingrediente que casa
        int tem = 0;
        for (int i = 0; i < num; i++) {
            tIngrediente *ing = getIngredientePorIndiceFornecedor(f, i);
            if (!ing) continue;

            if (strstr(getNomeIngrediente(ing), chave) != NULL) {
                tem = 1;
                break;
            }
        }
        if (!tem) continue;

        // imprime cabeçalho e só os ingredientes que casam
        printf("FORNECEDOR: %s (%s)\n", getNomeFornecedor(f), getCnpjFornecedor(f));

        for (int i = 0; i < num; i++) {
            tIngrediente *ing = getIngredientePorIndiceFornecedor(f, i);
            if (!ing) continue;

            if (strstr(getNomeIngrediente(ing), chave) == NULL) continue;

            printf("%d - %s, %d, %c, QUANTIDADE DISPONIVEL: %d UNIDADE(S).\n",
                   kMatch,
                   getNomeIngrediente(ing),
                   getCodigoBarrasIngrediente(ing),
                   getTipoIngrediente(ing),
                   getQuantidadeIngrediente(ing));
            kMatch++;
        }
    }

    return 0;
}

    // tipo desconhecido -> ignora
    return 0;
}

/* ===================== MAIN ===================== */
int main(void) {
    tSistema *s = criaSistema();
    if (!s) return 0;

    char linha[LINHA_TAM];

    while (fgets(linha, sizeof(linha), stdin)) {
        removeQuebraLinha(linha);
        char *p = trim(linha);

        if (p[0] == '\0') continue;
        if (strcmp(p, "OUT") == 0) break;

        if (strncmp(p, "CAT", 3) == 0) {
            if (processaCat(s, p)) break;
            continue;
        }

        if (strncmp(p, "CAI", 3) == 0) {
            if (processaCai(s, p)) break;
            continue;
        }

        if (strncmp(p, "COI", 3) == 0) {
            if (processaCoi(s, p)) break;
            continue;
        }

        // Demais comandos ainda não implementados nesta etapa:
        // CAP, COP, COL, APS, RPS, ECS, LSC, BAP
    }

    liberaSistema(s);
    return 0;
}