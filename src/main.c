#include "config.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "preprocessor/preprocessor.h"
#include "version.h"
#include <stdio.h>
#include <string.h>

char source[2048]
    = "void main()\n"
      "  {\n"
      "  byte a = 0; //comment test\n"
      "  byte b = 255; /* another comment test */\n"
      "  byte String[64] = \"hello, new line example: \\n, quote "
      "  inside quote example: \\\" '\";\n"
      "  byte char = 'a';\n"
      "  while (b)\n"
      "  {\n"
      "    putchar(a);\n"
      "    a++;\n"
      "    b--;\n"
      "  }\n"
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
                    tokenTypeToString (tokens[i].type), tokens[i].lexeme,
                    tokens[i].line, tokens[i].column);
        }
#endif
    printf ("Lexering Done!\n");

    printf ("Done building.");

    return 0;
}
