#include "lexer.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

char escape(char c) {
    switch (c) {
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

Token *lexer(char *src, uint32_t length) {
    Token *tokens = malloc((length + 1) * sizeof(Token));
    if (!tokens) return NULL;

    uint32_t token_count = 0;
    int column = 1;
    int line = 1;

    for (uint32_t i = 0; i < length; i++) {
        char c = src[i];
        switch (c) {
            case '(':
                tokens[token_count++] =
                    (Token){TOKEN_OPEN_PARENTHESIS, "(", line, column};
                break;
            case ')':
                tokens[token_count++] =
                    (Token){TOKEN_CLOSE_PARENTHESIS, ")", line, column};
                break;
            case '{':
                tokens[token_count++] =
                    (Token){TOKEN_OPEN_BRACE, "{", line, column};
                break;
            case '}':
                tokens[token_count++] =
                    (Token){TOKEN_OPEN_BRACE, "}", line, column};
                break;
            case ';':
                tokens[token_count++] =
                    (Token){TOKEN_SEMICOLON, ";", line, column};
                break;
            case '=':
                tokens[token_count++] =
                    (Token){TOKEN_EQUALS, "=", line, column};
                break;
            case '+':
                if (src[i + 1] == '+') {
                    tokens[token_count++] =
                        (Token){TOKEN_ADD, "++", line, column};
                    i++;
                } else {
                    tokens[token_count++] =
                        (Token){TOKEN_PLUS, "+", line, column};
                }
                break;
            case '-':
                if (src[i + 1] == '-') {
                    tokens[token_count++] =
                        (Token){TOKEN_SUBTRACT, "--", line, column};
                    i++;
                } else {
                    tokens[token_count++] =
                        (Token){TOKEN_MINUS, "-", line, column};
                }
                break;
            case '\n':
                column = 1;
                line++;
                break;
            default:
                if (isspace(c)) break;

                if (c == '_' ||
                    isalpha(c))  // get the word if there even is a word.
                {
                    uint32_t start = i;
                    int startcolumn = column;
                    while (src[i] == '_' || isalpha(src[i]) ||
                           isdigit(src[i])) {
                        i++;
                        column++;
                        if (i >= length) {
                            printf(
                                "Couldn't find ending. problematic code starts "
                                "at column: %d, line: %d\n",
                                startcolumn, line);
                            printf(
                                "This most likely isn't your code's problem, "
                                "as this should be impossible to my "
                                "knowledge.\n");
                            return NULL;
                        }
                    }

                    uint32_t leng = i - start;
                    char *lexeme = malloc(leng + 1);
                    if (!lexeme) {
                        printf("memory error\n");
                        return NULL;
                    }
                    strncpy(lexeme, &src[start], leng);
                    lexeme[leng] = '\0';

                    if (strcmp(lexeme, "byte") == 0) {
                        tokens[token_count++] =
                            (Token){TOKEN_BYTE, lexeme, line, column};
                        break;
                    } else if (strcmp(lexeme, "putchar") == 0) {
                        tokens[token_count++] =
                            (Token){TOKEN_PUTCHAR, lexeme, line, column};
                        break;
                    } else if (strcmp(lexeme, "while") == 0) {
                        tokens[token_count++] =
                            (Token){TOKEN_WHILE, lexeme, line, column};
                        break;
                    } else {  // byte A, where A is the indentifier
                        tokens[token_count++] =
                            (Token){TOKEN_IDENTIFIER, lexeme, line, column};
                        break;
                    }

                } else if (isdigit(c))  // TOKEN_NUMBER
                {
                    uint32_t start = i;
                    int startcolumn = column;
                    while (isdigit(src[i])) {
                        i++;
                        column++;

                        if (i >= length) {
                            printf(
                                "Couldn't find ending (number), problematic "
                                "code starts at column: %d, line: %d\n",
                                startcolumn, line);
                            printf(
                                "This most likely isn't your code's problem, "
                                "as this should be impossible to my "
                                "knowledge.\n");
                            return NULL;
                        }
                    }

                    uint32_t leng = i - start;
                    char *lexeme = malloc(leng + 1);
                    if (!lexeme) {
                        printf("memory error\n");
                        return NULL;
                    }
                    strncpy(lexeme, &src[start], leng);
                    lexeme[leng] = '\0';

                    tokens[token_count++] =
                        (Token){TOKEN_NUMBER, lexeme, line, column};
                    break;
                }

                else if (c == '\'' || c == '\"')  // TOKEN_STRING
                {
                    int startcolumn = column;
                    char startquote = c;

                    size_t lex_i = 0;
                    size_t bufsize = 128;
                    char *lexeme = malloc(bufsize);
                    if (!lexeme)  // i have to redo this fucking thing for
                                  // escape support :sob:
                    {
                        printf("memory error\n");
                        return NULL;
                    }

                    i++;
                    column++;  // get past the first "
                    while (src[i] != startquote) {
                        if (src[i] == '\\') {
                            i++;
                            column++;
                            if (i >= length)  // if it goes on infinitely, gotta
                                              // be safe after adding string
                                              // support after all.
                            {
                                printf(
                                    "Escape character had nothing. problematic "
                                    "code starts at column: %d, line: %d\n",
                                    startcolumn, line);
                                free(lexeme);
                                return NULL;
                            }
                            char retchar = escape(src[i]);
                            lexeme[lex_i++] = retchar;
                        } else {
                            lexeme[lex_i++] = src[i];
                        }

                        if (i >=
                            length)  // if it goes on infinitely, gotta be safe
                                     // after adding string support after all.
                        {
                            printf(
                                "Couldn't find ending to string (missing %c). "
                                "problematic code starts at column: %d, line: "
                                "%d\n",
                                startquote, startcolumn, line);
                            return NULL;
                        }

                        i++;
                        column++;

                        if (lex_i >= bufsize - 1)  // resize
                        {
                            bufsize *= 2;
                            char *temp = realloc(lexeme, bufsize);
                            if (!temp) {
                                printf("memory error\n");
                                free(lexeme);
                                return NULL;
                            }
                            lexeme = temp;
                        }
                    }
                    i++;
                    column++;  // remember to add the final " too.

                    tokens[token_count++] =
                        (Token){TOKEN_STRING, lexeme, line, column};
                    break;
                }
                break;
        }
        column++;
    }

    tokens[token_count++] = (Token){TOKEN_EOF, "", line, column};

    return tokens;
}
