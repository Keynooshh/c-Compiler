#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define DS_IMPLEMENTATION
#include "ds.h"
#include "Lexer.h"

int main(){
    char *buffer = NULL;
    int length = ds_io_read_file(NULL,&buffer);

    ds_dynamic_array tokens;
    ds_dynamic_array_init(&tokens, sizeof(struct token));

    lexer_tokenize(buffer, length , &tokens );

    for(unsigned int  i=0; i < tokens.count; i++){
        struct token tok;
        ds_dynamic_array_get(&tokens, i, &tok);
        print_token(tok);
    }

    printf("%s\n",buffer);
}