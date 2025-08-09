#include "lexer.h"
#include "token.h"
#include <stdio.h>

char source[2048] = "byte a = 0;\n"
                    "byte b = 255;\n"
                    "array String[64] = \"hello, new line example: \\n, quote "
                    "inside quote example: \\\" \\\'\"\n"
                    "while (b)\n"
                    "{\n"
                    "    putchar(a);\n"
                    "    a++;\n"
                    "    b--;\n"
                    "}";

/*  somewhat what the compiled result should look like:
 *
 *     byte a;             (cell 0)
 * >-  byte b; (underflow) (cell 1)
 * [   while
 *   <. putchar(a);
 *   +  a++;
 *   >-  b--; and (b)
 * ]   }
 */

int
main ()
{
    printf ("CF compiled at %s %s\n", __DATE__, __TIME__);
    Token *tokens = lexer (source, sizeof (source));

    if (tokens == NULL)
        {
            return 1;
        }

    for (int i = 0; tokens[i].type != TOKEN_EOF; i++)
        {
            printf ("type = %s, lexeme = %s, line = %d, column = %d\n",
                    tokenTypeToString (tokens[i].type), tokens[i].lexeme,
                    tokens[i].line, tokens[i].column);
        }

    return 0;
}
