#include "config.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "preprocessor/preprocessor.h"
#include "version.h"
#include <stdio.h>
#include <string.h>

char source[] = "void main()\n"
                "{\n"
                "byte terms = 13;\n"
                "byte a = 0, b = 1, next;\n"
                "\n"
                "while(terms)\n"
                "{\n"
                "next = a + b;\n"
                "putchar (next); // prints in ASCII, don't mind.\n"
                "putchar ('\\n');\n"
                "a = b;\n"
                "b = next;\n"
                "terms--;\n"
                "}\n"
                "}";

// in case if the user isnt using cmake.
#ifndef CMAKE_COMPILER_NAME
#define CMAKE_COMPILER_NAME "Unknown"
#endif
#ifndef CMAKE_COMPILER_VERSION
#define CMAKE_COMPILER_VERSION "Unknown"
#endif

int
main (int argc, char *argv[])
{
    printf ("CF %d.%d compiled at %s %s, with %s %s\n", VERSION_MAJOR,
            VERSION_MINOR, __DATE__, __TIME__, CMAKE_COMPILER_NAME,
            CMAKE_COMPILER_VERSION);
    preprocessor (source, strlen (source));

    printf ("Preprocessing Done!\n");

    Token *tokens = lexer (source, strlen (source));

    if (tokens == NULL)
        {
            printf ("Lexer: Build failed");
            return 1;
        }

#if PRINTDEBUG
    for (int i = 0; tokens[i].type != TOKEN_EOF; i++)
        {
            printf ("type = %s, lexeme = %s, line = %d, column = %d\n",
                    token_type_to_string (tokens[i].type), tokens[i].lexeme,
                    tokens[i].line, tokens[i].column);
        }
#endif
    printf ("Lexering Done!\n");

    printf ("Done building.");

    return 0;
}
