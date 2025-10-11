#include "parsing.h"
#include "lexer/token.h"
#include "node.h"
#include "parser.h"
#include "parser/expression.h"
#include "util/error.h"
#include <stdlib.h>

// TODO: important!! get condition

Node *parse_condition(Parser *p)
{
    eat(p, TOKEN_OPEN_PARENTHESIS);
    Node *condition = parse_expression(p);
    eat(p, TOKEN_CLOSE_PARENTHESIS);
    return condition;
}

/* follow this order (comment also exists in node.h)
 * 1: () . X++ X-- -> []                L -> R
 * 2: ++X --X !X ~X *X (derefernce) &X  R -> L
 * 3: * / %                             L -> R
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

Node *parse_block(Parser *p)
{
    Token *start = current_token(p);
    eat(p, TOKEN_OPEN_BRACE);

    Node **statements = NULL;
    size_t count = 0;

    while (current_token(p)->type != TOKEN_CLOSE_BRACE &&
           current_token(p)->type != TOKEN_EOF)
    {
        Node *statement = parse(p);
        if (statement)
        {

            Node **temp = realloc(statements, sizeof(Node *) * (count + 1));
            if (!temp)
                ufatal("Parser: Failed to realloc memory", statement->line,
                       statement->column);
            statements = temp;
            statements[count++] = statement;
        }
    }

    if (match(p, TOKEN_EOF))
    {
        ufatal("You failed to add a '}', it has reached the EOF token, "
               "which is really really bad",
               current_token(p)->line, current_token(p)->column);
    }
    eat(p, TOKEN_CLOSE_BRACE);

    Node *block =
        create_block_node(statements, count, start->line, start->column);
    return block;
}

Node **parse_parameters(Parser *p, size_t *size)
{
    size_t count = 0;
    Node **params = NULL;
    eat(p, TOKEN_OPEN_PARENTHESIS);

    if (current_token(p)->type == TOKEN_VOID)
    {
        eat(p, TOKEN_VOID);
    }
    else
    {
        while (current_token(p)->type != TOKEN_CLOSE_PARENTHESIS &&
               current_token(p)->type != TOKEN_EOF)
        {
            eat(p,
                TOKEN_BYTE); // TODO: expand to support typedef when it is added

            Token *identifier = current_token(p);
            eat(p, TOKEN_IDENTIFIER);

            Node *param =
                create_var_dec_node(identifier->lexeme, NULL, NULL,
                                    identifier->line, identifier->column);

            Node **temp = realloc(params, sizeof(Node *) * (count + 1));
            if (!temp)
                ufatal("Parser: Failed to realloc memory", identifier->line,
                       identifier->column);
            params = temp;
            params[count++] = param;

            if (match(p, TOKEN_COMMA))
            {
                eat(p, TOKEN_COMMA);
            }
            else
            {
                break;
            }
        }
    }

    if (match(p, TOKEN_EOF))
    {
        ufatal("You failed to add a ')', it has reached the EOF token, "
               "which is really really bad",
               current_token(p)->line, current_token(p)->column);
    }

    eat(p, TOKEN_CLOSE_PARENTHESIS);

    *size = count;
    return params;
}

Node *parse_token_void(Parser *p)
{
    Token *start = current_token(p);
    eat(p, TOKEN_VOID);
    const char *identifier = p->tokens[p->pos].lexeme;
    eat(p, TOKEN_IDENTIFIER);
    size_t size = 0;
    Node **parameters = parse_parameters(p, &size);
    Node *block = parse_block(p);
    return create_function_node(
        identifier, create_type_node("void", start->line, start->column),
        parameters, size, block, start->line, start->column);
}

Node *parse_token_byte(Parser *p)
{
    Token *start = current_token(p);
    Node *type = create_type_node("byte", start->line, start->column);
    eat(p, TOKEN_BYTE);
    const char *identifier = p->tokens[p->pos].lexeme;
    eat(p, TOKEN_IDENTIFIER);

    if (match(p, TOKEN_OPEN_PARENTHESIS))
    // it just happens to be a function instead of a variable
    {
        size_t size = 0;
        Node **parameters = parse_parameters(p, &size);
        Node *block = parse_block(p);
        return create_function_node(identifier, type, parameters, size, block,
                                    start->line, start->column);
    }

    Node *init = NULL;
    if (match(p, TOKEN_EQUALS))
    {
        eat(p, TOKEN_EQUALS);
        if (match(p, TOKEN_NUMBER))
        {
            Token *num = current_token(p);
            init = create_constant_node((uint8_t)atoi(num->lexeme), num->line,
                                        num->column);
        }
        eat_err(p, TOKEN_NUMBER, "You need to put an number after the =");
    }
    eat(p, TOKEN_SEMICOLON);

    return create_var_dec_node(identifier, type, init, start->line,
                               start->column);
}

Node *parse_token_while(Parser *p)
{
    Token *start = current_token(p);
    eat(p, TOKEN_WHILE);

    Node *block = parse_block(p);
    return create_while_node(NULL, block, start->line, start->column);
}
