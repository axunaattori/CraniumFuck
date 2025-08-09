#pragma once

typedef enum
{
    // separator
    TOKEN_OPEN_PARENTHESIS,
    TOKEN_CLOSE_PARENTHESIS,
    TOKEN_SEMICOLON,
    TOKEN_OPEN_BRACE,
    TOKEN_CLOSE_BRACE,
    TOKEN_OPEN_SQUARE_BRACE,
    TOKEN_CLOSED_SQUARE_BRACE,
    // math related and operators
    TOKEN_ADD,      //++
    TOKEN_SUBTRACT, //--
    TOKEN_EQUALS,
    TOKEN_PLUS,
    TOKEN_MINUS,
    // keywords
    TOKEN_BYTE,
    TOKEN_ARRAY,
    TOKEN_PUTCHAR,
    TOKEN_WHILE,
    // literals
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    // extra
    TOKEN_EOF // end of file
} tokenType;

typedef struct
{
    tokenType type;
    char *lexeme;
    int line;
    int column;
} Token;

const char *tokenTypeToString (tokenType type);
