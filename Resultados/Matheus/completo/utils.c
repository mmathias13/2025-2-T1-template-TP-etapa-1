#include "utils.h"
#include <ctype.h>
#include <string.h>

char* trim(char *s) {
    if (!s) return s;

    while (*s && isspace((unsigned char)*s)) s++;

    char *end = s + strlen(s);
    while (end > s && isspace((unsigned char)end[-1])) end--;
    *end = '\0';

    return s;
}