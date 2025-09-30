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

void eat(Parser *p, tokenType expect);
void eat_err(Parser *p, tokenType expect, char *error_msg);
// eat but with custom errors
Token *current_token(Parser *p);
Parser *create_parser(Token *tokens, size_t token_count);
Node *parse(Parser *p);
Node **parsing_loop(Parser *p, size_t *amount);

#endif
