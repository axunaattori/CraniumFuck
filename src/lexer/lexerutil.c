#include <stddef.h>
#include <stdint.h>
char
peek (char *src, uint32_t i, size_t len)
{
    if (i + 1 >= len)
        return ' '; // if this is the case, then this is the last token. and it
                    // would fail compiling anyway.
    return src[i + 1];
}
