#ifndef PPHELPER_H
#define PPHELPER_H

#include "preprocessor.h"
#include <stdint.h>

char *getseperated (char *src, uint32_t length, char c, int error,
                    uint32_t line, uint32_t *pos);

char *pptime ();

char *ppdate ();

void hashdefine (char *src, uint32_t length, uint32_t *definedCount,
                 uint32_t line, define **defined, uint32_t *pos);

void hasdundef (char *src, uint32_t length, uint32_t *definedCount,
                uint32_t line, define **defined, uint32_t *pos);

#endif
