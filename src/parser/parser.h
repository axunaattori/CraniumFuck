#ifndef PARSER_H
#define PARSER_H

#include "lexer/token.h"
#include <stddef.h>

typedef struct
{
    Token *tokens;
    size_t pos;
    size_t token_count;
} Parser;

void eat(Parser *p, tokenType expect);

#endif
