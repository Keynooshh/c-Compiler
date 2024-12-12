#define LEXER
#include "lexer.h"
#include "ds.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



// Function to return the string representation of a token kind
const char *show_token_kind(enum token_kind kind) {
    switch(kind){
        case TK_INPUT:     return "input";
        case TK_OUTPUT:    return "output";
        case TK_IF:        return "if";
        case TK_THEN:      return "then";
        case TK_GOTO:      return "goto";
        case TK_LABEL:     return "label";
        case TK_IDENT:     return "identifier";
        case TK_INT:       return "integer";
        case TK_EQUAL:     return "=";
        case TK_PLUS:      return "+";
        case TK_MINUS:     return "-";
        case TK_MULTIPLY:  return "*";
        case TK_DIVIDE:    return "/";
        case TK_LTHAN:     return "<";
        case TK_GTHAN:     return ">";
        case TK_LPAREN:    return "(";
        case TK_RPAREN:    return ")";
        case INVALID:      return "invalid"; 
        case END:          return "end";
    }
}

// Function to print a token's information
void print_token(struct token tok){
    const char *kind = show_token_kind(tok.kind);
    printf("%s", kind);
    if(tok.value != NULL){
        printf("(%s)", tok.value); 
    }
    printf("\n");
}

// Peek the next character from the buffer without advancing
static char lexer_peek_char(struct lexer* l) {
    if (l->read_pos >= l->buffer_len){
        return EOF;
    }
    return l->buffer[l->read_pos];
}

// Read the next character from the buffer and update lexer state
static char lexer_read_char(struct lexer* l) {
    l->ch = lexer_peek_char(l);
    l->pos = l->read_pos;
    l->read_pos++;
    return l->ch;
}

// Initialize lexer with the provided buffer and buffer length
void lexer_init(struct lexer* l, char *buffer, unsigned int buffer_len) {
    l->buffer = buffer;
    l->buffer_len = buffer_len;
    l->pos = 0;
    l->read_pos = 0;
    l->ch = 0;
    lexer_read_char(l);
}

// Skip whitespace characters (spaces, tabs, newlines)
static void skip_whitespaces(struct lexer* l) {
    while (isspace(l->ch)){
        lexer_read_char(l);
    }
}

// Function to get the next token from the lexer
struct token lexer_next_token(struct lexer* l) {
    skip_whitespaces(l);

    if(l->ch == EOF){
        lexer_read_char(l);
        return (struct token){.kind = END, .value = NULL};
    }

    // Handle label tokens (e.g., :label)
    if (l->ch == ':'){
        lexer_read_char(l);
        ds_string_slice slice = {.str = l->buffer + l->pos, .len = 0};
        while (isalnum(l->ch) || l->ch == '_') {
            slice.len += 1;
            lexer_read_char(l);
        }
        char *value = NULL;
        ds_string_slice_to_owned(&slice, &value);
        return (struct token){.kind = TK_LABEL, .value = value};
    }

    // Handle integer tokens (digits)
    else if (isdigit(l->ch)){
        ds_string_slice slice = {.str = l->buffer + l->pos, .len = 0};
        while (isdigit(l->ch)) {
            slice.len += 1;
            lexer_read_char(l);
        }
        char *value = NULL;
        ds_string_slice_to_owned(&slice, &value);
        return (struct token){.kind = TK_INT, .value = value};
    }

    // Handle keywords or identifiers
    else if (isalnum(l->ch) || l->ch == '_') {
        ds_string_slice slice = {.str = l->buffer + l->pos, .len = 0};
        while (isalnum(l->ch) || l->ch == '_') {
            slice.len += 1;
            lexer_read_char(l);
        }
        char *value = NULL;
        ds_string_slice_to_owned(&slice, &value);
        if (strcmp(value, "input") == 0) {
            return (struct token){.kind = TK_INPUT, .value = NULL};
        } else if (strcmp(value, "output") == 0) {
            return (struct token){.kind = TK_OUTPUT, .value = NULL};
        } else if (strcmp(value, "if") == 0) {
            return (struct token){.kind = TK_IF, .value = NULL};
        } else if (strcmp(value, "then") == 0) {
            return (struct token){.kind = TK_THEN, .value = NULL};
        } else if (strcmp(value, "goto") == 0) {
            return (struct token){.kind = TK_GOTO, .value = NULL};
        } else if (strcmp(value, "label") == 0) {
            return (struct token){.kind = TK_LABEL, .value = NULL};
        } else {
            return (struct token){.kind = TK_IDENT, .value = value};
        }
    }

    // Handle operators and symbols
    else if (l->ch == '=') {
        lexer_read_char(l);
        return (struct token){.kind = TK_EQUAL, .value = NULL};
    } else if (l->ch == '+') {
        lexer_read_char(l);
        return (struct token){.kind = TK_PLUS, .value = NULL};
    } else if (l->ch == '-') {
        lexer_read_char(l);
        return (struct token){.kind = TK_MINUS, .value = NULL};
    } else if (l->ch == '*') {
        lexer_read_char(l);
        return (struct token){.kind = TK_MULTIPLY, .value = NULL};
    } else if (l->ch == '/') {
        lexer_read_char(l);
        return (struct token){.kind = TK_DIVIDE, .value = NULL};
    } else if (l->ch == '<') {
        lexer_read_char(l);
        return (struct token){.kind = TK_LTHAN, .value = NULL};
    } else if (l->ch == '>') {
        lexer_read_char(l);
        return (struct token){.kind = TK_GTHAN, .value = NULL};
    } else if (l->ch == '(') {
        lexer_read_char(l);
        return (struct token){.kind = TK_LPAREN, .value = NULL};
    } else if (l->ch == ')') {
        lexer_read_char(l);
        return (struct token){.kind = TK_RPAREN, .value = NULL};
    }

    // End of string or invalid character
    else if (l->ch == '\0') {
        return (struct token){.kind = END, .value = NULL};
    } else {
        ds_string_slice slice = {.str = l->buffer + l->pos, .len = 1};
        char *value = NULL;
        ds_string_slice_to_owned(&slice, &value);
        lexer_read_char(l);
        return (struct token){.kind = INVALID, .value = value};
    }
}

// Function to tokenize a given buffer and store tokens in a dynamic array
int lexer_tokenize(char *buffer, unsigned int length, ds_dynamic_array *tokens) {
    struct lexer lexer;
    lexer_init(&lexer, buffer, length);

    struct token tok;
    do {
        tok = lexer_next_token(&lexer);
        if (ds_dynamic_array_append(tokens, &tok) != 0) {
            DS_PANIC("Failed to append token to array");
        }
    } while (tok.kind != END);

    return 0;
}
