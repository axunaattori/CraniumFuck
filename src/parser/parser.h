#ifndef PARSER_H
#define PARSER_H

#include "config.h"
#include "lexer/token.h"
#include "parser/node.h"
#include <stdbool.h>
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

bool eat(Parser *p, tokenType expect);
// eat but with custom errors
bool eat_err(Parser *p, tokenType expect, const char *error_msg);
// returns true if expect = current_token at parser *p
bool match(Parser *p, tokenType expect);
// match but it also eats
bool match_eat(Parser *p, tokenType expect);
Token *previous_token(Parser *p);
Token *current_token(Parser *p);
Parser *create_parser(Token *tokens, size_t token_count);
void free_parser(Parser *p);
Node *parse(Parser *p);
Node **parsing_loop(Parser *p, size_t *amount);

#endif
