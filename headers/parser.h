#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ds.h"

// Term kinds
enum term_kind{
    TERM_INPUT,
    TERM_IDENT,
    TERM_INT
};

// Term node structure
struct term_node{
    enum term_kind kind;
    union {
        char* value;
    };
};

enum expr_kind{
    EXPR_TERM,
    EXPR_PLUS,
    EXPR_MINUS,
    EXPR_MUL,
    EXPR_DIV
};


struct term_binary_node
{
    struct term_node lhs;
    struct term_node rhs;
};

// Expression node structure
struct expr_node{
    enum expr_kind kind;
    union{
        struct term_binary_node add;
        struct term_binary_node min;
        struct term_binary_node mul;
        struct term_binary_node div;
        struct term_node term;
    };
};


// Goto node structure
struct goto_node
{
    char *label;
};

// Output node structure
struct output_node
{
    struct term_node term;
};

// Label node structure
struct label_node{
    char *label;
};

// Expression kinds


// Term binary node structure


enum instr_kind{ 
    INSTR_ASSIGN,        
    INSTR_IF,         
    INSTR_GOTO,            
    INSTR_OUTPUT,     
    INSTR_LABEL,                         
};


struct assign_node{
    char *ident;
    struct expr_node expr;
};

// Relational kinds
enum rel_kind{
    REL_LESS_THAN,
    REL_GREATER_THAN
};

// Relational node structure
struct rel_node{
    enum rel_kind kind;
    union{
        struct term_binary_node less_than;
        struct term_binary_node greater_than;
    };
};

struct if_node{
    struct rel_node rel;
    struct instr_node *instr;
};

// Instruction node structure
struct instr_node{
    enum instr_kind kind;
    union{
        struct assign_node assign;
        struct if_node if_term;
        struct goto_node goto_term;
        struct output_node output;
        struct label_node label;
    };
};

// Program node structure
struct program_node{
    ds_dynamic_array instrs;
};

// Parser structure
struct parser{
    ds_dynamic_array tokens;
    unsigned int index;
};

// Parser functions
void parser_init(ds_dynamic_array tokens, struct parser* p );
void parser_current(struct parser* p , struct token *tok );
void parser_advance(struct parser *p);

// Parse functions
void parse_assign(struct parser* p, struct instr_node * instr);
void parse_instr(struct parser *p, struct instr_node *instr);
void parse_rel(struct parser *p , struct rel_node *rel);
void parse_if(struct parser* p, struct instr_node * instr);
void parse_goto(struct parser* p, struct instr_node * instr);
void parse_output(struct parser* p, struct instr_node * instr);
void parse_label(struct parser* p, struct instr_node * instr);
void parse_term(struct parser* p, struct term_node* term);
void parse_expr(struct parser* p, struct expr_node* expr);
void parse_program(struct parser *p, struct program_node *program);

#endif // PARSER_H
