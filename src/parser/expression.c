#include "parser/expression.h"
#include "lexer/token.h"
#include "parser/node.h"
#include "parser/parser.h"
#include "util/error.h"
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

/* follow this order (comment also exists in node.h)
 * https://en.cppreference.com/w/c/language/operator_precedence.html
 * 1: () . X++ X-- -> []                       L -> R
 * 2: ++X --X !X ~X *X (derefernce) &X R -> L
 * 3: * / %                                    L -> R
 * 4: + -
 * 5: << >>
 * 6: < <= > >=
 * 7: == !=
 * 8: &
 * 9: ^
 * 10: |
 * 11: &&
 * 12: ||
 * The language wont support 13 (?:) for now
 * 14: = (and those += -= if im not too lazy) R -> L
 * 15: ,    L -> R
 */

// L -> R while
// R -> L if

member_type get_mem_type_enum(Token *token)
{
    tokenType token_type = token->type;
    switch (token_type)
    {
    case TOKEN_DOT:
        return MEM_DOT;
    case TOKEN_ARROW:
        return MEM_ARROW;
    default:
        uerror("how did you get something else than . or -> here? please "
               "report this as a issue, this should be "
               "impossible.",
               token->line, token->column);
        return MEM_NONEXISTENT;
    }
}

operator get_op_enum_postfix(Token *token)
{
    tokenType token_type = token->type;
    switch (token_type)
    {
    case TOKEN_INCREMENT:
        return OP_POST_INC;
    case TOKEN_DECREMENT:
        return OP_POST_DEC;
    default:
        uerror("Source code is messed up, Somehow called a non operator "
               "into "
               "get_op_enum_postfix\n"
               "Please report this as it most likely isnt a problem that "
               "you've caused somehow.",
               token->line, token->column);
        return OP_NONEXISTENT; // yeah i copy and pasted this, what are you
                               // gonna do about it?
    }
}

operator get_op_enum_prefix(Token *token)
{
    tokenType token_type = token->type;
    switch (token_type)
    {
    case TOKEN_AMPERSAND:
        return OP_ADDRESS;
    case TOKEN_STAR:
        return OP_DEFERENCE;
    case TOKEN_NOT:
        return OP_LOGIC_NOT;
    case TOKEN_BIT_NOT:
        return OP_BIT_NOT;
    case TOKEN_INCREMENT:
        return OP_PRE_INC;
    case TOKEN_DECREMENT:
        return OP_PRE_DEC;
    default:
        uerror("Source code is messed up, Somehow called a non operator "
               "into "
               "get_op_enum_prefix\n"
               "Please report this as it most likely isnt a problem that "
               "you've caused somehow.",
               token->line, token->column);
        return OP_NONEXISTENT; // yeah i copy and pasted this, what are you
                               // gonna do about it?
    }
}

operator get_op_enum(Token *token)
{
    tokenType token_type = token->type;
    switch (token_type)
    {
    case TOKEN_COMMA:
        return OP_COMMA; // ,
    case TOKEN_EQUALS:
        return OP_ASSIGN; // =
    case TOKEN_LOGIC_OR:
        return OP_LOGIC_OR; // ||
    case TOKEN_LOGIC_AND:
        return OP_LOGIC_AND; // &&
    case TOKEN_BIT_OR:
        return OP_BIT_OR; // |
    case TOKEN_BIT_XOR:
        return OP_BIT_XOR; // ^
    case TOKEN_AMPERSAND:
        return OP_BIT_AND; // &
    case TOKEN_EQUAL_EQUAL:
        return OP_EQUAL; // ==
    case TOKEN_NOT_EQUAL:
        return OP_NOT_EQUAL; // !=
    case TOKEN_GREATER:
        return OP_GREATER; // >
    case TOKEN_GREATER_EQUAL:
        return OP_GREATER_EQUAL; // >=
    case TOKEN_LESS:
        return OP_LESS; // <
    case TOKEN_LESS_EQUAL:
        return OP_LESS_EQUAL; // <=
    case TOKEN_LEFT_SHIFT:
        return OP_LEFT_SHIFT; // <<
    case TOKEN_RIGHT_SHIFT:
        return OP_RIGHT_SHIFT; // >>
    case TOKEN_PLUS:
        return OP_ADD; // +
    case TOKEN_MINUS:
        return OP_SUB; // -
    case TOKEN_STAR:
        return OP_MULTI; // *
    case TOKEN_SLASH:
        return OP_DIVIDE; // /
    case TOKEN_MODULO:
        return OP_MODULO; // %
    default:
        uerror("Source code is messed up, Somehow called a non operator "
               "into "
               "get_op_enum\n"
               "Please report this as it most likely isnt a problem that "
               "you've caused somehow.",
               token->line, token->column);
        return OP_NONEXISTENT; // please use this for operators
    }
}

Node *parse_expression(Parser *p) { return parse_comma(p); }

Node *parse_comma(Parser *p)
{
    Node *left = parse_assignment(p);

    while (match_eat(p, TOKEN_COMMA))
    {
        Token *operator = previous_token(p);
        Node *right = parse_assignment(p);

        left = create_binary_node(left, right, get_op_enum(operator),
                                  operator->line, operator->column);
    }

    return left;
}

Node *parse_assignment(Parser *p)
{
    Node *left = parse_logical_or(p);

    if (match_eat(p, TOKEN_EQUALS))
    {
        Token *operator = previous_token(p);
        Node *right = parse_assignment(p);

        left = create_binary_node(left, right, get_op_enum(operator),
                                  operator->line, operator->column);
    }

    return left;
}

Node *parse_logical_or(Parser *p)
{
    Node *left = parse_logical_and(p);

    while (match_eat(p, TOKEN_LOGIC_OR))
    {
        Token *operator = previous_token(p);
        Node *right = parse_logical_and(p);

        left = create_binary_node(left, right, get_op_enum(operator),
                                  operator->line, operator->column);
    }

    return left;
}

Node *parse_logical_and(Parser *p)
{
    Node *left = parse_bitwise_or(p);

    while (match_eat(p, TOKEN_LOGIC_AND))
    {
        Token *operator = previous_token(p);
        Node *right = parse_bitwise_or(p);

        left = create_binary_node(left, right, get_op_enum(operator),
                                  operator->line, operator->column);
    }

    return left;
}

Node *parse_bitwise_or(Parser *p)
{
    Node *left = parse_bitwise_xor(p);

    while (match_eat(p, TOKEN_BIT_OR))
    {
        Token *operator = previous_token(p);
        Node *right = parse_bitwise_xor(p);

        left = create_binary_node(left, right, get_op_enum(operator),
                                  operator->line, operator->column);
    }

    return left;
}

Node *parse_bitwise_xor(Parser *p)
{
    Node *left = parse_bitwise_and(p);

    while (match_eat(p, TOKEN_BIT_XOR))
    {
        Token *operator = previous_token(p);
        Node *right = parse_bitwise_and(p);

        left = create_binary_node(left, right, get_op_enum(operator),
                                  operator->line, operator->column);
    }

    return left;
}

Node *parse_bitwise_and(Parser *p)
{
    Node *left = parse_equal(p);

    while (match_eat(p, TOKEN_AMPERSAND))
    {
        Token *operator = previous_token(p);
        Node *right = parse_equal(p);

        left = create_binary_node(left, right, get_op_enum(operator),
                                  operator->line, operator->column);
    }

    return left;
}

Node *parse_equal(Parser *p)
{
    Node *left = parse_inequality(p);

    while (match_eat(p, TOKEN_EQUAL_EQUAL) || match_eat(p, TOKEN_NOT_EQUAL))
    {
        Token *operator = previous_token(p);
        Node *right = parse_inequality(p);

        left = create_binary_node(left, right, get_op_enum(operator),
                                  operator->line, operator->column);
    }

    return left;
}

Node *parse_inequality(Parser *p)
{
    Node *left = parse_bitwise_shift(p);

    while (match_eat(p, TOKEN_GREATER) || match_eat(p, TOKEN_LESS) ||
           match_eat(p, TOKEN_GREATER_EQUAL) || match_eat(p, TOKEN_LESS_EQUAL))
    {
        Token *operator = previous_token(p);
        Node *right = parse_bitwise_shift(p);

        left = create_binary_node(left, right, get_op_enum(operator),
                                  operator->line, operator->column);
    }

    return left;
}

Node *parse_bitwise_shift(Parser *p)
{
    Node *left = parse_addition(p);

    while (match_eat(p, TOKEN_LEFT_SHIFT) || match_eat(p, TOKEN_RIGHT_SHIFT))
    {
        Token *operator = previous_token(p);
        Node *right = parse_addition(p);

        left = create_binary_node(left, right, get_op_enum(operator),
                                  operator->line, operator->column);
    }

    return left;
}

Node *parse_addition(Parser *p)
{
    Node *left = parse_multiplication(p);

    while (match_eat(p, TOKEN_PLUS) || match_eat(p, TOKEN_MINUS))
    {
        Token *operator = previous_token(p);
        Node *right = parse_multiplication(p);

        left = create_binary_node(left, right, get_op_enum(operator),
                                  operator->line, operator->column);
    }

    return left;
}

Node *parse_multiplication(Parser *p)
{
    Node *left = parse_prefix(p);

    while (match_eat(p, TOKEN_STAR) || match_eat(p, TOKEN_SLASH) ||
           match_eat(p, TOKEN_MODULO))
    {
        Token *operator = previous_token(p);
        Node *right = parse_prefix(p);

        left = create_binary_node(left, right, get_op_enum(operator),
                                  operator->line, operator->column);
    }

    return left;
}

Node *parse_prefix(Parser *p)
{
    if (match_eat(p, TOKEN_AMPERSAND) || match_eat(p, TOKEN_STAR) ||
        match_eat(p, TOKEN_NOT) || match_eat(p, TOKEN_BIT_NOT) ||
        match_eat(p, TOKEN_INCREMENT) || match_eat(p, TOKEN_DECREMENT))
    {
        Token *operator = previous_token(p);
        Node *operand = parse_prefix(p);

        return create_unary_node(operand, get_op_enum_prefix(operator),
                                 operator->line, operator->column);
    }

    return parse_postfix(p);
}

Node *parse_postfix(Parser *p)
{
    Node *left = parse_primary(p);

    while (true)
    {
        if (match_eat(p, TOKEN_INCREMENT) || match_eat(p, TOKEN_DECREMENT))
        {
            Token *operator = previous_token(p);
            left = create_unary_node(left, get_op_enum_postfix(operator),
                                     operator->line, operator->column);
        }
        else if (match_eat(p, TOKEN_DOT) || match_eat(p, TOKEN_ARROW))
        {
            Token *operator = previous_token(p);

            if (!match(p, TOKEN_IDENTIFIER))
                uerror("expected identifier after -> or .", operator->line,
                       operator->column);

            Token *current = current_token(p);
            eat(p, TOKEN_IDENTIFIER);

            left = create_member_node(left, current->lexeme,
                                      get_mem_type_enum(operator),
                                      operator->line, operator->column);
        }
        else if (match_eat(p, TOKEN_OPEN_SQUARE_BRACE))
        {
            Token *operator = previous_token(p);
            Node *index_expression = parse_expression(p);

            if (!match_eat(p, TOKEN_CLOSE_SQUARE_BRACE))
                uerror("expected ']'", operator->line, operator->column);

            left = create_index_node(left, index_expression, operator->line,
                                     operator->column);
        }
        else
        {
            break;
        }
    }
    return left;
}

Node *parse_primary(Parser *p)
{
    if (match_eat(p, TOKEN_NUMBER))
    {
        Token *tok = previous_token(p);
        char *endptr;
        errno = 0;

        unsigned long temp = strtoul(tok->lexeme, &endptr, 10);

        if (errno != 0 || temp > UINT8_MAX)
            uwarning("Number doesn't fit into the value! overflowing!",
                     tok->line, tok->column);

        if (*endptr != '\0')
        {
            uerror(
                "number contained invalid characters? i dont think this is "
                "supposed to even be possible so report this if you see this",
                tok->line, tok->column);
        }

        uint8_t number = (uint8_t)temp;

        return create_constant_node(number, tok->line, tok->column);
    }

    if (match_eat(p, TOKEN_IDENTIFIER))
    {
        Token *tok = previous_token(p);

        return create_identifier_node(tok->lexeme, tok->line, tok->column);
    }
    if (match_eat(p, TOKEN_OPEN_PARENTHESIS))
    {
        Node *expression = parse_expression(p);
        if (!match_eat(p, TOKEN_CLOSE_PARENTHESIS))
            uerror("expected ')", expression->line, expression->column);

        return expression;
    }

    Token *tok = current_token(p);
    uerror("unexpected token in parse_primary", tok->line, tok->column);
    return NULL;
}
