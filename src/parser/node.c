#include "node.h"
#include "util/error.h"
#include <stdlib.h>
#include <string.h>

Node *create_constant_node(uint8_t value, uint32_t line, uint32_t column)
{
    Node *new_node = malloc(sizeof(Node));
    if (!new_node)
    {
        ufatal("Failed to allocate memory for a constant Node", line, column);
    };
    *new_node = (Node){.type = NODE_CONSTANT,
                       .column = column,
                       .line = line,
                       .constant = value};
    return new_node;
}

Node *create_identifier_node(const char *name, uint32_t line, uint32_t column)
{
    Node *new_node = malloc(sizeof(Node));
    if (!new_node)
    {
        ufatal("Failed to allocate memory for a identifier Node", line, column);
    };
    char *copy = strdup(name);
    if (!copy)
    {
        ufatal("Failed to copy name of identifier", line, column);
    }
    *new_node = (Node){
        .type = NODE_IDENTIFIER, .column = column, .line = line, .name = copy};
    return new_node;
}

Node *create_assign_node(const char *name, Node *value, uint32_t line,
                         uint32_t column)
{
    Node *new_node = malloc(sizeof(Node));
    if (!new_node)
    {
        ufatal("Failed to allocate memory for an assign Node", line, column);
    };
    char *copy = strdup(name);
    if (!copy)
    {
        ufatal("Failed to copy name of identifier", line, column);
    }
    *new_node = (Node){.type = NODE_ASSIGNMENT,
                       .column = column,
                       .line = line,
                       .assign = {.name = copy, .value = value}};
    return new_node;
}

Node *create_binary_node(Node *left, Node *right, operator op, uint32_t line,
                         uint32_t column)
{
    Node *new_node = malloc(sizeof(Node));
    if (!new_node)
    {
        ufatal("Failed to allocate memory for a binary Node", line, column);
    };
    *new_node = (Node){.type = NODE_BINARY,
                       .column = column,
                       .line = line,
                       .binary = {.left = left, .right = right, .op = op}};
    return new_node;
}

Node *create_unary_node(Node *operand, operator op, uint32_t line,
                        uint32_t column)
{
    Node *new_node = malloc(sizeof(Node));
    if (!new_node)
    {
        ufatal("Failed to allocate memory for a unary Node", line, column);
    };
    *new_node = (Node){.type = NODE_UNARY,
                       .column = column,
                       .line = line,
                       .unary = {.operand = operand, .op = op}};
    return new_node;
}

Node *create_index_node(Node *base, Node *index, uint32_t line, uint32_t column)
{
    Node *new_node = malloc(sizeof(Node));
    if (!new_node)
    {
        ufatal("Failed to allocate memory for a unary Node", line, column);
    };
    *new_node = (Node){.type = NODE_INDEX,
                       .column = column,
                       .line = line,
                       .index = {.base = base, .index = index}};
    return new_node;
}

Node *create_member_node(struct Node *base, const char *name, member_type type,
                         uint32_t line, uint32_t column)
{
    Node *new_node = malloc(sizeof(Node));
    if (!new_node)
    {
        ufatal("Failed to allocate memory for a unary Node", line, column);
    };
    char *copy = strdup(name);
    if (!copy)
    {
        ufatal("Failed to copy name of identifier", line, column);
    }
    *new_node = (Node){.type = NODE_MEMBER,
                       .column = column,
                       .line = line,
                       .member = {.base = base, .name = copy, .type = type}};
    return new_node;
}

Node *create_var_dec_node(const char *name, Node *type, Node *init,
                          uint32_t line, uint32_t column)
{
    Node *new_node = malloc(sizeof(Node));
    if (!new_node)
    {
        ufatal("Failed to allocate memory for variable declartion Node", line,
               column);
    };
    char *copy = strdup(name);
    if (!copy)
    {
        ufatal("Failed to copy name of variable", line, column);
    }
    *new_node = (Node){.type = NODE_VARDEC,
                       .column = column,
                       .line = line,
                       .var_dec = {.name = copy, .type = type, .init = init}};
    return new_node;
}

Node *create_block_node(Node **statements, size_t size, uint32_t line,
                        uint32_t column)
{
    Node *new_node = malloc(sizeof(Node));
    if (!new_node)
    {
        ufatal("Failed to allocate memory for a block Node", line, column);
    };
    *new_node = (Node){.type = NODE_BLOCK,
                       .column = column,
                       .line = line,
                       .block = {.statements = statements, .size = size}};
    return new_node;
}

Node *create_while_node(Node *condition, Node *body, uint32_t line,
                        uint32_t column)
{
    Node *new_node = malloc(sizeof(Node));
    if (!new_node)
    {
        ufatal("Failed to allocate memory for a while Node", line, column);
    };
    *new_node = (Node){.type = NODE_WHILE,
                       .column = column,
                       .line = line,
                       .while_node = {.condition = condition, .body = body}};
    return new_node;
}

Node *create_function_node(const char *name, Node *type, Node **arguments,
                           size_t size, Node *body, uint32_t line,
                           uint32_t column)
{
    Node *new_node = malloc(sizeof(Node));
    if (!new_node)
    {
        ufatal("Failed to allocate memory for function Node", line, column);
    };
    char *copy = strdup(name);
    if (!copy)
    {
        ufatal("Failed to copy name of function", line, column);
    }
    *new_node = (Node){.type = NODE_FUNCTION,
                       .column = column,
                       .line = line,
                       .function = {.name = copy,
                                    .arguments = arguments,
                                    .size = size,
                                    .body = body,
                                    .return_type = type}};
    return new_node;
}

Node *create_type_node(const char *name, uint32_t line, uint32_t column)
{
    Node *new_node = malloc(sizeof(Node));
    if (!new_node)
    {
        ufatal("Failed to allocate memory for type Node", line, column);
    };
    char *copy = strdup(name);
    if (!copy)
    {
        ufatal("Failed to copy name of type", line, column);
    }

    *new_node =
        (Node){.type = NODE_TYPE, .column = column, .line = line, .name = name};
    return new_node;
}
