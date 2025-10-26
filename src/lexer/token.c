#include "token.h"

#define CASE(token)                                                            \
    case token:                                                                \
        return #token;

const char *token_type_to_string(tokenType type)
{
    switch (type)
    {
        // separator
        CASE(TOKEN_OPEN_PARENTHESIS)
        CASE(TOKEN_CLOSE_PARENTHESIS)
        CASE(TOKEN_SEMICOLON)
        CASE(TOKEN_OPEN_BRACE)
        CASE(TOKEN_CLOSE_BRACE)
        CASE(TOKEN_OPEN_SQUARE_BRACE)
        CASE(TOKEN_CLOSE_SQUARE_BRACE)
        CASE(TOKEN_COMMA)

        // math related and operators
        CASE(TOKEN_INCREMENT) //++
        CASE(TOKEN_DECREMENT) //--
        CASE(TOKEN_EQUALS)
        CASE(TOKEN_PLUS)
        CASE(TOKEN_MINUS)
        CASE(TOKEN_STAR)          // * multiplation and pointer
        CASE(TOKEN_SLASH)         // /
        CASE(TOKEN_EQUAL_EQUAL)   // ==
        CASE(TOKEN_NOT_EQUAL)     // !=
        CASE(TOKEN_GREATER)       // >
        CASE(TOKEN_LESS)          // <
        CASE(TOKEN_LESS_EQUAL)    // <=
        CASE(TOKEN_GREATER_EQUAL) // >=
        CASE(TOKEN_NOT)           // !
        CASE(TOKEN_MODULO)        // %
        CASE(TOKEN_AMPERSAND)     // & address of and bit and

        CASE(TOKEN_LOGIC_AND)
        CASE(TOKEN_LOGIC_OR)

        // member access operators
        CASE(TOKEN_DOT)   // .
        CASE(TOKEN_ARROW) // ->
        CASE(TOKEN_SCOPE) // ::

        // bitwise
        CASE(TOKEN_BIT_NOT)     // ~
        CASE(TOKEN_BIT_OR)      // |
        CASE(TOKEN_BIT_XOR)     // ^
        CASE(TOKEN_LEFT_SHIFT)  // <<
        CASE(TOKEN_RIGHT_SHIFT) // >>

        // keywords
        CASE(TOKEN_BYTE)
        CASE(TOKEN_WHILE)
        CASE(TOKEN_VOID)
        CASE(TOKEN_RETURN)
        CASE(TOKEN_IF)
        CASE(TOKEN_ELSE)
        CASE(TOKEN_DO)
        CASE(TOKEN_BREAK)
        CASE(TOKEN_CONTINUE)
        CASE(TOKEN_TYPEDEF)
        CASE(TOKEN_STRUCT)
        CASE(TOKEN_NAMESPACE)

        // literals
        CASE(TOKEN_NUMBER)
        CASE(TOKEN_IDENTIFIER)
        CASE(TOKEN_STRING) // ""
        CASE(TOKEN_CHAR)   // ''

        // extra
        CASE(TOKEN_EOF) // end of file

    default:
        return "UNKNOWN";
    }
}
