#pragma once
#include <stdint.h>
#include "token.h"

char escape(char c);
Token* lexer(char *src, uint32_t length);
