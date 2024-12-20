#include "../include/ds.h"
#include "../include/parser.h"
#include "../include/asm.h"
#define ASM

int find_variable(ds_dynamic_array *variables, char *ident)
{
    for (unsigned int i = 0; i < variables->count; i++)
    {
        char *variable = NULL;
        ds_dynamic_array_get(variables, i, &variable);

        if (strcmp(ident, variable) == 0)
        {
            return i;
        }
    }

    return -1;
}

void term_asm(struct term_node *term, ds_dynamic_array *variables)
{
    switch (term->kind)
    {
    case TERM_INPUT:
    {
        printf("    mov rdi, 1\n");
        printf("    mov rsi, input_label\n");
        printf("    mov rdx, 15\n");
        printf("    mov rax, 1\n");
        printf("    syscall\n");

        printf("    read 0, line, LINE_MAX\n");
        printf("    mov rdi, line\n");
        printf("    call strlen\n");
        printf("    mov rdi, line\n");
        printf("    mov rsi, rax\n");
        printf("    call parse_uint\n");
        break;
    }
    case TERM_INT:
        printf("    mov rax, %s\n", term->value);
        break;
    case TERM_IDENT:
        // changed
        {
            if (strcmp(term->value, ",") == 0)
            {
                // Handle comma output
                printf("    mov rdi, 1\n");
                printf("    mov rsi, comma\n"); // Assume `comma` is a predefined label for ","
                printf("    mov rdx, 1\n");
                printf("    mov rax, 1\n");
                printf("    syscall\n");
            }
            else
            {
                int index = find_variable(variables, term->value);
                printf("    mov rax, qword [rbp - %d]\n", index * 8 + 8);
            }
            break;
        }
        // {
        //     int index = find_variable(variables, term->value);
        //     printf("    mov rax, qword [rbp - %d]\n", index * 8 + 8);
        //     break;
        // }
    }
}

void expr_asm(struct expr_node *expr, ds_dynamic_array *variables)
{
    switch (expr->kind)
    {
    case EXPR_TERM:
    {
        term_asm(&expr->term, variables);
        break;
    }
    case EXPR_PLUS:
    {
        term_asm(&expr->add.lhs, variables);
        printf("    mov rdx, rax\n");
        term_asm(&expr->add.rhs, variables);
        printf("    add rax, rdx\n");
        break;
    }
    case EXPR_MINUS:
    {
        term_asm(&expr->min.lhs, variables);
        printf("    mov rbx, rax\n");
        term_asm(&expr->min.rhs, variables);
        printf("    sub rbx, rax\n");
        printf("    mov rax, rbx\n");
        break;
    }
    case EXPR_MUL:
    {
        term_asm(&expr->mul.lhs, variables); // added mul
        printf("    mov rdx, rax\n");
        term_asm(&expr->mul.rhs, variables);
        printf("    imul rax, rdx\n");
        break;
    }
    case EXPR_DIV:
    {
        term_asm(&expr->div.lhs, variables); // added div
        printf("    mov rdx, rax\n");
        term_asm(&expr->div.rhs, variables);
        printf("    mov rbx, rax\n");
        printf("    mov rax, rdx\n");
        printf("    cqo\n");
        printf("    idiv rbx\n");
        break;
    }
    }
}

void rel_asm(struct rel_node *rel, ds_dynamic_array *variables)
{
    switch (rel->kind)
    {
    case REL_LESS_THAN:
    {
        term_asm(&rel->less_than.lhs, variables);
        printf("    mov rdx, rax\n");
        term_asm(&rel->less_than.rhs, variables);
        printf("    cmp rdx, rax\n");
        printf("    setl al\n");
        printf("    and al, 1\n");
        printf("    movzx rax, al\n");
        break;
    }
    case REL_GREATER_THAN:
    {
        term_asm(&rel->greater_than.lhs, variables); // added greater than
        printf("    mov rdx, rax\n");
        term_asm(&rel->greater_than.rhs, variables);
        printf("    cmp rdx, rax\n");
        printf("    setg al\n");
        printf("    and al, 1\n");
        printf("    movzx rax, al\n");
        break;
    }
    }
}

void instr_asm(struct instr_node *instr, ds_dynamic_array *variables,
               int *if_count)
{
    switch (instr->kind)
    {
    case INSTR_ASSIGN:
    {
        expr_asm(&instr->assign.expr, variables);
        int index = find_variable(variables, instr->assign.ident);
        printf("    mov qword [rbp - %d], rax\n", index * 8 + 8);
        break;
    }
    case INSTR_IF:
    {
        rel_asm(&instr->if_term.rel, variables); // changed if_ to if_term
        int label = (*if_count)++;
        printf("    test rax, rax\n");
        printf("    jz .endif%d\n", label);
        instr_asm(instr->if_term.instr, variables, if_count);
        printf(".endif%d:\n", label);
        break;
    }

    case INSTR_LABEL:
    {
        printf(".%s:\n", instr->label.label);
        break;
    }

    case INSTR_GOTO:
    {
        printf("    jmp .%s\n", instr->goto_term.label);
        break;
    }
    case INSTR_OUTPUT:
    {
        term_asm(&instr->output.term, variables);
        struct term_node *term_Check = &instr->output.term;
        if (strcmp(term_Check->value, ",") == 0)
        {
            break;
        }

        printf("    mov rdi, 1\n");
        printf("    mov rsi, rax\n");
        printf("    call write_uint\n");
        break;
    }
    }
}

void term_declare_variables(struct term_node *term, ds_dynamic_array *variables)
{
    switch (term->kind)
    {
    case TERM_INPUT:
        break;
    case TERM_INT:
        break;
    case TERM_IDENT:
        for (unsigned int i = 0; i < variables->count; i++)
        {
            char *variable = NULL;
            ds_dynamic_array_get(variables, i, &variable);

            if (strcmp(term->value, variable) == 0)
            {
                return;
            }
        }
        if (term->value = ",")
        {
            return;
        }

        DS_PANIC("Identifier is not defined %s", term->value);
        break;
    }
}

void expr_declare_variables(struct expr_node *expr,
                            ds_dynamic_array *variables)
{
    switch (expr->kind)
    {
    case EXPR_TERM:
    {
        term_declare_variables(&expr->term, variables);
        break;
    }
    case EXPR_PLUS:
    {
        term_declare_variables(&expr->add.lhs, variables);
        term_declare_variables(&expr->add.rhs, variables);
        break;
    }
    case EXPR_MINUS:
    {
        term_declare_variables(&expr->min.lhs, variables); // added min
        term_declare_variables(&expr->min.rhs, variables);
        break;
    }

    case EXPR_MUL:
    {
        term_declare_variables(&expr->mul.lhs, variables); // added mul
        term_declare_variables(&expr->mul.rhs, variables);
        break;
    }

    case EXPR_DIV:
    {
        term_declare_variables(&expr->div.lhs, variables); // added div
        term_declare_variables(&expr->div.rhs, variables);
        break;
    }
    }
}

void rel_declare_variables(struct rel_node *rel, ds_dynamic_array *variables)
{
    switch (rel->kind)
    {
    case REL_LESS_THAN:
        term_declare_variables(&rel->less_than.lhs, variables);
        term_declare_variables(&rel->less_than.rhs, variables);
        break;

    case REL_GREATER_THAN:
        term_declare_variables(&rel->greater_than.lhs, variables); // added greater than
        term_declare_variables(&rel->greater_than.rhs, variables);
        break;
    }
}

void instr_declare_variables(struct instr_node *instr,
                             ds_dynamic_array *variables)
{
    switch (instr->kind)
    {
    case INSTR_ASSIGN:
    {
        expr_declare_variables(&instr->assign.expr, variables);
        for (unsigned int i = 0; i < variables->count; i++)
        {
            char *variable = NULL;
            ds_dynamic_array_get(variables, i, &variable);

            if (strcmp(instr->assign.ident, variable) == 0)
            {
                return;
            }
        }
        ds_dynamic_array_append(variables, &instr->assign.ident);
        break;
    }
    case INSTR_IF:
    {
        rel_declare_variables(&instr->if_term.rel, variables);    // changed if_ to if_term
        instr_declare_variables(instr->if_term.instr, variables); // changed if_ to if_term
        break;
    }
    case INSTR_GOTO:
    {
        break;
    }
    case INSTR_OUTPUT:
    {
        term_declare_variables(&instr->output.term, variables);
        break;
    }
    case INSTR_LABEL:
        break;
    }
}

// void print_asm(struct program_node *program)
// {
//     ds_dynamic_array variables;

//     ds_dynamic_array_init(&variables, sizeof(char *));

//     for (unsigned int i = 0; i < program->instrs.count; i++)
//     {
//         struct instr_node instr;
//         ds_dynamic_array_get(&program->instrs, i, &instr);

//         instr_declare_variables(&instr, &variables);
//     }

//     for (unsigned int i = 0; i < program->instrs.count; i++)
//     {
//         struct instr_node instr;

//         ds_dynamic_array_get(&program->instrs, i, &instr);
//         instr_asm(&instr);
//     }
// }

void program_asm(struct program_node *program)
{
    int if_count = 0;
    ds_dynamic_array variables;
    ds_dynamic_array_init(&variables, sizeof(char *));

    for (unsigned int i = 0; i < program->instrs.count; i++)
    {
        struct instr_node instr;
        ds_dynamic_array_get(&program->instrs, i, &instr);

        instr_declare_variables(&instr, &variables);
    }

    printf("format ELF64 executable\n");
    printf("LINE_MAX equ 1024\n");
    printf("segment readable executable\n");
    printf("include \"./include/linux.inc\"\n");
    printf("include \"./include/utils.inc\"\n");
    printf("entry _start\n");
    printf("_start:\n");

    printf("    mov rbp, rsp\n");

    printf("    sub rsp, %d\n", variables.count * 8);

    for (unsigned int i = 0; i < program->instrs.count; i++)
    {
        struct instr_node instr;
        ds_dynamic_array_get(&program->instrs, i, &instr);

        instr_asm(&instr, &variables, &if_count);
    }
    printf("    add rsp, %d\n", variables.count * 8);
    printf("    mov rdi, 1\n");
    printf("    mov rsi, newline_label\n");
    printf("    mov rdx, 1\n");
    printf("    mov rax, 1\n");
    printf("    syscall\n");

    printf("    mov rax, 60\n");
    printf("    xor rdi, rdi\n");
    printf("    syscall\n");

    printf("segment readable writeable\n");
    printf("line rb LINE_MAX\n");
    printf("comma db ',',0\n");
    printf("newline_label: db 0x0A ,0\n");
    printf("input_label: db \"Input: \",0\n");
}