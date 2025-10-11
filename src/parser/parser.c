#include "parser.h"
#include "lexer/token.h"
#include "parser/node.h"
#include "parser/parsing.h"
#include "util/error.h"
#include <stdbool.h>
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

void eat_err(Parser *p, tokenType expect, char *error_msg)
{
    if (current_token(p)->type != expect)
    {
        uerror(error_msg, current_token(p)->line, current_token(p)->column);
    }
    else
    {
        eat(p, expect);
    }
}

bool match(Parser *p, tokenType expect)
{
    if (current_token(p)->type != expect)
        return false;
    return true;
}

bool match_eat(Parser *p, tokenType expect)
{
    if (match(p, expect))
    {
        eat(p, expect);
        return true;
    }
    return false;
}

Token *current_token(Parser *p)
{
    if (p->pos >= p->token_count)
    {
        ufatal("Parser: pos is somehow bigger than token_count", -1, -1);
    }
    return &p->tokens[p->pos];
}

Token *previous_token(Parser *p)
{
    if (p->pos >= p->token_count)
    {
        ufatal("Parser: pos is somehow bigger than token_count", -1, -1);
    }
    return &p->tokens[p->pos - 1];
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

Node *parse(Parser *p)
{
    if (p->tokens[p->pos].type == TOKEN_BYTE)
    {
        return parse_token_byte(p);
    }
    else if (p->tokens[p->pos].type == TOKEN_VOID)
    {

        return parse_token_void(p);
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
