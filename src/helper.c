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
