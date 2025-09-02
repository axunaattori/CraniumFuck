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
    TOKEN_CLOSE_SQUARE_BRACE,
    TOKEN_COMMA,
    // math related and operators
    TOKEN_ADD,      //++
    TOKEN_SUBTRACT, //--
    TOKEN_EQUALS,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,          // * multiplation and pointer
    TOKEN_SLASH,         // /
    TOKEN_EQUAL_EQUAL,   // ==
    TOKEN_NOT_EQUAL,     // !=
    TOKEN_GREATER,       // >
    TOKEN_LESS,          // <
    TOKEN_LESS_EQUAL,    // <=
    TOKEN_GREATER_EQUAL, // >=
    TOKEN_NOT,           // !
    TOKEN_MODULO,        // %
    TOKEN_AMPERSAND,     // & address of and bit and

    // logic
    TOKEN_LOGIC_AND, // &&
    TOKEN_LOGIC_OR,  // ||

    // member access operators
    TOKEN_DOT,   // .
    TOKEN_ARROW, // ->

    // bitwise
    TOKEN_BIT_NOT,     // ~
    TOKEN_BIT_OR,      // |
    TOKEN_BIT_XOR,     // ^
    TOKEN_LEFT_SHIFT,  // <<
    TOKEN_RIGHT_SHIFT, // >>

    // keywords
    TOKEN_BYTE,
    TOKEN_PUTCHAR,
    TOKEN_GETCHAR,
    TOKEN_WHILE,
    TOKEN_VOID,
    TOKEN_RETURN,
    TOKEN_IF,
    TOKEN_ELSE,

    TOKEN_BREAK,
    TOKEN_CONTINUE,

    TOKEN_TYPEDEF,
    TOKEN_STRUCT,
    // literals
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_STRING, // ""
    TOKEN_CHAR,   // ''

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

const char *token_type_to_string (tokenType type);

#endif
