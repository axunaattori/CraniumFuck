#ifndef TOKEN_H
#define TOKEN_H

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
    TOKEN_COMMA,
    // math related and operators
    TOKEN_ADD,      //++
    TOKEN_SUBTRACT, //--
    TOKEN_EQUALS,
    TOKEN_PLUS,
    TOKEN_MINUS,
    // keywords
    TOKEN_BYTE,
    TOKEN_PUTCHAR,
    TOKEN_WHILE,
    TOKEN_VOID,
    TOKEN_RETURN,
    TOKEN_IF,
    // literals
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    // extra
    TOKEN_EOF, // end of file
    TOKEN_MAIN // start, like in c.
} tokenType;

typedef struct
{
    tokenType type;
    char *lexeme;
    int line;
    int column;
} Token;

const char *tokenTypeToString (tokenType type);

#endif
