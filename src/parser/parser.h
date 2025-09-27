#ifndef PARSER_H
#define PARSER_H

#include "lexer/token.h"
#include "parser/node.h"
#include <stddef.h>

typedef struct
{
    Token *tokens;
    size_t pos;
    size_t token_count;
} Parser;

void eat(Parser *p, tokenType expect);
Token *current_token(Parser *p);
Parser *create_parser(Token *tokens, size_t token_count);
Node *parse(Parser *p);

Node *parse_block(Parser *p);
Node *parse_parameters(Parser *p);

#endif
