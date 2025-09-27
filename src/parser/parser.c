#include "parser.h"
#include "lexer/token.h"
#include "parser/node.h"
#include "util/error.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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
            // do something man
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

        return create_var_dec_node(identifier, NULL, init, start->line,
                                   start->column);
    }
    else if (p->tokens[p->pos].type == TOKEN_VOID)
    {
        Token *start = current_token(p);
        eat(p, TOKEN_VOID);
        const char *identifier = p->tokens[p->pos].lexeme;
        eat(p, TOKEN_IDENTIFIER);
        eat(p, TOKEN_OPEN_PARENTHESIS);
        // who the fuck know how i pasre this :D
        eat(p, TOKEN_CLOSE_PARENTHESIS);
        eat(p, TOKEN_OPEN_BRACE);
        // also, how the fuck?
        eat(p, TOKEN_CLOSE_BRACE);
    }

    return NULL;
}
