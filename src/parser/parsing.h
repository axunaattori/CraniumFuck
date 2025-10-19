#ifndef PARSING_H
#define PARSING_H

#include "node.h"
#include "parser.h"

Node *parse_condition(Parser *p);
Node *parse_block(Parser *p);
Node **parse_parameters(Parser *p, size_t *size);
Node **parse_arguments(Parser *p, size_t *size);

Node *parse_token_void(Parser *p);
Node *parse_token_byte(Parser *p);
Node *parse_token_while(Parser *p);
Node *parse_identifier(Parser *p);

#endif
