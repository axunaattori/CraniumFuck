#include "lexer.h"
#include "util/error.h"
#include "util/helper.h"

#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "token.h"

char parse_escape(char *src, uint32_t length, uint32_t *i, int line,
                  int *column)
{
    uint8_t esc_size = 0;
    char esc[5] = {0};

    if (*i >= length)
    {
        ufatal("Escape character at end of input.", line, *column);
    }

    if (src[*i] == '\'' || src[*i] == '"')
    {
        return src[*i];
    }

    if (isdigit(src[*i]))
    {
        esc[esc_size++] = src[*i];
        (*i)++;
        (*column)++;
        while (*i < length && isdigit(src[*i]) && esc_size < 3 &&
               src[*i] != '\'' && src[*i] != '"')
        {
            esc[esc_size++] = src[*i];
            (*i)++;
            (*column)++;
        }
        return escape(esc);
    }
    else if (src[*i] == 'x')
    {
        esc[esc_size++] = src[*i];
        (*i)++;
        (*column)++;
        while (*i < length && isxdigit(src[*i]) && esc_size < 4 &&
               src[*i] != '\'' && src[*i] != '"')
        {
            esc[esc_size++] = src[*i];
            (*i)++;
            (*column)++;
        }
        return escape(esc);
    }
    else
    {
        esc[esc_size++] = src[*i];
        (*column)++;
        (*i)++;
        return escape(esc);
    }
}

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
                          {"while", TOKEN_WHILE},
                          {"if", TOKEN_IF},
                          {"void", TOKEN_VOID},
                          {"return", TOKEN_RETURN},
                          {"break", TOKEN_BREAK},
                          {"continue", TOKEN_CONTINUE},
                          {"typedef", TOKEN_TYPEDEF},
                          {"struct", TOKEN_STRUCT},
                          {"do", TOKEN_DO},
                          {"else", TOKEN_ELSE}};

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
            uint32_t startline = line;

            char *str = malloc(capacity);
            if (!str)
                ufatal("Memory Error", line, column);

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
                str[len++] = ch;
                i++;

                if (ch == '\\')
                {
                    ch = parse_escape(src, length, &i, line, &column);
                    str[len - 1] = ch; // We need to override it.
                }

                if (len + 1 >= capacity)
                {
                    capacity *= 2;
                    str = realloc(str, capacity);
                    if (!str)
                        ufatal("Memory error", line, column);
                }
            }

            if (i >= length || src[i] != startquote)
            {
                free(str);
                ufatal("Lexer: Couldn't find ending quote", startline,
                       startcolumn);
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
                           startline, startcolumn);
                }
                tokens[token_count++] =
                    (Token){TOKEN_CHAR, strdup(str), line, startcolumn};
            }
            free(str);
            matched = true;
        }

        if (c == '\\')
        {
            int startcolumn = column;
            char lexeme[2];
            lexeme[0] = parse_escape(src, length, &i, line, &column);
            lexeme[1] = '\0';
            tokens[token_count++] =
                (Token){TOKEN_NUMBER, strdup(lexeme), line, startcolumn};
            matched = true;
            i--;
            column--;
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
