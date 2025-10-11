#ifndef HELPER_H
#define HELPER_H

#include <stddef.h>
#include <stdint.h>

unsigned char escape(char *c);
char peek(char *src, uint32_t i, size_t len);

#endif
