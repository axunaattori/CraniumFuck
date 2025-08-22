#include "pphelper.h"
#include "util/error.h"
#include "version.h"
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

char *
ppversion ()
{
    static char ver[16];
    snprintf (ver, sizeof (ver), "%d.%d", VERSION_MAJOR, VERSION_MINOR);
    return ver;
}
