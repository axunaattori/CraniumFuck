#include "error.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

void
errorprint (char *text, uint32_t line, uint32_t column)
{
    printf ("%s\n", text);
    printf ("line: %" PRIu32 "\n", line);
    printf ("column: %" PRIu32 "\n", column);
}

void
uwarning (char *text, uint32_t line, uint32_t column)
{
    printf (YELLOW "WARNING:\n" RESET);
    errorprint (text, line, column);
}

void
uerror (char *text, uint32_t line, uint32_t column)
{
    printf (RED "ERROR:\n" RESET);
    errorprint (text, line, column);
    exit (1);
}
