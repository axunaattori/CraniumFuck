#ifndef IR_H
#define IR_H

#include <stdint.h>

typedef enum
{
    OP_SET,     // dest arg1/immediate
    OP_PUTCHAR, // arg1
    OP_GETCHAR, // dest
    OP_WHILE,   // arg1
    OP_END,     // nothing
    OP_ADD,     // dest arg1 arg2/immediate
    OP_SUB,     // dest arg1 arg2/immediate
} IR_OP;

/* probably will need to add more */

typedef struct
{
    IR_OP op;
    char *dest;
    char *arg1;
    char *arg2;
    uint8_t immediate;
} IR_instruction;

#endif
