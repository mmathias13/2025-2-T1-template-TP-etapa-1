#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sistema.h"
#include "utils.h" // trim(), etc.

/* Por enquanto: stub só pra main compilar e o parser evoluir passo a passo.
   Depois a gente completa CAT de verdade (vai ler a linha de atributos via fgets). */
static int processaCat(tSistema *s, char *linhaCmd) {
    (void)s;
    (void)linhaCmd;
    return 0; // 0 = continua execução
}

int main(void)
{
    tSistema *s = criaSistema();
    if (s == NULL) return 0;

    char linha[512];

    while (fgets(linha, sizeof(linha), stdin)) {
        linha[strcspn(linha, "\n")] = '\0';

        char *p = trim(linha);

        /* Ignora linhas vazias (ou só espaços) */
        if (p[0] == '\0') continue;

        /* Encerra */
        if (strcmp(p, "OUT") == 0) break;

        /* Dispatch (por enquanto só CAT) */
        if (strncmp(p, "CAT", 3) == 0) {
            int sair = processaCat(s, p);
            if (sair) break;
        }

        /* Outros comandos virão aqui (CAI, CAP, ...) */
    }

    liberaSistema(s);
    return 0;
}