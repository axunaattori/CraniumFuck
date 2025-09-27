#include "lexer.h"
#include "util/error.h"
#include "util/helper.h"

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

Token *lexer(char *src, uint32_t length,
             size_t *token_amount) // token_amount for parser
{
    typedef struct
    {
        const char *str;
        tokenType type;
        int length;
    } lex_operator;

    typedef struct
    {
        const char *name;
        tokenType type;
    } Keyword;
    lex_operator operators[] = {
        {"++", TOKEN_INCREMENT, 2},
        {"--", TOKEN_DECREMENT, 2},
        {"->", TOKEN_ARROW, 2},
        {"!=", TOKEN_NOT_EQUAL, 2},
        {"==", TOKEN_EQUAL_EQUAL, 2},
        {">=", TOKEN_GREATER_EQUAL, 2},
        {"<=", TOKEN_LESS_EQUAL, 2},
        {"<<", TOKEN_LEFT_SHIFT, 2},
        {">>", TOKEN_RIGHT_SHIFT, 2},
        {"&&", TOKEN_LOGIC_AND, 2},
        {"||", TOKEN_LOGIC_OR, 2},
        {"(", TOKEN_OPEN_PARENTHESIS, 1},
        {")", TOKEN_CLOSE_PARENTHESIS, 1},
        {"{", TOKEN_OPEN_BRACE, 1},
        {"}", TOKEN_CLOSE_BRACE, 1},
        {"[", TOKEN_OPEN_SQUARE_BRACE, 1},
        {"]", TOKEN_CLOSE_SQUARE_BRACE, 1},
        {";", TOKEN_SEMICOLON, 1},
        {",", TOKEN_COMMA, 1},
        {"%", TOKEN_MODULO, 1},
        {"~", TOKEN_BIT_NOT, 1},
        {"^", TOKEN_BIT_XOR, 1},
        {".", TOKEN_DOT, 1},
        {"*", TOKEN_STAR, 1},
        {"/", TOKEN_SLASH, 1},
        {"+", TOKEN_PLUS, 1},
        {"-", TOKEN_MINUS, 1},
        {"<", TOKEN_LESS, 1},
        {">", TOKEN_GREATER, 1},
        {"=", TOKEN_EQUALS, 1},
        {"!", TOKEN_NOT, 1},
        {"&", TOKEN_AMPERSAND, 1},
        {"|", TOKEN_BIT_OR, 1},
    };

    Keyword keywords[] = {{"byte", TOKEN_BYTE},
                          {"putchar", TOKEN_PUTCHAR},
                          {"getchar", TOKEN_GETCHAR},
                          {"while", TOKEN_WHILE},
                          {"if", TOKEN_IF},
                          {"void", TOKEN_VOID},
                          {"return", TOKEN_RETURN},
                          {"break", TOKEN_BREAK},
                          {"continue", TOKEN_CONTINUE},
                          {"typedef", TOKEN_TYPEDEF},
                          {"struct", TOKEN_STRUCT},
                          {"do", TOKEN_DO}};

    Token *tokens = malloc((length + 1) * sizeof(Token));
    if (!tokens)
        return NULL;

    uint32_t token_count = 0;
    int column = 1;
    int line = 1;

    for (uint32_t i = 0; i < length; i++)
    {
        char c = src[i];

        bool matched = false;
        for (size_t j = 0; j < sizeof(operators) / sizeof(operators[0]); j++)
        {
            lex_operator op = operators[j];
            if (i + op.length - 1 < length &&
                strncmp(&src[i], op.str, op.length) == 0)
            {
                tokens[token_count++] = (Token){op.type, op.str, line, column};
                i += op.length - 1;
                column += op.length;
                matched = true;
                break;
            }
        }

        uint32_t start = i;
        int startcolumn = column;
        if (c == '_' || isalpha(c)) // identifiers and keywords
        {
            while ((src[i]) == '_' || isalpha(src[i]))
            {
                i++;
                column++;
                if (i >= length)
                {
                    ufatal("Couldn't find ending.", line, column);
                }
            }

            i--; // adjust

            uint32_t leng = i - start + 1;
            char *lexeme = malloc(leng + 1);
            if (!lexeme)
            {
                ufatal("Memory Error", line, column);
            }
            strncpy(lexeme, &src[start], leng);
            lexeme[leng] = '\0';

            tokenType type = TOKEN_IDENTIFIER;
            for (uint8_t j = 0; j < sizeof(keywords) / sizeof(keywords[0]); j++)
            {
                if (strcmp(lexeme, keywords[j].name) == 0)
                {
                    type = keywords[j].type;
                    break;
                }
            }
            tokens[token_count++] = (Token){type, lexeme, line, startcolumn};
            matched = true;
        }
        if (isdigit(c))
        {
            while (isdigit(src[i]))
            {
                i++;
                column++;
                if (i >= length)
                {
                    ufatal("Couldn't find ending "
                           "(number).",
                           line, column);
                }
            }

            i--; // adjust

            size_t leng = i - start + 1;
            char *lexeme = malloc(leng + 1);
            if (!lexeme)
            {
                ufatal("Memory Error", line, column);
            }
            memcpy(lexeme, &src[start], leng);
            lexeme[leng] = '\0';

            tokens[token_count++] =
                (Token){TOKEN_NUMBER, lexeme, line, startcolumn};
            matched = true;
        }
        if (c == '\'' || c == '"')
        {
            char startquote = c;
            int startcolumn = column;
            size_t capacity = 128;
            size_t len = 0;

            char *str = malloc(capacity);
            if (!str)
                uerror("Memory Error", line, column);

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
                        free(str);
                        ufatal("Escape character "
                               "at end of input.",
                               line, column);
                    }
                    ch = escape(src[i]);
                }

                // resize if needed
                if (len + 1 >= capacity)
                {
                    capacity *= 2;
                    char *tmp = realloc(str, capacity);
                    if (!tmp)
                    {
                        free(str);
                        ufatal("Memory Error", line, column);
                    }
                    str = tmp;
                }

                str[len++] = ch;
                i++;
                column++;
                matched = true;
            }

            if (i >= length || src[i] != startquote)
            {
                free(str);
                ufatal("Lexer: Couldn't find ending quote", line, column);
            }
            str[len] = '\0';
            if (startquote == '"')
            {
                tokens[token_count++] =
                    (Token){TOKEN_STRING, strdup(str), line, startcolumn};
            }
            else
            {
                if (len != 1)
                {
                    uerror("char byte too long! (or "
                           "it has nothing) use "
                           "arrays for "
                           "multi-character storing.",
                           line, column);
                }
                tokens[token_count++] =
                    (Token){TOKEN_CHAR, strdup(str), line, startcolumn};
            }
            free(str);
            matched = true;
        }

        if (!matched)
        {
            if (isspace(c))
            {
                if (c == '\n')
                {
                    column = 1;
                    line++;
                }
                else
                {
                    column++;
                }
            }
            else
            {
                uerror("Lexer: Faulty operator.", line, column);
            }
        }
    }

    tokens[token_count++] = (Token){TOKEN_EOF, "EOF", line, column};

    *token_amount = (size_t)token_count;

    return tokens;
}
