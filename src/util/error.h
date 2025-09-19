#ifndef ERROR_H
#define ERROR_H

#include <stdbool.h>
#include <stdint.h>

#define RED "\033[31m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

bool get_error_flag();

void uwarning(char *text, uint32_t line, uint32_t column);

void uerror(char *text, uint32_t line, uint32_t column);

void ufatal(char *text, uint32_t line, uint32_t column);

#endif
