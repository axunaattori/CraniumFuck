#include "error.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

static bool error_flag = false;

bool get_error_flag() { return error_flag; }

void errorprint(const char *text, uint32_t line, uint32_t column)
{
    printf("%s\n", text);
    printf("line: %" PRIu32 "\n", line);
    printf("column: %" PRIu32 "\n", column);
}

void uwarning(const char *text, uint32_t line, uint32_t column)
{
    printf(YELLOW "WARNING:\n" RESET);
    errorprint(text, line, column);
}

void uerror(const char *text, uint32_t line, uint32_t column)
{
    printf(RED "ERROR:\n" RESET);
    errorprint(text, line, column);
    error_flag = true;
}

// while errors are still fatal
// (quitting once processing it all so the user doesnt have to keep compiling),
// ufatal tells us that something is so fucked up that it cannot continue
// processing it anymore.
void ufatal(const char *text, uint32_t line, uint32_t column)
{
    printf(RED "!!FATAL!!:\n" RESET);
    errorprint(text, line, column);
    exit(1);
}
