// TODO: make it possible to input files and an engine for arguments, a simple
// one at that would be nice.

#include "version.h"
#include <stdio.h>

void print_help()
{
    printf("CF %d.%d compiled at %s %s\n", VERSION_MAJOR, VERSION_MINOR,
           __DATE__, __TIME__);
    printf("usage:\n");
    printf("    -h to print this message\n");
    // actually work on this and add more
}

void process_args(int argc, char *argv[]) {}
