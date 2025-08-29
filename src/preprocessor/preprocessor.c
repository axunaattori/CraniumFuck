#include "preprocessor.h"
#include "config.h"
#include "pphelper.h"
#include "util/error.h"
#include "util/helper.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// TODO:
// (X = done)
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
// comment removal

// default
// __ARRAYSIZE__
// __TIME__
// __DATE__
// __VERSION__

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER (x)

void
preprocessor (char *src, uint32_t length)
{
    define *defined = malloc (256 * sizeof (define));
    if (!defined)
        return;

    uint32_t definedCount = 0;

    defined[definedCount++] = (define){ "__ARRAYSIZE__", STR (ARRAYSIZE) };

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
    for (uint32_t i = 0; i < length; i++)
        {
            char c = src[i];
            switch (c)
                {
                case '#':
                    {
                        // planning to rewrite this
                        break;
                    }
                case '\n':
                    line++;
                    column = 1;
                    break;
                case '/':
                    if (peek (src, i, length) == '/')
                        {
                            while (i < length && src[i] != '\n'
                                   && src[i] != '\0')
                                {
                                    src[i] = ' ';
                                    i++;
                                    column++;
                                }
                            if (i < length && src[i] == '\n')
                                {
                                    line++;
                                    column = 1;
                                }
                            continue;
                        }
                    else if (peek (src, i, length) == '*')
                        {
                            uint32_t startline = line;
                            uint32_t startcolumn = column;
                            while (i < length)
                                {
                                    if (src[i] == '\0')
                                        {
                                            uerror ("Multiline comments end "
                                                    "with */",
                                                    startline, startcolumn);
                                        }
                                    if (src[i] == '*'
                                        && peek (src, i, length) == '/')
                                        {
                                            src[i++] = ' ';
                                            src[i++] = ' ';
                                            column += 2;
                                            break;
                                        }
                                    else if (src[i] == '\n')
                                        {
                                            line++;
                                            i++;
                                            column = 1;
                                        }
                                    else
                                        {
                                            src[i] = ' ';
                                            i++;
                                            column++;
                                        }
                                }
                            continue;
                        }
                    break;
                case '"':
                case '\'':
                    {
                        char startquote = c;
                        i++;
                        while (i < length && src[i] != startquote)
                            {
                                if (src[i] == '\n')
                                    {
                                        line++;
                                        column = 0;
                                    }
                                else
                                    {
                                        column++;
                                    }
                                if (src[i] == '\\')
                                    i++; // skip escape, the preprocessor
                                         // doesnt have any use for it.
                                i++;
                            }
                        if (i >= length)
                            {
                                char buffer[1024];
                                snprintf (buffer, sizeof (buffer),
                                          "preprocessor: couldn't "
                                          "find ending "
                                          "quote (%c). ",
                                          startquote);
                                uerror (buffer, line, column);
                            }

                        break;
                    }
                default:
                    // todo: add function to check for defined stuff and
                    // expand them
                    break;
                }
            column++;
        }

#if PRINTDEBUG
    printf ("printing defined stuff\n");
    for (uint32_t i = 0; i < definedCount; i++)
        {
            printf ("name: %s content: %s\n", defined[i].name,
                    defined[i].content);
        }
#endif
    free (defined);
}
