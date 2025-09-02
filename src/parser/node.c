#include "node.h"
#include "util/error.h"
#include <stdlib.h>
#include <string.h>

node *
create_constant_node (uint8_t value, uint32_t line, uint32_t column)
{
    node *newNode = malloc (sizeof (node));
    if (!newNode)
        {
            uerror ("Failed to allocate memory for a constant node", line,
                    column);
        };
    *newNode = (node){ .column = column, .line = line, .constant = value };
    return newNode;
}

node *
create_identifier_node (const char *name, uint32_t line, uint32_t column)
{
    node *newNode = malloc (sizeof (node));
    if (!newNode)
        {
            uerror ("Failed to allocate memory for a identifier node", line,
                    column);
        };
    char *copy = strdup (name);
    if (!copy)
        {
            uerror ("Failed to copy name of identifier", line, column);
        }
    *newNode = (node){ .column = column, .line = line, .name = copy };
    return newNode;
}

node *
create_assign_node (const char *name, node *value, uint32_t line,
                    uint32_t column)
{
    node *newNode = malloc (sizeof (node));
    if (!newNode)
        {
            uerror ("Failed to allocate memory for an assign node", line,
                    column);
        };
    char *copy = strdup (name);
    if (!copy)
        {
            uerror ("Failed to copy name of identifier", line, column);
        }
    *newNode = (node){ .column = column,
                       .line = line,
                       .assign = { .name = copy, .value = value } };
    return newNode;
}

node *
create_binary_node (node *left, node *right, operator op, uint32_t line,
                    uint32_t column)
{
    node *newNode = malloc (sizeof (node));
    if (!newNode)
        {
            uerror ("Failed to allocate memory for a binary node", line,
                    column);
        };
    *newNode = (node){ .column = column,
                       .line = line,
                       .binary = { .left = left, .right = right, .op = op } };
    return newNode;
}
