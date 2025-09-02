#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <stdint.h>

Token *lexer (char *src, uint32_t length);

#endif
