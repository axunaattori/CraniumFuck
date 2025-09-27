#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <stddef.h>
#include <stdint.h>

Token *lexer(char *src, uint32_t length, size_t *token_amount);

#endif
