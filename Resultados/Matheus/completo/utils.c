#include "utils.h"
#include <ctype.h>
#include <string.h>

// Funções utilitárias pequenas (ex.: trim de strings) usadas por vários módulos.

/*
 * Remove espaços em branco do começo e do fim de uma string.
 */
char* trim(char *s) {
    if (!s) return s;

    while (*s && isspace((unsigned char)*s)) s++;

    char *end = s + strlen(s);
    while (end > s && isspace((unsigned char)end[-1])) end--;
    *end = '\0';

    return s;
}