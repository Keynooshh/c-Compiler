#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "parser.h"
#include "ds.h"

int main(){
    char *buffer = NULL;
    int length = ds_io_read_file(NULL,&buffer);

    ds_dynamic_array tokens;
    ds_dynamic_array_init(&tokens, sizeof(struct token));

    lexer_tokenize(buffer, length , &tokens );

    struct parser p;
    struct program_node program;
    parser_init(tokens,&p);
    parse_program(&p,&program);
}