#include "parser.h"
#include "lexer/token.h"
#include "util/error.h"
#include <stdio.h>

void eat(Parser *p, tokenType expect)
{
    if (p->pos >= p->token_count)
    {
        ufatal("Parser: pos is somehow bigger than token_count", -1, -1);
    }
    if (p->tokens[p->pos].type == expect)
    {
        p->pos++;
    }
    else
    {
        char buffer[128];
        snprintf(buffer, sizeof(buffer), "Expected %s but got %s",
                 token_type_to_string(expect),
                 token_type_to_string(p->tokens[p->pos].type));
        uerror(buffer, p->tokens[p->pos].line, p->tokens[p->pos].column);
    }
}
