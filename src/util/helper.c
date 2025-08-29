#include "helper.h"

char
escape (char c)
{
    switch (c)
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
            return c;
        }
}

char
peek (char *src, uint32_t i, size_t len)
{
    if (i + 1 >= len)
        return '\0';
    return src[i + 1];
}
