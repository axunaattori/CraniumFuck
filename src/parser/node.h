#ifndef NODE_H
#define NODE_H
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

typedef enum // currently has the nodes required for the fibonacci test
{
    NODE_IDENTIFIER,
    NODE_CONSTANT,

    NODE_ASSIGNMENT,
    NODE_BINARY, // + - /
    NODE_UNARY,  // -- ++
    NODE_VARDEC, // variable declartion
    NODE_INDEX,  // [variable]
    NODE_MEMBER, // . ->

    NODE_BLOCK,
    NODE_WHILE,
    NODE_FUNCTION,
    NODE_TYPE,
    NODE_CALL
} node_type;

typedef enum
{
    MEM_DOT,
    MEM_ARROW,
    MEM_NONEXISTENT
} member_type;

typedef enum
{
    // binary
    OP_COMMA,         // ,
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
    OP_ASSIGN,        // =
    OP_EQUAL,         // ==
    OP_NOT_EQUAL,     // !=
    OP_LOGIC_AND,     // &&
    OP_LOGIC_OR,      // ||
    OP_LEFT_SHIFT,    // <<
    OP_RIGHT_SHIFT,   // >>

    // unary
    OP_LOGIC_NOT, // !variable
    OP_BIT_NOT,   // ~variable
    OP_POST_INC,  // variable++
    OP_POST_DEC,  // variable--
    OP_PRE_INC,   // ++varaible
    OP_PRE_DEC,   // --variable
    OP_ADDRESS,   // &variable
    OP_DEFERENCE, // *variable

    OP_NONEXISTENT // just to make the function in expression.c happy
} operator;

/* https://en.cppreference.com/w/c/language/operator_precedence.html
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

typedef struct Node
{
    node_type type;
    uint32_t column;
    uint32_t line;
    union
    {
        uint8_t constant;
        const char *name; // identifier

        struct
        {
            const char *name;
            struct Node *value;
        } assign;

        struct
        {
            struct Node *left;
            struct Node *right;
            operator op;
        } binary;

        struct
        {
            struct Node *operand;
            operator op;
        } unary;

        struct
        {
            struct Node *base;
            struct Node *index;
        } index;

        struct
        {
            struct Node *base;
            const char *name;
            member_type type;
        } member;

        struct
        {
            const char *name;
            struct Node *type; // unused for now.
            struct Node *init;
        } var_dec;

        struct
        {
            struct Node **statements;
            size_t size;
        } block;

        struct
        {
            struct Node *condition;
            struct Node *body;
        } while_node;

        struct
        {
            const char *name;
            struct Node **arguments;
            size_t size;
            struct Node *body;
            struct Node *return_type;
        } function;

        struct
        {
            const char *name;
            struct Node **arguments;
            size_t size;
        } call;
    };
} Node;

Node *create_constant_node(uint8_t value, uint32_t line, uint32_t column);
Node *create_identifier_node(const char *name, uint32_t line, uint32_t column);
Node *create_assign_node(const char *name, Node *value, uint32_t line,
                         uint32_t column);
Node *create_binary_node(Node *left, Node *right, operator op, uint32_t line,
                         uint32_t column);
Node *create_unary_node(Node *operand, operator op, uint32_t line,
                        uint32_t column);
Node *create_index_node(Node *base, Node *index, uint32_t line,
                        uint32_t column);
Node *create_member_node(struct Node *base, const char *name, member_type type,
                         uint32_t line, uint32_t column);
Node *create_var_dec_node(const char *name, Node *type, Node *init,
                          uint32_t line, uint32_t column);
// type is unuseable for now. but will be in the function for future me. so i
// dont have to replace it everywhere
Node *create_block_node(Node **statements, size_t size, uint32_t line,
                        uint32_t column);
Node *create_while_node(Node *condition, Node *body, uint32_t line,
                        uint32_t column);
Node *create_function_node(const char *name, Node *type, Node **arguments,
                           size_t size, Node *body, uint32_t line,
                           uint32_t column);
Node *create_type_node(const char *name, uint32_t line, uint32_t column);
Node *create_call_node(const char *name, Node **args, size_t size,
                       uint32_t line, uint32_t column);

#endif
