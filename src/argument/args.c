#include "version.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

void print_help()
{
    printf("CF %d.%d compiled at %s %s\n", VERSION_MAJOR, VERSION_MINOR,
           __DATE__, __TIME__);
    printf("usage:\n");
    printf("    --help -h to print this message.\n");
    printf("    --version -v to print version.\n");
    printf("    --input -i, specify which files to input.\n");
    printf("    --output -o, specify output name.\n");
    exit(0);
}

void process_args(int argc, char *argv[])
{
    int option;
    int option_index;
    static const struct option options[] = {
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'},
        {"input", required_argument, NULL, 'i'},
        {"output", required_argument, NULL, 'o'},
        {0, 0, 0, 0}};

    while ((option = getopt_long(argc, argv, "hvi:", options, &option_index)) !=
           -1)
    {
        switch (option)
        {
        case 'h':
            print_help();
            print_help();
        case 'v':
            printf("CF %d.%d compiled at %s %s\n", VERSION_MAJOR, VERSION_MINOR,
                   __DATE__, __TIME__);
            exit(0);
        case 'i':
        case 'o':
        case 0:
            break;
        }
    }
}
