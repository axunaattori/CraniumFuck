#ifndef PARSER_H
#define PARSER_H

#include "config.h"
#include "lexer/token.h"
#include "parser/node.h"
#include <stddef.h>

typedef struct
{
    Token *tokens;
    size_t pos;
    size_t token_count;
} Parser;

#if PRINTDEBUG
void print_node(Node *node, int indent);
#endif
Node *parse_block(Parser *p);
Node **parse_parameters(Parser *p, size_t *size);
void eat(Parser *p, tokenType expect);
Token *current_token(Parser *p);
Parser *create_parser(Token *tokens, size_t token_count);
Node *parse(Parser *p);
Node **parsing_loop(Parser *p, size_t *amount);

#endif
