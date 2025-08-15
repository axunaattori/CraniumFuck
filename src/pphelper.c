#include "pphelper.h"
#include <stdio.h>
#include <time.h>

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
