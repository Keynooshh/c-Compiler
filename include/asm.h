#ifndef ASM_H
#define ASM_H

#include "../include/ds.h"
#include "../include/parser.h"

void print_term(struct term_node *term);
void term_asm(struct term_node *term, ds_dynamic_array *variables);
void expr_asm(struct expr_node *expr, ds_dynamic_array *variables);
void rel_asm(struct rel_node *rel, ds_dynamic_array *variables);
void instr_asm(struct instr_node *instr, ds_dynamic_array *variables,
               int *if_count);
void term_declare_variables(struct term_node *term, ds_dynamic_array *variables);
void expr_declare_variables(struct expr_node *expr,
                            ds_dynamic_array *variables);
void rel_declare_variables(struct rel_node *rel, ds_dynamic_array *variables);
void program_asm(struct program_node *program);
int find_variable(ds_dynamic_array *variables, char *ident);
#endif