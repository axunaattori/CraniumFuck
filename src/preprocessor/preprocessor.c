#include "preprocessor.h"
#include "config.h"
#include "pphelper.h"
#include "util/error.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO:
// (X = done):
// #define
// #undef
// #ifdef
// #ifndef
// #endif
// #else
// #elif
// #include
// #error
// #warning

// default
// __ARRAYSIZE__
// __TIME__
// __DATE__
// __VERSION__

char *
preprocessor (char *src, uint32_t length)
{
    define *defined = malloc (256 * sizeof (define));
    if (!defined)
        return NULL;

    uint32_t definedCount = 0;

    defined[definedCount++] = (define){ "__ARRAYSIZE__", ArraySize };

    static char dateWithQuotes[20];
    sprintf (dateWithQuotes, "\"%s\"", ppdate ());
    static char timeWithQuotes[20];
    sprintf (timeWithQuotes, "\"%s\"", pptime ());
    static char versionWithQuotes[20];
    sprintf (versionWithQuotes, "\"%s\"", ppversion ());

    defined[definedCount++] = (define){ "__DATE__", dateWithQuotes };
    defined[definedCount++] = (define){ "__TIME__", timeWithQuotes };
    defined[definedCount++] = (define){ "__VERSION__", versionWithQuotes };

    uint32_t column = 1;
    uint32_t line = 1;
    for (uint32_t pos = 0; pos < length; pos++)
        {
            char c = src[pos];
            switch (c)
                {
                case '#':
                    {
                        // planning to rewrite this
                    }
                case '\n':
                    line++;
                    column = 1;
                    break;
                case '"':
                case '\'':
                    {
                        char startquote = c;
                        pos++;
                        while (pos < length && src[pos] != startquote)
                            {
                                if (src[pos] == '\n')
                                    {
                                        line++;
                                        column = 0;
                                    }
                                else
                                    {
                                        column++;
                                    }
                                if (src[pos] == '\\')
                                    pos++; // skip escape
                                pos++;
                            }
                        if (pos >= length)
                            {
                                char buffer[1024];
                                snprintf (buffer, sizeof (buffer),
                                          "preprocessor: Couldn't "
                                          "find ending "
                                          "quote (%c). ",
                                          startquote);
                                uerror (buffer, line, column);
                            }

                        break;
                    }
                default:
                    // TODO: add function to check for defined stuff and expand
                    // them
                    break;
                }
            column++;
        }

#if printdebug
    printf ("printing defined stuff\n");
    for (uint32_t i = 0; i < definedCount; i++)
        {
            printf ("name: %s content: %s\n", defined[i].name,
                    defined[i].content);
        }
#endif
    free (defined);
    return "example"; // TODO: actually preprocess this and return the
                      // processed thing
}
