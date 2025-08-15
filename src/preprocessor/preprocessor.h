#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

typedef struct
{
    char *name;
    char *content;
} define;

#include <stdint.h>

char *preprocessor (char *src, uint32_t length);

#endif
