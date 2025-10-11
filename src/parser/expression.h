#ifndef EXPRESSION_H
#define EXPRESSION_H
#include "lexer/token.h"
#include "parser/node.h"
#include "parser/parser.h"

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

operator get_op_enum(Token * token);

Node *parse_expression(Parser *p);

Node *parse_comma(Parser *p);

Node *parse_assignment(Parser *p);

Node *parse_logical_or(Parser *p);

Node *parse_logical_and(Parser *p);

Node *parse_bitwise_or(Parser *p);

Node *parse_bitwise_xor(Parser *p);

Node *parse_bitwise_and(Parser *p);

Node *parse_equal(Parser *p);

Node *parse_inequality(Parser *p);

Node *parse_bitwise_lr(Parser *p);

Node *parse_addition(Parser *p);

Node *parse_multiplication(Parser *p);

Node *parse_prefix(Parser *p);

Node *parse_postfix(Parser *p);

Node *parse_primary(Parser *p);

#endif
