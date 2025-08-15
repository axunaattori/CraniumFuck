#include "preprocessor.h"
#include "config.h"
#include "pphelper.h"
#include "util/error.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// stuff to add (X = done):
// #define X (no fuction support yet)
// #undef X (i believe so ill go sleep dont want to test it rn)
// #ifdef
// #ifndef
// #endif
// #else
// #elif
// #include
// #error X
// #warning X

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

    uint32_t line = 1;
    for (uint32_t pos = 0; pos < length; pos++)
        {
            char c = src[pos];
            switch (c)
                {
                case '#':
                    {
                        char *type
                            = getseperated (src, length, ' ', 1, 0, &pos);
                        if (type == NULL)
                            {
                                return NULL;
                            }
                        if (strcmp (type, "#define") == 0)
                            {
                                hashdefine (src, length, &definedCount, line,
                                            &defined, &pos);
                            }
                        else if (strcmp (type, "#undef") == 0)
                            {
                                hasdundef (src, length, &definedCount, line,
                                           &defined, &pos);
                            }
                        else if (strcmp (type, "#error") == 0)
                            {
                                pos++;
                                char *text = getseperated (src, length, '\n',
                                                           1, 0, &pos);
                                uerror (text, line, 0);
                            }
                        else if (strcmp (type, "#warning") == 0)
                            {
                                pos++;
                                char *text = getseperated (src, length, '\n',
                                                           1, 0, &pos);
                                uwarning (text, line, 0);
                                free (text);
                                line++;
                            }
                        free (type);
                        break;
                    }
                case '\n':
                    line++;
                    break;
                case '"':
                case '\'':
                    {
                        char startquote = c;
                        pos++;
                        while (pos < length && src[pos] != startquote)
                            {
                                if (src[pos] == '\n')
                                    line++;
                                if (src[pos] == '\\')
                                    pos++; // skip escape
                                pos++;
                            }
                        if (pos >= length)
                            {
                                char buffer[1024];
                                sprintf (buffer,
                                         "preprocessor: Couldn't "
                                         "find ending "
                                         "quote (%c). ",
                                         startquote);
                                uerror (buffer, line, 0);
                            }

                        break;
                    }
                default:
                    // todo: add function to check for defined stuff and
                    break;
                }
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
    return "example";
}
