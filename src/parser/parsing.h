#ifndef PARSING_H
#define PARSING_H

#include "node.h"
#include "parser.h"

Node *parse_block(Parser *p);
Node **parse_parameters(Parser *p, size_t *size);

Node *parse_token_void(Parser *p);
Node *parse_token_byte(Parser *p);
#endif
