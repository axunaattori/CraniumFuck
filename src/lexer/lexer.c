#include "lexer.h"
#include "util/error.h"
#include "util/helper.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

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
                                                     "[", line, column };
                    column++;
                    break;
                case ']':
                    tokens[token_count++] = (Token){ TOKEN_CLOSED_SQUARE_BRACE,
                                                     "]", line, column };
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
                case ',':
                    tokens[token_count++]
                        = (Token){ TOKEN_COMMA, ",", line, column };
                    column++;
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
                                    i++;
                                    column++;
                                    if (i >= length)
                                        {
                                            uerror ("Couldn't find ending.",
                                                    line, column);
                                        }
                                }

                            i--; // adjust

                            uint32_t leng = i - start + 1;
                            char *lexeme = malloc (leng + 1);
                            if (!lexeme)
                                {
                                    uerror ("Memory Error", line, column);
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
                            else if (strcmp (lexeme, "if") == 0)
                                {
                                    tokens[token_count++]
                                        = (Token){ TOKEN_IF, lexeme, line,
                                                   startcolumn };
                                    break;
                                }
                            else if (strcmp (lexeme, "void") == 0)
                                {
                                    tokens[token_count++]
                                        = (Token){ TOKEN_VOID, lexeme, line,
                                                   startcolumn };
                                    break;
                                }
                            else if (strcmp (lexeme, "return") == 0)
                                {
                                    tokens[token_count++]
                                        = (Token){ TOKEN_RETURN, lexeme, line,
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
                                    i++;
                                    column++;
                                    if (i >= length)
                                        {
                                            uerror ("Couldn't find ending "
                                                    "(number).",
                                                    line, column);
                                        }
                                }

                            i--; // adjust

                            uint32_t leng = i - start + 1;
                            char *lexeme = malloc (leng + 1);
                            if (!lexeme)
                                {
                                    uerror ("Memory Error", line, column);
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
                                    uerror ("Memory Error", line, column);
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
                                                    free (strarray);
                                                    uerror ("Escape character "
                                                            "at end of input.",
                                                            line, column);
                                                }
                                            char escaped = escape (src[i]);
                                            if (len >= arraysize - 1)
                                                {
                                                    arraysize *= 2;
                                                    char *temp = realloc (
                                                        strarray, arraysize);
                                                    if (!temp)
                                                        {
                                                            free (strarray);
                                                            uerror (
                                                                "Memory Error",
                                                                line, column);
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
                                                    free (strarray);
                                                    uerror ("Memory Error",
                                                            line, column);
                                                }
                                            strarray = temp;
                                        }
                                    strarray[len++] = src[i];
                                    i++;
                                    column++;
                                }
                            if (i >= length || src[i] != startquote)
                                {
                                    free (strarray);
                                    char buffer[1024];
                                    sprintf (buffer,
                                             "Lexer: Couldn't find "
                                             "ending quote (%c)",
                                             startquote);
                                    uerror ("Lexer: Couldn't find ending "
                                            "quote (%c)",
                                            line, column);
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
