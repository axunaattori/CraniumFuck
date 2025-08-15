#include "pphelper.h"
#include "preprocessor/preprocessor.h"
#include "util/error.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *
getseperated (char *src, uint32_t length, char c, int error, uint32_t line,
              uint32_t *pos)
{
    uint32_t start = *pos;
    while (src[*pos] != c)
        {
            (*pos)++;
            if (*pos >= length)
                {
                    if (error != 0)
                        uerror ("Couldn't find ending to #define!", line, 0);
                }
        }

    uint32_t size = *pos - start;
    char *text = malloc (size + 1);
    if (!text)
        return NULL;

    memcpy (text, &src[start], size);
    text[size] = '\0';

    return text;
}

char *
pptime ()
{
    time_t t = time (NULL);
    struct tm tm = *localtime (&t);

    // 30 should be enough for both.
    static char time[30];

    snprintf (time, sizeof (time), "%02d:%02d:%02d", tm.tm_hour, tm.tm_min,
              tm.tm_sec);

    return time;
}

char *
ppdate ()
{
    time_t t = time (NULL);
    struct tm tm = *localtime (&t);

    static char date[30];
    char month[4]; // get the current month ("Aug" for example) null included

    strftime (month, sizeof (month), "%b", &tm);

    snprintf (date, sizeof (date), "%s %02d %04d", month, tm.tm_mday,
              tm.tm_year + 1900);

    return date;
}

void
hashdefine (char *src, uint32_t length, uint32_t *definedCount, uint32_t line,
            define **defined, uint32_t *pos)
{
    (*pos)++; // move past the space
    char *name = getseperated (src, length, ' ', 1, 0, pos);
    if (!name)
        uerror ("missing identifier for #define", line, 0);

    for (uint32_t i = 0; i < *definedCount; i++)
        {
            if (strcmp (name, (*defined)[i].name) == 0)
                {
                    char buffer[1024];
                    sprintf (buffer,
                             "%s is already defined, use #undef first.", name);
                    uerror (buffer, line, 0);
                }
        }

    (*pos)++;
    char *text = getseperated (src, length, '\n', 0, 0, pos);
    if (!text)
        {
            text = getseperated (
                src, length, '\0', 1, 0,
                pos); // check for null even tho its // at the end, making this
                      // // define fucking useless.
            if (!text)
                {
                    free (name);
                    uerror ("missing text", line, 0);
                }
        }
    else
        {
            line++;
        }

    (*defined)[*definedCount].name = strdup (name);
    (*defined)[*definedCount].content = strdup (text);
    (*definedCount)++;

    free (name);
    if (text)
        free (text);
}

void
hasdundef (char *src, uint32_t length, uint32_t *definedCount, uint32_t line,
           define **defined, uint32_t *pos)
{
    (*pos)++; // move past the space
    char *name = getseperated (src, length, '\n', 0, 0, pos);
    if (!name)
        {
            name = getseperated (
                src, length, '\0', 1, 0,
                pos); // check for null even tho its // at the end, making this
                      // // define fucking useless.
            if (!name)
                {
                    uerror ("You didn't tell what to undef.", line, 0);
                }
        }

    for (uint32_t i = 0; i < *definedCount; i++)
        {
            if (strcmp (name, (*defined)[i].name) == 0)
                {
                    free ((*defined)[i].name);
                    free ((*defined)[i].content);

                    for (uint32_t j = i; j < *definedCount - 1; j++)
                        {
                            (*defined)[j] = (*defined)[j + 1];
                        }

                    (*definedCount)--;
                    break;
                }
        }
}
