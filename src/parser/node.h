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
    OP_LOGIC_AND,     // &&
    OP_LOGIC_OR,      // ||

    // unary
    OP_LOGIC_NOT, // !variable
    OP_BIT_NOT,   // ~variable
    OP_INC,       //++
    OP_DEC,       //--
    OP_ADDRESS,   // &variable
    OP_DEFERENCE  // *variable
} operator;

/*
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

typedef struct node
{
    uint32_t column;
    uint32_t line;
    union
    {
        uint8_t constant;
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
            struct node *type; // unused for now.
            struct node *init;
        } varDec;

        struct
        {
            struct node **statements;
            size_t size;
        } block;

        struct
        {
            struct node *condition;
            struct node *body;
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

node *create_constant_node (uint8_t value, uint32_t line, uint32_t column);
node *create_identifier_node (const char *name, uint32_t line,
                              uint32_t column);
node *create_assign_node (const char *name, node *value, uint32_t line,
                          uint32_t column);
node *create_binary_node (node *left, node *right, operator op, uint32_t line,
                          uint32_t column);
node *create_unary_node (node *operand, operator op, uint32_t line,
                         uint32_t column);
node *create_varDec_node (const char *name, node *type, node *init,
                          uint32_t line, uint32_t column);
// type is unuseable for now. but will be in the function for future me. so i
// dont have to replace it everywhere
node *create_block_node (node **statements, size_t size, uint32_t line,
                         uint32_t column);
node *create_while_node (node *condition, node *body, uint32_t line,
                         uint32_t column);
node *create_function_node (const char *name, node **arguments, size_t size,
                            node *body, uint32_t line, uint32_t column);

#endif
