#include "helper.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

unsigned char escape(char *c)
{
    if (c[0] == 'x')
    {
        return strtol(c + 1, NULL, 16);
    }
    else if (isdigit(c[0]))
    {
        return strtol(c, NULL, 8);
    }

    switch (c[0])
    {
    case 'n':
        return '\n';
    case 't':
        return '\t';
    case '\\':
        return '\\';
    case '"':
        return '"';
    case '\'':
        return '\'';
    case 'b':
        return '\b';
    case 'r':
        return '\r';
    case 'f':
        return '\f';
    default:
        return c[0];
    }
}

char peek(char *src, uint32_t i, size_t len)
{
    if (i + 1 >= len)
        return '\0';
    return src[i + 1];
}
