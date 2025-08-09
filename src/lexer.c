#include "lexer.h"

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

char
escape (char c)
{
    switch (c)
        {
        case 'n':
            return '\n';
        case 't':
            return '\t';
        case '\\':
            return '\\';
        case '"':
            return '"';
        case '\'':
            return '\'';
        case 'b':
            return '\b';
        case 'r':
            return '\r';
        case 'f':
            return '\f';
        default:
            return c;
        }
}

Token *
lexer (char *src, uint32_t length)
{
    Token *tokens = malloc ((length + 1) * sizeof (Token));
    if (!tokens)
        return NULL;

    uint32_t token_count = 0;
    int column = 1;
    int line = 1;

    for (uint32_t i = 0; i < length; i++)
        {
            char c = src[i];
            switch (c)
                {
                case '(':
                    tokens[token_count++]
                        = (Token){ TOKEN_OPEN_PARENTHESIS, "(", line, column };
                    column++;
                    break;
                case ')':
                    tokens[token_count++] = (Token){ TOKEN_CLOSE_PARENTHESIS,
                                                     ")", line, column };
                    column++;
                    break;
                case '{':
                    tokens[token_count++]
                        = (Token){ TOKEN_OPEN_BRACE, "{", line, column };
                    column++;
                    break;
                case '}':
                    tokens[token_count++]
                        = (Token){ TOKEN_CLOSE_BRACE, "}", line, column };
                    column++;
                    break;
                case '[':
                    tokens[token_count++] = (Token){ TOKEN_OPEN_SQUARE_BRACE,
                                                     "{", line, column };
                    column++;
                    break;
                case ']':
                    tokens[token_count++] = (Token){ TOKEN_CLOSED_SQUARE_BRACE,
                                                     "}", line, column };
                    column++;
                    break;
                case ';':
                    tokens[token_count++]
                        = (Token){ TOKEN_SEMICOLON, ";", line, column };
                    column++;
                    break;
                case '=':
                    tokens[token_count++]
                        = (Token){ TOKEN_EQUALS, "=", line, column };
                    column++;
                    break;
                case '+':
                    if (src[i + 1] == '+')
                        {
                            tokens[token_count++]
                                = (Token){ TOKEN_ADD, "++", line, column };
                            i++;
                            column += 2;
                        }
                    else
                        {
                            tokens[token_count++]
                                = (Token){ TOKEN_PLUS, "+", line, column };
                            column++;
                        }
                    break;
                case '-':
                    if (src[i + 1] == '-')
                        {
                            tokens[token_count++]
                                = (Token){ TOKEN_SUBTRACT, "--", line,
                                           column };
                            i++;
                            column += 2;
                        }
                    else
                        {
                            tokens[token_count++]
                                = (Token){ TOKEN_MINUS, "-", line, column };
                            column++;
                        }
                    break;
                case '\n':
                    column = 1;
                    line++;
                    break;

                default: // complete rewrite
                    if (isspace (c))
                        {
                            column++;
                            break;
                        }

                    uint32_t start = i;
                    int startcolumn = column;
                    if (c == '_' || isalpha (c)) // identifiers and keywords
                        {
                            while ((src[i]) == '_' || isalpha (src[i]))
                                {
                                    printf ("%d %d %c\n", i, column, src[i]);
                                    i++;
                                    column++;
                                    if (i >= length)
                                        {
                                            printf ("Couldn't find ending "
                                                    "(number), problematic"
                                                    " Couldne starts at "
                                                    "column : "
                                                    "%d, line: %d\n",
                                                    startcolumn, line);
                                            printf (
                                                "This most likely isn't"
                                                "your code's problem, as this "
                                                "should be impossible"
                                                " to my knowledge.\n");
                                            return NULL;
                                        }
                                }
                            printf ("%d %d %c\n", i, column, src[i]);

                            i--; // adjust

                            uint32_t leng = i - start + 1;
                            char *lexeme = malloc (leng);
                            if (!lexeme)
                                {
                                    printf ("Memory error\n");
                                    return NULL;
                                }
                            strncpy (lexeme, &src[start], leng);
                            lexeme[leng] = '\0';

                            if (strcmp (lexeme, "byte") == 0)
                                {
                                    tokens[token_count++]
                                        = (Token){ TOKEN_BYTE, lexeme, line,
                                                   startcolumn };
                                    break;
                                }
                            else if (strcmp (lexeme, "array") == 0)
                                {
                                    tokens[token_count++]
                                        = (Token){ TOKEN_ARRAY, lexeme, line,
                                                   startcolumn };
                                    break;
                                }
                            else if (strcmp (lexeme, "putchar") == 0)
                                {
                                    tokens[token_count++]
                                        = (Token){ TOKEN_PUTCHAR, lexeme, line,
                                                   startcolumn };
                                    break;
                                }
                            else if (strcmp (lexeme, "putchar") == 0)
                                {
                                    tokens[token_count++]
                                        = (Token){ TOKEN_PUTCHAR, lexeme, line,
                                                   startcolumn };
                                    break;
                                }
                            else if (strcmp (lexeme, "while") == 0)
                                {
                                    tokens[token_count++]
                                        = (Token){ TOKEN_WHILE, lexeme, line,
                                                   startcolumn };
                                    break;
                                }
                            else
                                {
                                    tokens[token_count++]
                                        = (Token){ TOKEN_IDENTIFIER, lexeme,
                                                   line, startcolumn };
                                    break;
                                }
                        }
                    if (isdigit (c))
                        {
                            while (isdigit (src[i]))
                                {
                                    printf ("%d %d %c\n", i, column, src[i]);
                                    i++;
                                    column++;
                                    if (i >= length)
                                        {
                                            printf ("Couldn't find ending "
                                                    "(number), problematic"
                                                    " Couldne starts at "
                                                    "column : "
                                                    "%d, line: %d\n",
                                                    startcolumn, line);
                                            printf (
                                                "This most likely isn't"
                                                "your code's problem, as this "
                                                "should be impossible"
                                                " to my knowledge.\n");
                                            return NULL;
                                        }
                                }
                            printf ("%d %d %c\n", i, column, src[i]);

                            i--; // adjust

                            uint32_t leng = i - start + 1;
                            char *lexeme = malloc (leng);
                            if (!lexeme)
                                {
                                    printf ("Memory error\n");
                                    return NULL;
                                }
                            strncpy (lexeme, &src[start], leng);
                            lexeme[leng] = '\0';

                            tokens[token_count++]
                                = (Token){ TOKEN_NUMBER, lexeme, line,
                                           startcolumn };
                            break;
                        }
                    if (c == '\'' || c == '"') // strings
                        {
                            char startquote = c;
                            int startcolumn = column;
                            size_t arraysize = 128;
                            size_t len = 0;
                            char *strarray = malloc (arraysize);
                            if (!strarray)
                                {
                                    printf ("Memory error\n");
                                    return NULL;
                                }
                            i++; // skip opening quote
                            column++;
                            while (i < length && src[i] != startquote)
                                {
                                    if (src[i] == '\n')
                                        {
                                            line++;
                                            column = 1;
                                            i++;
                                            continue;
                                        }
                                    if (src[i] == '\\')
                                        {
                                            i++;
                                            column++;
                                            if (i >= length)
                                                {
                                                    printf (
                                                        "Escape character at "
                                                        "end of input. "
                                                        "Problem at column: "
                                                        "%d, line: %d\n",
                                                        startcolumn, line);
                                                    free (strarray);
                                                    return NULL;
                                                }
                                            char escaped = escape (src[i]);
                                            if (len >= arraysize - 1)
                                                {
                                                    arraysize *= 2;
                                                    char *temp = realloc (
                                                        strarray, arraysize);
                                                    if (!temp)
                                                        {
                                                            printf ("Memory "
                                                                    "error\n");
                                                            free (strarray);
                                                            return NULL;
                                                        }
                                                    strarray = temp;
                                                }
                                            strarray[len++] = escaped;
                                            i++;
                                            column++;
                                            continue;
                                        }
                                    if (len >= arraysize - 1)
                                        {
                                            arraysize *= 2;
                                            char *temp = realloc (strarray,
                                                                  arraysize);
                                            if (!temp)
                                                {
                                                    printf ("Memory error\n");
                                                    free (strarray);
                                                    return NULL;
                                                }
                                            strarray = temp;
                                        }
                                    strarray[len++] = src[i];
                                    i++;
                                    column++;
                                }
                            if (i >= length || src[i] != startquote)
                                {
                                    printf (
                                        "Couldn't find ending quote (%c). "
                                        "Problem at column: %d, line: %d\n",
                                        startquote, startcolumn, line);
                                    free (strarray);
                                    return NULL;
                                }
                            strarray[len] = '\0';
                            tokens[token_count++]
                                = (Token){ TOKEN_STRING, strarray, line,
                                           startcolumn };
                            column++;
                            break;
                        }
                }
        }

    tokens[token_count++] = (Token){ TOKEN_EOF, "EOF", line, column };

    return tokens;
}
