#include "preprocessor.h"
#include "config.h"
#include "pphelper.h"
#include "util/error.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t pos = 0; // probably not the best code.

char *
getseperated (char *src, uint32_t length, char c, int error, uint32_t line)
{
    uint32_t start = pos;
    while (src[pos] != c)
        {
            pos++;
            if (pos >= length)
                {
                    if (error != 0)
                        uerror ("Couldn't find ending to #define!", line, 0);
                }
        }

    uint32_t size = pos - start;
    char *text = malloc (size + 1);
    if (!text)
        return NULL;

    memcpy (text, &src[start], size);
    text[size] = '\0';

    return text;
}

typedef struct
{
    char *name;
    char *content;
} define;

// stuff to add (X = done):
// #define X (no fuction support yet)
// #undef
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

    uint32_t line = 0;
    while (pos < length)
        {
            char c = src[pos];
            switch (c)
                {
                case '#':
                    {
                        char *type = getseperated (src, length, ' ', 1, 0);
                        if (type == NULL)
                            {
                                return NULL;
                            }
                        if (strcmp (type, "#define") == 0)
                            {
                                // pos++ is used to offset off of the space
                                pos++;
                                char *name
                                    = getseperated (src, length, ' ', 1, 0);
                                if (name == NULL)
                                    {
                                        free (type);
                                        return NULL;
                                    }
                                for (uint32_t i = 0; i < definedCount; i++)
                                    {
                                        if (strcmp (name, defined[i].name)
                                            == 0)
                                            {
                                                char buffer[1024];
                                                sprintf (
                                                    buffer,
                                                    "%s is already defined, "
                                                    "please use #undef before "
                                                    "redefining it.",
                                                    name);
                                                uerror (buffer, line, 0);
                                            }
                                    }
                                pos++;
                                char *text
                                    = getseperated (src, length, '\n', 0, 0);
                                if (text == NULL)
                                    {
                                        text = getseperated (
                                            src, length, '\0', 1,
                                            0); // check for null even tho its
                                                // at the end, making this
                                                // define fucking useless.
                                        if (text == NULL)
                                            {
                                                free (type);
                                                free (name);
                                                return NULL;
                                            }
                                    }
                                else
                                    {
                                        line++;
                                    }
                                defined[definedCount++]
                                    = (define){ strdup (name), strdup (text) };
                                free (name);
                            }
                        else if (strcmp (type, "#undef") == 0)
                            {
                            }
                        else if (strcmp (type, "#error") == 0)
                            {
                                pos++;
                                char *text
                                    = getseperated (src, length, '\n', 1, 0);
                                uerror (text, line, 0);
                            }
                        else if (strcmp (type, "#warning") == 0)
                            {
                                pos++;
                                char *text
                                    = getseperated (src, length, '\n', 1, 0);
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
                default:
                    // todo: add function to check for defined stuff and
                    // replace
                    //  if ' or " ignore till the next " or ' and skip.
                    if (c == '\'' || c == '"')
                        {
                            char startquote = c;
                            pos++;
                            while (pos < length && src[pos] != startquote)
                                {
                                    if (src[pos] == '\n')
                                        {
                                            line++;
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
                                    if (src[pos] == '\\')
                                        {
                                            pos++;
                                        }
                                    pos++;
                                }
                            break;
                        }
                    break;
                }
            pos++;
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
