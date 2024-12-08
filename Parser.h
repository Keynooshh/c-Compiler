#include "ds.h"
#include "Lexer.h"
#define PARSER

enum instr_kind{ 
    // keywords
    INSTR_ASSIGN,        
    INSTR_IF,         
    INSTR_GOTO,            
    INSTR_OUTPUT,     
    INSTR_LABEL,                         
};

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




struct assign_node{
    char *ident;
    struct expr_node *expr;
};


enum rel_kind{
    REL_LESS_THAN,
    REL_GREATER_THAN
};

struct rel_node{
    enum rel_kind kind;
    union{
        struct term_binary_node less_than;
        struct term_binary_node greater_than;
    };
};

enum term_kind{
    TERM_INPUT,
    TERM_IDENT,
    TERM_INT
};

struct term_node{
    enum term_kind kind;
    union 
    {
        char* value;
    };
    
};

struct if_node{
    struct rel_node rel;
    struct instr_node *instr;
};

struct goto_node
{
    char *label;
};

struct output_node
{
    struct term_node term;
};

struct label_node{
    char *label;
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

struct program_node{
    ds_dynamic_array instrs;
};


struct parser{
    ds_dynamic_array tokens;
    unsigned int index;
};

void parser_init(ds_dynamic_array tokens, struct parser* p ){
    p -> tokens = tokens;
    p -> index = 0;
}

void parser_current (struct parser* p , struct token *tok ){
    ds_dynamic_array_get(&p->tokens ,  p->index , tok);
}

void parser_advance(struct parser *p){
    p -> index ++ ;
}


void parse_assign(struct parser* p, struct instr_node * instr){
    struct token token;

    parser_current(p,&token);
    instr ->assign.ident = token.value;
    parser_advance(p);

    parser_current(p,&token);
    if(token.kind != TK_EQUAL){
        DS_PANIC("Expected Equal found %s",show_token_kind(token.kind));
    }
    parser_advance(p);
    parse_expr(p,&instr-> assign.expr);
}


void parse_if(struct parser* p, struct instr_node * instr){

}

void parse_goto(struct parser* p, struct instr_node * instr){

}

void parse_output(struct parser* p, struct instr_node * instr){

}

void parse_label(struct parser* p, struct instr_node * instr){

}

void parse_term(struct parser* p,struct term_node* term){
    struct token token;

    parser_current(p,&token);
    if (token.kind == TK_INPUT){
        term-> kind = TERM_INPUT;

    }else if (token.kind == TK_INT){
        term -> kind = TERM_INT;
        term -> value = token.value;
    }else if (token.kind == TK_IDENT){
        term  -> kind = TERM_IDENT;
        term -> value = token.value;
    } else {
        DS_PANIC("Unexpected term %d",show_token_kind(token.kind)); 
    }
    parser_advance(p);
}



void parse_expr(struct parser* p, struct expr_node* expr){
    struct token token;
    struct term_node lhs , rhs;
    parse_term(p,&lhs);

    parser_current(p , &token);
    if( token.kind == TK_PLUS) {
        parser_advance(p);
        parse_term(p,&rhs);
        
        expr->kind = EXPR_PLUS;
        expr->add.lhs = lhs;
        expr->add.rhs = rhs;
    }  else if( token.kind == TK_MINUS) {
        parser_advance(p);
        parse_term(p,&rhs);
        
        expr->kind = EXPR_MINUS;
        expr->min.lhs = lhs;
        expr->min.rhs = rhs;
    }  else if( token.kind == TK_DIVIDE) {
        parser_advance(p);
        parse_term(p,&rhs);
        
        expr->kind = EXPR_DIV;
        expr->div.lhs = lhs;
        expr->div.rhs = rhs;
    }  else if( token.kind == TK_MULTIPLY) {
        parser_advance(p);
        parse_term(p,&rhs);
        
        expr->kind = EXPR_PLUS;
        expr->mul.lhs = lhs;
        expr->mul.rhs = rhs;
    }  else{ 
        expr->kind = EXPR_TERM;
        expr->term = lhs;
    }       
     
}


void parse_program(struct parser* p, struct program_node* program){
    ds_dynamic_array_init(&program->instrs, sizeof(struct instr_node));
    struct token tok;
    do{
        struct instr_node *instr;
        parser_current(p, &tok);

    if (tok.kind == TK_IDENT) {
        parse_assign(p, instr);
    } else if (tok.kind == TK_IF) {
        parse_if(p, instr);
    } else if (tok.kind == TK_GOTO) {
        parse_goto(p, instr);
    } else if (tok.kind == TK_OUTPUT) {
        parse_output(p, instr);
    } else if (tok.kind == TK_LABLE) {
        parse_label(p, instr);
    } else {
        DS_PANIC("unexpected token %s", show_token_kind(tok.kind));
    }
        parser_advance(p);
        ds_dynamic_array_append(&program -> instrs , &instr);
    } while ( tok.kind != END);
}




void parse_program(ds_dynamic_array tokens, struct program_node *program){
    ds_dynamic_array_init(&program -> instrs , sizeof(struct instr_node));
}