#include "preprocessor.h"
#include "pphelper.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char *name;
    char *content;
} define;

// default
// __ARRAYSIZE__ 30000
// __TIME__
// __DATE__

char *
preprocessor (char *src, uint32_t length)
{
    define *defined = malloc ((length + 1) * sizeof (define));
    if (!defined)
        return NULL;

    uint32_t definedCount = 0;

    defined[definedCount++] = (define){ "__ARRAYSIZE__", "\"30000\"" };

    static char dateWithQuotes[20];
    sprintf (dateWithQuotes, "\"%s\"", ppdate ());
    static char timeWithQuotes[20];
    sprintf (timeWithQuotes, "\"%s\"", pptime ());

    defined[definedCount++] = (define){ "__DATE__", dateWithQuotes };
    defined[definedCount++] = (define){ "__TIME__", timeWithQuotes };

    uint32_t line = 0;
    for (uint32_t i = 0; i < length; i++)
        {
            char c = src[i];
            switch (c)
                {
                case '#':
                    break;
                case '\n':
                    line++;
                    break;
                default:
                    // if ' or " ignore till the next " or ' and skip.
                    if (c == '\'' || c == '"')
                        {
                            char startquote = c;
                            i++;
                            while (i < length && src[i] != startquote)
                                {
                                    if (src[i] == '\n')
                                        {
                                            line++;
                                        }
                                    if (i >= length)
                                        {
                                            printf ("preprocessor: Couldn't "
                                                    "find ending "
                                                    "quote (%c). "
                                                    "Problem at, "
                                                    "line: %d\n",
                                                    startquote, line);
                                            return NULL;
                                        }
                                    if (src[i] == '\\')
                                        {
                                            i++;
                                        }
                                    i++;
                                }
                            break;
                        }
                    break;
                }
        }
    return "example";
}
