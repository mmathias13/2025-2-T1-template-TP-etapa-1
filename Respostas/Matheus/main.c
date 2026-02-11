// main.c
// gcc -Wall -Wextra -std=c11 main.c sistema.c cliente.c loja.c fornecedor.c ingrediente.c avaliacao.c utils.c produto.c industrializado.c prato.c sacola.c -o prog

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "sistema.h"
#include "cliente.h"
#include "loja.h"
#include "fornecedor.h"
#include "ingrediente.h"
#include "utils.h"
#include "sacola.h"
#include "produto.h"
#include "industrializado.h"
#include "prato.h"
#include "avaliacao.h"

#define LINHA_TAM 1024

/* Protótipos */
static int processaCat(tSistema *s, char *linhaCmd);
static int processaCai(tSistema *s, char *linhaCmd);
static int processaCoi(tSistema *s, char *linhaCmd);
static int processaCap(tSistema *s, char *linhaCmd);
static int processaCop(tSistema *s, char *linhaCmd);
static int processaCol(tSistema *s, char *linhaCmd);
static int processaBap(tSistema *s, char *linhaCmd);
static int processaAps(tSistema *s, char *linhaCmd);
static int processaRps(tSistema *s, char *linhaCmd);
static int processaLsc(tSistema *s, char *linhaCmd);
static int processaEcs(tSistema *s, char *linhaCmd);

static void removeQuebraLinha(char *s) {
    if (!s) return;
    s[strcspn(s, "\n")] = '\0';
}

/* Split por ';' aplicando trim em cada campo. */
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

static int camposValidos(char **campos, int n) {
    for (int i = 0; i < n; i++) {
        if (!campos[i] || campos[i][0] == '\0') return 0;
    }
    return 1;
}

static int parseInt(char *s, int *out) {
    if (!s) return 0;
    s = trim(s);
    if (s[0] == '\0') return 0;

    char *end = NULL;
    long v = strtol(s, &end, 10);
    if (end == s) return 0;
    while (*end && isspace((unsigned char)*end)) end++;
    if (*end != '\0') return 0;

    *out = (int)v;
    return 1;
}

static int parseFloat(char *s, float *out) {
    if (!s) return 0;
    s = trim(s);
    if (s[0] == '\0') return 0;

    char *end = NULL;
    double v = strtod(s, &end);
    if (end == s) return 0;
    while (*end && isspace((unsigned char)*end)) end++;
    if (*end != '\0') return 0;

    *out = (float)v;
    return 1;
}

static int leLinha(char *buf, int tam) {
    if (!fgets(buf, tam, stdin)) return 1; // EOF
    removeQuebraLinha(buf);
    return 0;
}

/* ===================== CAT ===================== */
static int processaCat(tSistema *s, char *linhaCmd) {
    char *tipo = trim(linhaCmd + 3);

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
        return 0;
    }

    char linhaAttr[LINHA_TAM];

    while (1) {
        if (leLinha(linhaAttr, sizeof(linhaAttr))) return 1;
        char *p = trim(linhaAttr);
        if (strcmp(p, "OUT") == 0) return 1;

        char *campos[16];
        int n = splitPorPontoEVirgula(p, campos, 16);

        if (n < esperado || !camposValidos(campos, esperado)) {
            printf("ATRIBUTO(S) FALTANTE(S), FAVOR INICIAR O CADASTRO NOVAMENTE.\n");
            continue;
        }

        if (modo == CAT_CLIENTE) {
            tCliente *c = criaCliente(
                campos[0], campos[1], campos[2], campos[3], campos[4],
                campos[5], // email
                campos[6], // username
                campos[7], // senha
                campos[8]  // cartao
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
static int processaCoi(tSistema *s, char *linhaCmd) {
    char *args = trim(linhaCmd + 3);
    if (args[0] == '\0') return 0;

    char *tipo = strtok(args, " ");
    char *chave = strtok(NULL, "");
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
        int kMatch = 1;

        for (int k = 0; k < nforn; k++) {
            tFornecedor *f = getFornecedorSistema(s, k);
            if (!f) continue;

            int num = getNumIngredientesFornecedor(f);

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

    return 0;
}

/* ===================== CAP ===================== */
/* Entrada:
   CAP <CNPJ_LOJA> P
   COD; NOME; PRECO   OU  COD; NOME; DESC; PRECO
   N; ING1; ING2; ... */
static int processaCap(tSistema *s, char *linhaCmd){
    char buf[LINHA_TAM];
    snprintf(buf, sizeof(buf), "%s", linhaCmd);

    strtok(buf, " ");                 // "CAP"
    char *cnpj = strtok(NULL, " ");
    char *tipoStr = strtok(NULL, " ");

    if(!cnpj || !tipoStr){
        printf("ATRIBUTO(S) FALTANTE(S), FAVOR INICIAR O CADASTRO NOVAMENTE.\n");
        return 0;
    }

    char tipo = trim(tipoStr)[0];

    tLoja* l = buscaLojaPorCNPJ(s, cnpj);
    if(!l){
        printf("LOJA SEM CADASTRO!\n");
        return 0;
    }

    if(tipo == 'P'){
        /* Para CAP P, o script de correção envia novas tentativas sem repetir o "CAP".
           Então precisamos ficar aguardando até conseguir cadastrar ou até vir OUT/EOF. */
        while (1) {
            char linha1[LINHA_TAM], linha2[LINHA_TAM];
            if (leLinha(linha1, sizeof(linha1))) return 1;
            char *p1 = trim(linha1);
            if (strcmp(p1, "OUT") == 0) return 1;

            // linha1: COD; NOME; PRECO  OU  COD; NOME; DESC; PRECO
            char *c1[8];
            int n1 = splitPorPontoEVirgula(p1, c1, 8);

            char *cod = NULL, *nome = NULL, *desc = NULL;
            float preco = 0.0f;

            int ok1 = 0;
            if (n1 == 3 && camposValidos(c1, 3) && parseFloat(c1[2], &preco)) {
                cod = c1[0];
                nome = c1[1];
                desc = (char*)"";
                ok1 = 1;
            } else if (n1 >= 4 && camposValidos(c1, 4) && parseFloat(c1[3], &preco)) {
                cod = c1[0];
                nome = c1[1];
                desc = c1[2];
                ok1 = 1;
            }

            if (!ok1) {
                printf("ATRIBUTO(S) FALTANTE(S), FAVOR INICIAR O CADASTRO NOVAMENTE.\n");
                continue;
            }

            if (leLinha(linha2, sizeof(linha2))) return 1;
            char *p2 = trim(linha2);
            if (strcmp(p2, "OUT") == 0) return 1;

            // linha2: N; ING1; ...
            char *c2[128];
            int n2 = splitPorPontoEVirgula(p2, c2, 128);
            int N = 0;
            if (n2 < 2 || !camposValidos(c2, 2) || !parseInt(c2[0], &N) || N <= 0 || n2 < N + 1) {
                printf("ATRIBUTO(S) FALTANTE(S), FAVOR INICIAR O CADASTRO NOVAMENTE.\n");
                continue;
            }

            if (buscaProdutoLojaPorId(l, cod) != NULL) {
                printf("PRODUTO JA CADASTRADO! OPERACAO NAO PERMITIDA!\n");
                return 0;
            }

            // valida disponibilidade de cada ingrediente (>=1)
            for (int i = 0; i < N; i++) {
                char *nomeIng = trim(c2[i + 1]);
                if (!buscaIngredienteDisponivelPorNome(s, nomeIng, 1)) {
                    printf("INGREDIENTE %s INDISPONIVEL! CADASTRO DE PRODUTO CANCELADO!\n", nomeIng);
                    return 0;
                }
            }

            tPrato* pr = criaPrato(s, cod, nome, desc, preco, N, &c2[1]);
            if (!pr) return 0;

            tProduto* prod = criaProduto(
                pr,
                imprimeFisicoPrato,
                imprimeDigitalPrato,
                liberaPrato,
                getValorPrato,
                getCodPrato,
                getNomePrato,
                getTipoPrato,
                getDescPrato,
                getDisponibilidadePrato,
                alteraDisponibilidadePrato,
                printaPrato
            );

            if (!prod) {
                liberaPrato(pr);
                return 0;
            }

            if (!adicionaProdutoLoja(l, prod)) {
                printf("PRODUTO JA CADASTRADO! OPERACAO NAO PERMITIDA!\n");
                liberaProduto(prod);
                return 0;
            }

            printf("PRODUTO CADASTRADO COM SUCESSO!\n");
            return 0;
        }
    }

    if(tipo == 'I'){
        /* Para CAP I, o script de correção também tenta novamente sem repetir o "CAP" */
        while (1) {
            tIndustrializado* ind = criaIndustrializado();
            if (!ind) {
                printf("ATRIBUTO(S) FALTANTE(S), FAVOR INICIAR O CADASTRO NOVAMENTE.\n");
                continue;
            }

        char* cod = getCodIndustrializado(ind);
        if(!cod || cod[0]=='\0'){
            liberaIndustrializado(ind);
            return 0;
        }

        if(buscaProdutoLojaPorId(l, cod) != NULL){
            printf("PRODUTO JA CADASTRADO! OPERACAO NAO PERMITIDA!\n");
            liberaIndustrializado(ind);
            return 0;
        }

        tProduto* prod = criaProduto(
            ind,
            imprimeFisicoIndustrializado,
            imprimeDigitalIndustrializado,
            liberaIndustrializado,
            getValorIndustrializado,
            getCodIndustrializado,
            getNomeIndustrializado,
            getTipoIndustrializado,
            getDescIndustrializado,
            getDisponibilidadeIndustrializado,
            alteraDisponibilidadeIndustrializado,
            printaIndustrializado
        );

        if(!prod){
            liberaIndustrializado(ind);
            return 0;
        }

        if(!adicionaProdutoLoja(l, prod)){
            printf("PRODUTO JA CADASTRADO! OPERACAO NAO PERMITIDA!\n");
            liberaProduto(prod);
            return 0;
        }

            printf("PRODUTO CADASTRADO COM SUCESSO!\n");
            return 0;
        }
    }

    printf("TIPO DE PRODUTO INVALIDO!\n");
    return 0;
}

/* ===================== COP ===================== */
/* Exemplo:
   COP NOME PEPERONI
   COP TIPO P
   COP TUDO
*/
static int processaCop(tSistema *s, char *linhaCmd){
    char buf[LINHA_TAM];
    snprintf(buf, sizeof(buf), "%s", linhaCmd);

    strtok(buf, " "); // "COP"
    char *modo = strtok(NULL, " ");
    char *chave = strtok(NULL, ""); // resto (pode ter espaços)

    if(!modo) return 0;

    if(chave) chave = trim(chave);
    else chave = (char*)"";

    int idxGlobal = 1;

    int nlojas = getNumLojasSistema(s);
    for(int i=0;i<nlojas;i++){
        tLoja* l = getLojaSistema(s, i);
        if(!l) continue;

        int nprod = getNumProdutosLoja(l);
        for(int j=0;j<nprod;j++){
            tProduto* p = getProdutoLoja(l, j);
            if(!p) continue;

            int ok = 0;

            if(strcmp(modo, "TUDO") == 0){
                ok = 1;
            }
            else if(strcmp(modo, "NOME") == 0){
                char* nome = getNomeProduto(p);
                if(!nome) nome = (char*)"";
                ok = (strstr(nome, chave) != NULL);
            }
            else if(strcmp(modo, "TIPO") == 0){
                char t = (chave && chave[0]) ? chave[0] : '\0';
                ok = (getTipoProduto(p) == t);
            }
            else {
                return 0; // modo desconhecido
            }

            if(!ok) continue;

            // Formato do exemplo: "<idx> -  <LOJA>, <produto...> AVALIACAO MEDIA: ..."
            printf("%d - %s, ", idxGlobal, getNomeLoja(l));
            printaProduto(p);            // produto imprime o miolo (prato/ind) + avaliações (se você colocou isso lá)
            idxGlobal++;
        }
    }

    return 0;
}

/* ===================== COL ===================== */
static int processaCol(tSistema *s, char *linhaCmd) {
    char buf[LINHA_TAM];
    snprintf(buf, sizeof(buf), "%s", linhaCmd);

    strtok(buf, " ");               // "COL"
    char *modo = strtok(NULL, " ");
    char *chave = strtok(NULL, ""); // resto
    if (!modo) return 0;
    if (chave) chave = trim(chave);
    else chave = (char*)"";

    if (strcmp(modo, "TUDO") == 0) {
        int nlojas = getNumLojasSistema(s);
        for (int i = 0; i < nlojas; i++) {
            tLoja *l = getLojaSistema(s, i);
            if (!l) continue;

            printf("%d - %s, %s, %s, %s\n",
                   i + 1,
                   getNomeLoja(l),
                   getCnpjLoja(l),
                   getTelefoneLoja(l),
                   getEnderecoLoja(l));
        }
        return 0;
    }

    if (strcmp(modo, "NOME_LOJA") == 0) {
        int nlojas = getNumLojasSistema(s);
        int achou = 0;

        for (int i = 0; i < nlojas; i++) {
            tLoja *l = getLojaSistema(s, i);
            if (!l) continue;
            if (strstr(getNomeLoja(l), chave) == NULL) continue;

            achou = 1;

            printf("LOJA: %s (%s): \n", getNomeLoja(l), getCnpjLoja(l));

            int nprod = getNumProdutosLoja(l);
            for (int j = 0; j < nprod; j++) {
                tProduto *p = getProdutoLoja(l, j);
                if (!p) continue;
                printf("%d - ", j + 1);
                printaProduto(p);
            }
        }

        (void)achou;
        return 0;
    }

    if (strcmp(modo, "NOME_PRODUTO") == 0) {
        int nlojas = getNumLojasSistema(s);
        int idx = 1;

        for (int i = 0; i < nlojas; i++) {
            tLoja *l = getLojaSistema(s, i);
            if (!l) continue;

            int nprod = getNumProdutosLoja(l);
            int oferta = 0;
            for (int j = 0; j < nprod; j++) {
                tProduto *p = getProdutoLoja(l, j);
                if (!p) continue;
                char *nomeP = getNomeProduto(p);
                if (!nomeP) nomeP = (char*)"";
                if (strstr(nomeP, chave) != NULL) { oferta = 1; break; }
            }

            if (!oferta) continue;

            printf("%d - %s, %s, %s, %s\n",
                   idx,
                   getNomeLoja(l),
                   getCnpjLoja(l),
                   getTelefoneLoja(l),
                   getEnderecoLoja(l));
            idx++;
        }
        return 0;
    }

    return 0;
}

/* ================== BAP ======================*/
static int processaBap(tSistema *s, char *linhaCmd) {
    char *cod = trim(linhaCmd + 3); // depois de "BAP"
    if (!cod || cod[0] == '\0') return 0;

    tLoja *lojaDona = NULL;
    tProduto *p = buscaProdutoSistemaPorCod(s, cod, &lojaDona);

    if (!p) {
        printf("PRODUTO NAO EXISTENTE! TENTE NOVAMENTE!\n");
        return 0;
    }

    printaAvaliacoesProduto(p);
    return 0;
}

/* ================================= APS ===========================*/
static int processaAps(tSistema *s, char *linhaCmd){
    // APS <CPF> <COD> <QTD>
    char buf[LINHA_TAM];
    snprintf(buf, sizeof(buf), "%s", linhaCmd);

    strtok(buf, " ");                 // "APS"
    char *cpf = strtok(NULL, " ");
    char *cod = strtok(NULL, " ");
    char *qtdStr = strtok(NULL, " ");

    if(!cpf || !cod || !qtdStr){
        return 0;
    }

    int qtd = 0;
    if(!parseInt(qtdStr, &qtd) || qtd <= 0){
        return 0;
    }

    tCliente* c = buscaClientePorCPF(s, cpf);
    if(!c){
        printf("CLIENTE SEM CADASTRO!\n");
        return 0;
    }

    tLoja* lojaDona = NULL;
    tProduto* p = buscaProdutoSistemaPorCod(s, cod, &lojaDona);
    if(!p){
        printf("O PRODUTO NAO ESTA CADASTRADO EM NENHUMA LOJA!\n");
        sacolaImprime(getSacolaCliente(c));
        return 0;
    }

    sacolaAdicionaProduto(getSacolaCliente(c), p, qtd);
    sacolaImprime(getSacolaCliente(c));
    return 0;
}

/* ============== RPS ================*/
static int processaRps(tSistema *s, char *linhaCmd){
    // RPS <CPF> <COD> <QTD>
    char buf[LINHA_TAM];
    snprintf(buf, sizeof(buf), "%s", linhaCmd);

    strtok(buf, " ");                 // "RPS"
    char *cpf = strtok(NULL, " ");
    char *cod = strtok(NULL, " ");
    char *qtdStr = strtok(NULL, " ");

    if(!cpf || !cod || !qtdStr){
        return 0;
    }

    int qtd = 0;
    if(!parseInt(qtdStr, &qtd) || qtd <= 0){
        return 0;
    }

    tCliente* c = buscaClientePorCPF(s, cpf);
    if(!c){
        printf("CLIENTE SEM CADASTRO!\n");
        return 0;
    }

    tLoja* lojaDona = NULL;
    tProduto* p = buscaProdutoSistemaPorCod(s, cod, &lojaDona);
    if(!p){
        printf("O PRODUTO NAO ESTA CADASTRADO EM NENHUMA LOJA!\n");
        return 0;
    }

    tStatusSacola st = sacolaRemoveProdutoPorCod(getSacolaCliente(c), cod, qtd);
    if(st == SACOLA_NAO_ENCONTRADO){
        printf("O PRODUTO NAO ESTA PRESENTE NA SACOLA DE COMPRAS!\n");
        sacolaImprime(getSacolaCliente(c));
        return 0;
    }

    printf("O PRODUTO FOI REMOVIDO DA SACOLA DE COMPRAS!\n");
    sacolaImprime(getSacolaCliente(c));
    return 0;
}

/* ==================== LSC ===============================*/
static int processaLsc(tSistema *s, char *linhaCmd){
    // LSC <CPF>
    char *cpf = trim(linhaCmd + 3);
    if(!cpf || cpf[0] == '\0') return 0;

    tCliente* c = buscaClientePorCPF(s, cpf);
    if(!c){
        printf("CLIENTE SEM CADASTRO!\n");
        return 0;
    }

    esvaziaSacolaCliente(c);
    printf("SACOLA DE COMPRAS ESVAZIADA!\n");
    return 0;
}

/* ====================== ECS =====================*/
static int processaEcs(tSistema *s, char *linhaCmd){
    // ECS <CPF>
    char *cpf = trim(linhaCmd + 3);
    if(!cpf || cpf[0]=='\0') return 0;

    tCliente* c = buscaClientePorCPF(s, cpf);
    if(!c){
        printf("CLIENTE SEM CADASTRO!\n");
        return 0;
    }

    tSacola* sac = getSacolaCliente(c);
    if(!sac || sacolaGetNumItens(sac) == 0){
        printf("SACOLA DE COMPRAS VAZIA!\n");
        return 0;
    }

    int n = sacolaGetNumItens(sac);

    tProduto** compradosP = (tProduto**)malloc(n * sizeof(tProduto*));
    int* compradosQ = (int*)malloc(n * sizeof(int));
    if(!compradosP || !compradosQ){
        free(compradosP);
        free(compradosQ);
        return 0;
    }

    int nComprados = 0;
    int teveIndisp = 0;

    /* Importante: a disponibilidade precisa considerar o consumo cumulativo.
       Então, quando um item está disponível, já consumimos os insumos (ou estoque)
       imediatamente, para que os próximos itens sejam avaliados com o saldo restante.
       Na impressão da NF não consumimos novamente. */
    for(int i=0;i<n;i++){
        tProduto* p = sacolaGetProduto(sac, i);
        int qtd = sacolaGetQtd(sac, i);

        if(getDisponibilidadeProduto(p, qtd)){
            /* reserva/consome agora para afetar os próximos itens */
            alteraDisponibilidadeProduto(p, qtd);

            compradosP[nComprados] = p;
            compradosQ[nComprados] = qtd;
            nComprados++;
        } else {
            teveIndisp = 1;
            printf("PRODUTO %d ESTA INDISPONIVEL NO MOMENTO!\n", i + 1);
        }
    }

    if(nComprados == 0){
        printf("COMPRA CANCELADA POR INDISPONIBILIDADE DE PRODUTOS!\n");
        free(compradosP);
        free(compradosQ);
        return 0;
    }

    if(teveIndisp) printf("COMPRA EFETUADA COM SUCESSO (PARCIALMENTE)!\n");
    else printf("COMPRA EFETUADA COM SUCESSO!\n");

    printf("------------------------------------------\n");
    printf("NOTA FISCAL DE COMPRA:\n");
    printf("------------------------------------------\n");

    float totalNF = 0.0f;

    for(int i=0;i<nComprados;i++){
        tProduto* p = compradosP[i];
        int qtd = compradosQ[i];

        totalNF += getValorProduto(p) * (float)qtd;

        imprimeFisico(p, qtd);
        imprimeDigital(p, getEmailCliente(c), qtd);

        if(i < nComprados - 1){
            printf("------------------------------------------\n");
        }
    }

    printf("------------------------------------------\n");
    printf("VALOR TOTAL DA NOTA FISCAL: %.2f\n", totalNF);
    printf("------------------------------------------\n");

    for(int k=0;k<nComprados;k++){
        const char* cod = getCodProduto(compradosP[k]);
        sacolaRemovePorCod(sac, cod, compradosQ[k]);
    }

    printf("DESEJA REALIZAR A AVALIAÇÃO DO(S) PRODUTO(S) COMPRADO(S)? (SIM/NAO/OUT)\n");

    char resp[256];
    if(!fgets(resp, sizeof(resp), stdin)){
        free(compradosP);
        free(compradosQ);
        return 1;
    }
    resp[strcspn(resp, "\n")] = '\0';
    char* r = trim(resp);

    if(strcmp(r, "OUT") == 0){
        free(compradosP);
        free(compradosQ);
        return 1;
    }

    if(strcmp(r, "SIM") == 0){
        for(int i=0;i<nComprados;i++){
            tProduto* p = compradosP[i];

            printf("PRODUTO %s, %s:\n", getCodProduto(p), getNomeProduto(p));

            tAvaliacao* a = criaAvaliacao(getCpfCliente(c));
            if(!a){
                free(compradosP);
                free(compradosQ);
                return 1;
            }

            insereAvaliacaoProduto(p, a);
            printf("AVALIACAO REGISTRADA COM SUCESSO!\n");
        }
    }

    free(compradosP);
    free(compradosQ);
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

        if (strncmp(p, "CAT", 3) == 0) { if (processaCat(s, p)) break; continue; }
        if (strncmp(p, "CAI", 3) == 0) { if (processaCai(s, p)) break; continue; }
        if (strncmp(p, "COI", 3) == 0) { if (processaCoi(s, p)) break; continue; }
        if (strncmp(p, "CAP", 3) == 0) { if (processaCap(s, p)) break; continue; }
        if (strncmp(p, "COP", 3) == 0) { if (processaCop(s, p)) break; continue; }
        if (strncmp(p, "COL", 3) == 0) { if (processaCol(s, p)) break; continue; }
        if (strncmp(p, "BAP", 3) == 0) { if (processaBap(s, p)) break; continue; }
        if (strncmp(p, "APS", 3) == 0) { if (processaAps(s, p)) break; continue; }
        if (strncmp(p, "RPS", 3) == 0) { if (processaRps(s, p)) break; continue; }
        if (strncmp(p, "LSC", 3) == 0) { if (processaLsc(s, p)) break; continue; }
        if (strncmp(p, "ECS", 3) == 0) { if (processaEcs(s, p)) break; continue; }

        // comandos desconhecidos: ignora
    }

    liberaSistema(s);
    return 0;
}