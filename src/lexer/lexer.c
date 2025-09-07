#include "lexer.h"
#include "util/error.h"
#include "util/helper.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

// to the person reading this: make this better if you want.
// mainly the default: sucks.

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
                    tokens[token_count++] = (Token){ TOKEN_CLOSE_SQUARE_BRACE,
                                                     "]", line, column };
                    column++;
                    break;
                case ';':
                    tokens[token_count++]
                        = (Token){ TOKEN_SEMICOLON, ";", line, column };
                    column++;
                    break;
                case '%':
                    tokens[token_count++]
                        = (Token){ TOKEN_MODULO, "%", line, column };
                    column++;
                    break;
                case '&':
                    if (peek (src, i, length) == '&')
                        {
                            tokens[token_count++]
                                = (Token){ TOKEN_LOGIC_AND, "&&", line,
                                           column };
                            i++;
                            column += 2;
                        }
                    else
                        {
                            tokens[token_count++]
                                = (Token){ TOKEN_AMPERSAND, "&", line,
                                           column };
                            column++;
                        }
                    break;
                case '~':
                    tokens[token_count++]
                        = (Token){ TOKEN_BIT_NOT, "~", line, column };
                    column++;
                    break;
                case '^':
                    tokens[token_count++]
                        = (Token){ TOKEN_BIT_XOR, "^", line, column };
                    column++;
                    break;
                case '.':
                    tokens[token_count++]
                        = (Token){ TOKEN_DOT, ".", line, column };
                    column++;
                    break;
                case '|':
                    if (peek (src, i, length) == '|')
                        {
                            tokens[token_count++]
                                = (Token){ TOKEN_LOGIC_OR, "||", line,
                                           column };
                            i++;
                            column += 2;
                        }
                    else
                        {
                            tokens[token_count++]
                                = (Token){ TOKEN_BIT_OR, "|", line, column };
                            column++;
                        }
                    break;
                case '>':
                    if (peek (src, i, length) == '=')
                        {
                            tokens[token_count++]
                                = (Token){ TOKEN_LESS_EQUAL, ">=", line,
                                           column };
                            i++;
                            column += 2;
                        }
                    else if (peek (src, i, length) == '>')
                        {
                            tokens[token_count++]
                                = (Token){ TOKEN_RIGHT_SHIFT, ">>", line,
                                           column };
                            i++;
                            column += 2;
                        }
                    else
                        {
                            tokens[token_count++]
                                = (Token){ TOKEN_LESS, ">", line, column };
                            column++;
                        }
                    break;
                case '<':
                    if (peek (src, i, length) == '=')
                        {
                            tokens[token_count++]
                                = (Token){ TOKEN_LESS_EQUAL, "<=", line,
                                           column };
                            i++;
                            column += 2;
                        }
                    else if (peek (src, i, length) == '<')
                        {
                            tokens[token_count++]
                                = (Token){ TOKEN_LEFT_SHIFT, "<<", line,
                                           column };
                            i++;
                            column += 2;
                        }
                    else
                        {
                            tokens[token_count++]
                                = (Token){ TOKEN_LESS, "<", line, column };
                            column++;
                        }
                    break;
                case '=':
                    if (peek (src, i, length) == '=')
                        {
                            tokens[token_count++]
                                = (Token){ TOKEN_EQUAL_EQUAL, "==", line,
                                           column };
                            i++;
                            column += 2;
                        }
                    else
                        {
                            tokens[token_count++]
                                = (Token){ TOKEN_EQUALS, "=", line, column };
                            column++;
                        }
                    break;
                case '+':
                    if (peek (src, i, length) == '+')
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
                    if (peek (src, i, length) == '-')
                        {
                            tokens[token_count++]
                                = (Token){ TOKEN_SUBTRACT, "--", line,
                                           column };
                            i++;
                            column += 2;
                        }
                    else if (peek (src, i, length) == '>')
                        {
                            tokens[token_count++]
                                = (Token){ TOKEN_ARROW, "->", line, column };
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
                case '*':
                    tokens[token_count++]
                        = (Token){ TOKEN_STAR, "*", line, column };
                    column++;
                    break;
                case '/':
                    tokens[token_count++]
                        = (Token){ TOKEN_SLASH, "/", line, column };
                    column++;
                    break;
                case '!':
                    if (peek (src, i, length) == '=')
                        {
                            tokens[token_count++]
                                = (Token){ TOKEN_NOT_EQUAL, "!=", line,
                                           column };
                            i++;
                            column += 2;
                        }
                    else
                        {
                            tokens[token_count++]
                                = (Token){ TOKEN_NOT, "!", line, column };
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
                            else if (strcmp (lexeme, "getchar") == 0)
                                {
                                    tokens[token_count++]
                                        = (Token){ TOKEN_GETCHAR, lexeme, line,
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
                            else if (strcmp (lexeme, "break") == 0)
                                {
                                    tokens[token_count++]
                                        = (Token){ TOKEN_BREAK, lexeme, line,
                                                   startcolumn };
                                    break;
                                }
                            else if (strcmp (lexeme, "continue") == 0)
                                {
                                    tokens[token_count++]
                                        = (Token){ TOKEN_CONTINUE, lexeme,
                                                   line, startcolumn };
                                    break;
                                }
                            else if (strcmp (lexeme, "typedef") == 0)
                                {
                                    tokens[token_count++]
                                        = (Token){ TOKEN_TYPEDEF, lexeme, line,
                                                   startcolumn };
                                    break;
                                }
                            else if (strcmp (lexeme, "struct") == 0)
                                {
                                    tokens[token_count++]
                                        = (Token){ TOKEN_STRUCT, lexeme, line,
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

                            size_t leng = i - start + 1;
                            char *lexeme = malloc (leng + 1);
                            if (!lexeme)
                                {
                                    uerror ("Memory Error", line, column);
                                }
                            memcpy (lexeme, &src[start], leng);
                            lexeme[leng] = '\0';

                            tokens[token_count++]
                                = (Token){ TOKEN_NUMBER, lexeme, line,
                                           startcolumn };
                        }
                    if (c == '\'' || c == '"')
                        {
                            char startquote = c;
                            int startcolumn = column;
                            size_t capacity = 128;
                            size_t len = 0;

                            char *str = malloc (capacity);
                            if (!str)
                                uerror ("Memory Error", line, column);

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

                                    char ch = src[i];

                                    // handle escape sequences
                                    if (ch == '\\')
                                        {
                                            i++;
                                            column++;
                                            if (i >= length)
                                                {
                                                    free (str);
                                                    uerror ("Escape character "
                                                            "at end of input.",
                                                            line, column);
                                                }
                                            ch = escape (src[i]);
                                        }

                                    // resize if needed
                                    if (len + 1 >= capacity)
                                        {
                                            capacity *= 2;
                                            char *tmp
                                                = realloc (str, capacity);
                                            if (!tmp)
                                                {
                                                    free (str);
                                                    uerror ("Memory Error",
                                                            line, column);
                                                }
                                            str = tmp;
                                        }

                                    str[len++] = ch;
                                    i++;
                                    column++;
                                }

                            if (i >= length || src[i] != startquote)
                                {
                                    free (str);
                                    uerror (
                                        "Lexer: Couldn't find ending quote",
                                        line, column);
                                }
                            str[len] = '\0';
                            if (startquote == '"')
                                {
                                    tokens[token_count++]
                                        = (Token){ TOKEN_STRING, strdup (str),
                                                   line, startcolumn };
                                }
                            else
                                {
                                    if (len != 1)
                                        {
                                            uerror ("char byte too long! (or "
                                                    "it has nothing) use "
                                                    "arrays for "
                                                    "multi-character storing.",
                                                    line, column);
                                        }
                                    tokens[token_count++]
                                        = (Token){ TOKEN_CHAR, strdup (str),
                                                   line, startcolumn };
                                }
                            free (str);
                        }
                }
        }

    tokens[token_count++] = (Token){ TOKEN_EOF, "EOF", line, column };

    return tokens;
}
