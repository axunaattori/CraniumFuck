#include "parser.h"
#include "lexer/token.h"
#include "parser/node.h"
#include "util/error.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if PRINTDEBUG
void print_node(Node *node, int indent)
{
    if (!node)
        return;

    for (int i = 0; i < indent; i++)
        putchar(' ');

    switch (node->type)
    {
    case NODE_VARDEC:
        printf("VAR_DEC (%s): %s", node->var_dec.type->name,
               node->var_dec.name);
        if (node->var_dec.init)
            printf(" = %d", node->var_dec.init->constant);
        putchar('\n');
        break;

    case NODE_CONSTANT:
        printf("CONSTANT: %d\n", node->constant);
        break;

    case NODE_FUNCTION:
        printf("FUNCTION: %s, return type = %s, params = %zu\n",
               node->function.name, node->function.return_type->name,
               node->function.size);
        for (size_t i = 0; i < node->function.size; i++)
            print_node(node->function.arguments[i], indent + 1);
        print_node(node->function.body, indent + 1);
        break;

    case NODE_BLOCK:
        printf("BLOCK: %zu statements\n", node->block.size);
        for (size_t i = 0; i < node->block.size; i++)
            print_node(node->block.statements[i], indent + 1);
        break;

    default:
        printf("Unknown node type\n");
        break;
    }
}
#endif

void eat(Parser *p, tokenType expect)
{
    if (p->pos >= p->token_count)
    {
        ufatal("Parser: pos is somehow bigger than token_count", -1, -1);
    }
    if (p->tokens[p->pos].type == expect)
    {
        p->pos++;
    }
    else
    {
        char buffer[128];
        snprintf(buffer, sizeof(buffer), "Expected %s but got %s",
                 token_type_to_string(expect),
                 token_type_to_string(p->tokens[p->pos].type));
        uerror(buffer, p->tokens[p->pos].line, p->tokens[p->pos].column);
    }
}

Token *current_token(Parser *p)
{
    if (p->pos >= p->token_count)
    {
        ufatal("Parser: pos is somehow bigger than token_count", -1, -1);
    }
    return &p->tokens[p->pos];
}

Parser *create_parser(Token *tokens, size_t token_count)
{
    if (!tokens || token_count == 0)
        ufatal("token_count cannot be 0 or nonexistent!", -1, -1);
    Parser *p = malloc(sizeof(Parser));
    if (!p)
        ufatal("Failed to allocate memory for parser!", -1, -1);
    p->tokens = tokens;
    p->pos = 0;
    p->token_count = token_count;

    return p;
}

Node *parse(Parser *p)
{
    if (p->tokens[p->pos].type == TOKEN_BYTE)
    {
        Token *start = current_token(p);
        eat(p, TOKEN_BYTE);
        const char *identifier = p->tokens[p->pos].lexeme;
        eat(p, TOKEN_IDENTIFIER);

        if (current_token(p)->type == TOKEN_OPEN_PARENTHESIS)
        // it just happens to be a function instead of a variable
        {
            size_t size = 0;
            Node **parameters = parse_parameters(p, &size);
            Node *block = parse_block(p);
            return create_function_node(
                identifier,
                create_type_node("byte", start->line, start->column),
                parameters, size, block, start->line, start->column);
        }

        Node *init = NULL;
        if (current_token(p)->type == TOKEN_EQUALS)
        {
            eat(p, TOKEN_EQUALS);
            Token *num = current_token(p);
            init = create_constant_node((uint8_t)atoi(num->lexeme), num->line,
                                        num->column);
            eat(p, TOKEN_NUMBER);
        }
        eat(p, TOKEN_SEMICOLON);

        return create_var_dec_node(
            identifier, create_type_node("byte", start->line, start->column),
            init, start->line, start->column);
    }
    else if (p->tokens[p->pos].type == TOKEN_VOID)
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

    char buffer[256];
    snprintf(buffer, sizeof(buffer),
             "Invalid (or unsupported, it takes time to add everything you "
             "know!) Token, Letting the parser go forward: %s (%s)",
             token_type_to_string(current_token(p)->type),
             current_token(p)->lexeme);
    uerror(buffer, current_token(p)->line, current_token(p)->column);
    p->pos++;
    return NULL;
}

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

    if (current_token(p)->type == TOKEN_EOF)
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

            if (current_token(p)->type == TOKEN_COMMA)
            {
                eat(p, TOKEN_COMMA);
            }
            else
            {
                break;
            }
        }
    }

    if (current_token(p)->type == TOKEN_EOF)
    {
        ufatal("You failed to add a ')', it has reached the EOF token, "
               "which is really really bad",
               current_token(p)->line, current_token(p)->column);
    }

    eat(p, TOKEN_CLOSE_PARENTHESIS);

    *size = count;
    return params;
}

Node **parsing_loop(Parser *p, size_t *amount)
{
    size_t count = 0;
    Node **nodes = NULL;

    while (current_token(p)->type != TOKEN_EOF && p->pos < p->token_count)
    {
        Node *node = parse(p);
        if (!node)
            break;

        Node **temp = realloc(nodes, sizeof(Node *) * (count + 1));
        if (!temp)
            ufatal("Parser: Failed to realloc memory", node->line,
                   node->column);
        nodes = temp;
        nodes[count++] = node;
    }

    *amount = count;
    return nodes;
}
