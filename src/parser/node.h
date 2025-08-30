#ifndef NODE_H
#define NODE_H
#include <stddef.h>
#include <stdint.h>

typedef enum // currently has the nodes required for the fibonacci test
{
    NODE_IDENTIFIER,
    NODE_CONSTANT,

    NODE_ASSIGNMENT,
    NODE_BINARY, // + - /
    NODE_UNARY,  // -- ++
    NODE_VARDEC, // variable declartion

    NODE_BLOCK,
    NODE_WHILE,
    NODE_FUNCTION
} nodeType;

typedef enum
{
    // binary
    OP_ADD,           // +
    OP_SUB,           // -
    OP_MULTI,         // *
    OP_DIVIDE,        // /
    OP_MODULO,        // %
    OP_LESS,          // <
    OP_GREATER,       // >
    OP_LESS_EQUAL,    // <=
    OP_GREATER_EQUAL, // >=
    OP_BIT_AND,       // &
    OP_BIT_OR,        // |
    OP_BIT_XOR,       // ^
    OP_EQUAL,         // ==
    OP_NOT_EQUAL,     // !=

    // unary
    OP_LOGIC_NOT, // !variable
    OP_INC,       //++
    OP_DEC,       //--
    OP_ADDRESS    // &variable
} operator;

typedef struct node
{
    uint32_t column;
    uint32_t line;
    union
    {
        int constant;
        const char *name; // identifier

        struct
        {
            const char *name;
            struct node *value;
        } assign;

        struct
        {
            struct node *left;
            struct node *right;
            operator op;
        } binary;

        struct
        {
            struct node *operand;
            operator op; //++ for +, -- for -, others have 1 char
        } unary;

        struct
        {
            const char *name;
            // struct node *type; ill most likely end up adding typedef support
            struct node *init;
        } varDec;

        struct
        {
            struct node **statements;
            size_t size;
        } block;

        struct
        {
            struct node *body;
            struct node *condition;
        } whileNode;

        struct
        {
            const char *name;
            struct node **arguments;
            size_t size;
            struct node *body;
        } function;
    };
} node;

#endif
