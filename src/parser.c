#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/ds.h"
#define PARSER



// Parser initialization
void parser_init(ds_dynamic_array tokens, struct parser *p) {
    p->tokens = tokens;
    p->index = 0;
    
}

// Get the current token
void parser_current(struct parser *p, struct token *tok) {
    // if (p->index >= ds_dynamic_array_size(&p->tokens)) {
    //     DS_PANIC("Out of bounds access: index %d", p->index);
    // }
    ds_dynamic_array_get(&p->tokens, p->index, tok);
}

// Advance to the next token
void parser_advance(struct parser *p) {
    p->index++;
}

// Parse assignment instruction
void parse_assign(struct parser *p, struct instr_node *instr) {
    struct token token;

    instr->kind = INSTR_ASSIGN;
    
    parser_current(p, &token);
    instr->assign.ident = token.value;
    parser_advance(p);

    parser_current(p, &token);
    if (token.kind != TK_EQUAL) {
        DS_PANIC("Expected Equal found %s", show_token_kind(token.kind));
    }
    parser_advance(p);
       parser_current(p, &token);
       print_token(token);
    parse_expr(p, &instr->assign.expr);
}

// Parse instruction (could be assign, if, goto, output, or label)
void parse_instr(struct parser *p, struct instr_node *instr) {
    struct token token;

    parser_current(p, &token);

    if (token.kind == TK_IDENT) {
        parse_assign(p, instr);
    } else if (token.kind == TK_IF) {
        parse_if(p, instr);
    } else if (token.kind == TK_GOTO) {
        parse_goto(p, instr);
    } else if (token.kind == TK_OUTPUT) {
        parse_output(p, instr);
    } else if (token.kind == TK_LABEL) {
        parse_label(p, instr);
    } else {
        DS_PANIC("unexpected token %s", show_token_kind(token.kind));
    }
}

// Parse relational expression (less than or greater than)
void parse_rel(struct parser *p, struct rel_node *rel) {
    struct token token;
    struct term_node lhs, rhs;

    parse_term(p, &lhs);

    parser_current(p, &token);
    if (token.kind == TK_LTHAN) {
        parser_advance(p);
        parse_term(p, &rhs);

        rel->kind = REL_LESS_THAN;
        rel->less_than.lhs = lhs;
        rel->less_than.rhs = rhs;
    } else if (token.kind == TK_GTHAN) {
        parser_advance(p);
        parse_term(p, &rhs);

        rel->kind = REL_GREATER_THAN;
        rel->greater_than.lhs = lhs;
        rel->greater_than.rhs = rhs;
    } else {
        DS_PANIC("Expected Less Than `<` found %s", show_token_kind(token.kind));
    }
}

// Parse if statement
void parse_if(struct parser *p, struct instr_node *instr) {
    struct token token;

    instr->kind = INSTR_IF;
    parser_advance(p);

    parse_rel(p, &instr->if_term.rel);

    parser_current(p, &token);
    if (token.kind != TK_THEN) {
        DS_PANIC("Expected Then found %s", show_token_kind(token.kind));
    }
    parser_advance(p);

    instr->if_term.instr = malloc(sizeof(struct instr_node));
    
    parse_instr(p, instr->if_term.instr);
}

// Parse goto instruction
void parse_goto(struct parser *p, struct instr_node *instr) {
    struct token token;
    instr->kind = INSTR_GOTO;
    parser_advance(p);

    parser_current(p, &token);
    if (token.kind != TK_LABEL) {
        DS_PANIC("Expected Label found %s", show_token_kind(token.kind));
    }
    parser_advance(p);
    instr->goto_term.label = token.value;
}

// Parse output instruction
void parse_output(struct parser *p, struct instr_node *instr) {
    struct token token;
    struct term_node lhs;

    instr->kind = INSTR_OUTPUT;
    parser_advance(p);

    parse_term(p, &lhs);

    instr->output.term = lhs;
}

// Parse label instruction
void parse_label(struct parser *p, struct instr_node *instr) {
    struct token token;

    instr->kind = INSTR_LABEL;

    parser_current(p, &token);
    instr->label.label = token.value;

    parser_advance(p);
}

// Parse term (input, integer, or identifier)
void parse_term(struct parser *p, struct term_node *term) {
    struct token token;

    parser_current(p, &token);
    if (token.kind == TK_INPUT) {
        term->kind = TERM_INPUT;
    } else if (token.kind == TK_INT) {
        term->kind = TERM_INT;
        term->value = token.value;
    } else if (token.kind == TK_IDENT) {
        term->kind = TERM_IDENT;
        term->value = token.value;
    } else {
        DS_PANIC("Unexpected term %d", show_token_kind(token.kind));
    }
    parser_advance(p);
}

// Parse expression (addition, subtraction, multiplication, division, or term)
void parse_expr(struct parser *p, struct expr_node *expr) {
    struct token token;
    struct term_node lhs, rhs;

    parse_term(p, &lhs);

    parser_current(p, &token);

    if (token.kind == TK_PLUS) {
        parser_advance(p);
        parse_term(p, &rhs);

        expr->kind = EXPR_PLUS;
        expr->add.lhs = lhs;
        expr->add.rhs = rhs;
    } 
     else if (token.kind == TK_MINUS) {
         parser_advance(p);
         parse_term(p, &rhs);


         expr->kind = EXPR_MINUS;
         expr->min.lhs = lhs;
         expr->min.rhs = rhs;
     } else if (token.kind == TK_DIVIDE) {
         parser_advance(p);
         parse_term(p, &rhs);

         expr->kind = EXPR_DIV;
         expr->div.lhs = lhs;
         expr->div.rhs = rhs;
     } else if (token.kind == TK_MULTIPLY) {
         parser_advance(p);
         parse_term(p, &rhs);
 

         expr->kind = EXPR_MUL;
         expr->mul.lhs = lhs;
         expr->mul.rhs = rhs;
     }
     else {
        expr->kind = EXPR_TERM;
        expr->term = lhs;
    }
}

// Parse the entire program
void parse_program(struct parser *p, struct program_node *program) {
    ds_dynamic_array_init(&program->instrs, sizeof(struct instr_node));

    struct token token;
    do {
        struct instr_node instr;

        parse_instr(p, &instr);

        ds_dynamic_array_append(&program->instrs, &instr);

        parser_current(p, &token);

    } while (token.kind != END);
}
