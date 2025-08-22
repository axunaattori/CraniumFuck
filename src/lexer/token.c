#include "token.h"

const char *
tokenTypeToString (tokenType type)
{
    switch (type)
        {
        case TOKEN_OPEN_PARENTHESIS:
            return "TOKEN_OPEN_PARENTHESIS";
        case TOKEN_CLOSE_PARENTHESIS:
            return "TOKEN_CLOSE_PARENTHESIS";
        case TOKEN_SEMICOLON:
            return "TOKEN_SEMICOLON";
        case TOKEN_OPEN_BRACE:
            return "TOKEN_OPEN_BRACE";
        case TOKEN_CLOSE_BRACE:
            return "TOKEN_CLOSE_BRACE";
        case TOKEN_OPEN_SQUARE_BRACE:
            return "TOKEN_OPEN_SQUARE_BRACE";
        case TOKEN_CLOSED_SQUARE_BRACE:
            return "TOKEN_CLOSED_SQUARE_BRACE";
        case TOKEN_COMMA:
            return "TOKEN_COMMA";
        case TOKEN_ADD:
            return "TOKEN_ADD";
        case TOKEN_SUBTRACT:
            return "TOKEN_SUBTRACT";
        case TOKEN_EQUALS:
            return "TOKEN_EQUALS";
        case TOKEN_PLUS:
            return "TOKEN_PLUS";
        case TOKEN_MINUS:
            return "TOKEN_MINUS";
        case TOKEN_BYTE:
            return "TOKEN_BYTE";
        case TOKEN_PUTCHAR:
            return "TOKEN_PUTCHAR";
        case TOKEN_WHILE:
            return "TOKEN_WHILE";
        case TOKEN_IF:
            return "TOKEN_IF";
        case TOKEN_VOID:
            return "TOKEN_VOID";
        case TOKEN_RETURN:
            return "TOKEN_RETURN";
        case TOKEN_NUMBER:
            return "TOKEN_NUMBER";
        case TOKEN_IDENTIFIER:
            return "TOKEN_IDENTIFIER";
        case TOKEN_STRING:
            return "TOKEN_STRING";
        case TOKEN_EOF:
            return "TOKEN_EOF";
        case TOKEN_MAIN:
            return "TOKEN_MAIN";
        default:
            return "UNKNOWN";
        }
}
