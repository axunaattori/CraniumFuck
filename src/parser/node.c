#include "node.h"
#include "util/error.h"
#include <stdlib.h>
#include <string.h>

Node *
create_constant_node (uint8_t value, uint32_t line, uint32_t column)
{
    Node *new_node = malloc (sizeof (Node));
    if (!new_node)
        {
            uerror ("Failed to allocate memory for a constant Node", line,
                    column);
        };
    *new_node = (Node){ .column = column, .line = line, .constant = value };
    return new_node;
}

Node *
create_identifier_node (const char *name, uint32_t line, uint32_t column)
{
    Node *new_node = malloc (sizeof (Node));
    if (!new_node)
        {
            uerror ("Failed to allocate memory for a identifier Node", line,
                    column);
        };
    char *copy = strdup (name);
    if (!copy)
        {
            uerror ("Failed to copy name of identifier", line, column);
        }
    *new_node = (Node){ .column = column, .line = line, .name = copy };
    return new_node;
}

Node *
create_assign_node (const char *name, Node *value, uint32_t line,
                    uint32_t column)
{
    Node *new_node = malloc (sizeof (Node));
    if (!new_node)
        {
            uerror ("Failed to allocate memory for an assign Node", line,
                    column);
        };
    char *copy = strdup (name);
    if (!copy)
        {
            uerror ("Failed to copy name of identifier", line, column);
        }
    *new_node = (Node){ .column = column,
                        .line = line,
                        .assign = { .name = copy, .value = value } };
    return new_node;
}

Node *
create_binary_node (Node *left, Node *right, operator op, uint32_t line,
                    uint32_t column)
{
    Node *new_node = malloc (sizeof (Node));
    if (!new_node)
        {
            uerror ("Failed to allocate memory for a binary Node", line,
                    column);
        };
    *new_node = (Node){ .column = column,
                        .line = line,
                        .binary = { .left = left, .right = right, .op = op } };
    return new_node;
}
