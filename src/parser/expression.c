#include "parser/expression.h"
#include "lexer/token.h"
#include "parser/node.h"
#include "parser/parser.h"
#include "util/error.h"

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

operator get_op_enum(Token *token)
{
    tokenType token_type = token->type;
    switch (token_type)
    {
    case TOKEN_COMMA:
        return OP_COMMA;
    case TOKEN_EQUALS:
        return OP_ASSIGN;
    default:
        uerror("Source code is messed up, Somehow called a non operator into "
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
    Node *left = NULL;

    if (match_eat(p, TOKEN_EQUALS))
    {
        Token *operator = previous_token(p);
        Node *right = parse_assignment(p);

        left = create_binary_node(left, right, get_op_enum(operator),
                                  operator->line, operator->column);
    }

    return left;
}
