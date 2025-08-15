#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <stdint.h>

char escape (char c);
Token *lexer (char *src, uint32_t length);

#endif
