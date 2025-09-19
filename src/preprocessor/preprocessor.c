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
// comment removal X

// default
// __ARRAYSIZE__
// __TIME__
// __DATE__
// __VERSION__

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

void preprocessor(char *src, uint32_t length)
{
    define *defined = malloc(256 * sizeof(define));
    if (!defined)
        return;

    uint32_t defined_count = 0;

    defined[defined_count++] = (define){"__ARRAYSIZE__", STR(ARRAYSIZE)};

    static char date_with_quotes[20];
    sprintf(date_with_quotes, "\"%s\"", ppdate());
    static char time_with_quotes[20];
    sprintf(time_with_quotes, "\"%s\"", pptime());
    static char version_with_quotes[20];
    sprintf(version_with_quotes, "\"%s\"", ppversion());

    defined[defined_count++] = (define){"__DATE__", date_with_quotes};
    defined[defined_count++] = (define){"__TIME__", time_with_quotes};
    defined[defined_count++] = (define){"__VERSION__", version_with_quotes};

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
            if (peek(src, i, length) == '/')
            {
                while (i < length && src[i] != '\n' && src[i] != '\0')
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
            else if (peek(src, i, length) == '*')
            {
                uint32_t startline = line;
                uint32_t startcolumn = column;
                while (i < length)
                {
                    if (src[i] == '\0')
                    {
                        uerror("Multiline comments end "
                               "with */",
                               startline, startcolumn);
                    }
                    if (src[i] == '*' && peek(src, i, length) == '/')
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
                snprintf(buffer, sizeof(buffer),
                         "preprocessor: couldn't "
                         "find ending "
                         "quote (%c). ",
                         startquote);
                ufatal(buffer, line, column);
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
    printf("printing defined stuff\n");
    for (uint32_t i = 0; i < defined_count; i++)
    {
        printf("name: %s content: %s\n", defined[i].name, defined[i].content);
    }
#endif
    free(defined);
}
