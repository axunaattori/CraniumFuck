#include "config.h"
#include "lexer.h"
#include "preprocessor.h"
#include "token.h"
#include <stdio.h>

char source[2048]
    = "#define hello test\n" // should be removed by the preprocessor!
      "byte a = 0;\n"
      "byte b = 255;\n"
      "byte String[64] = \"hello, new line example: \\n, quote "
      "inside quote example: \\\" \\\'\"\n"
      "while (b)\n"
      "{\n"
      "    putchar(a);\n"
      "    a++;\n"
      "    b--;\n"
      "}\n";

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
    char *Newsrc = preprocessor (source, sizeof (source));
    if (Newsrc == NULL)
        {
            return 1;
        }

    printf ("Preprocessing Done!\n");

    Token *tokens = lexer (source, sizeof (source));

    if (tokens == NULL)
        {
            return 1;
        }

#if printdebug
    for (int i = 0; tokens[i].type != TOKEN_EOF; i++)
        {
            printf ("type = %s, lexeme = %s, line = %d, column = %d\n",
                    tokenTypeToString (tokens[i].type), tokens[i].lexeme,
                    tokens[i].line, tokens[i].column);
        }
#endif
    printf ("Lexering Done!\n");

    return 0;
}
