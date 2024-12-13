#ifndef LEXER_H // Include guard to prevent multiple inclusions
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Forward declaration of the dynamic array and string slice types, assuming you have the necessary definitions for them
#include "ds.h"

// Enum for different token kinds
enum token_kind
{
    TK_INPUT,
    TK_OUTPUT,
    TK_IF,
    TK_THEN,
    TK_GOTO,
    TK_LABEL,
    TK_IDENT,
    TK_INT,
    TK_EQUAL,
    TK_PLUS,
    TK_MINUS,
    TK_MULTIPLY,
    TK_DIVIDE,
    TK_LTHAN,
    TK_GTHAN,
    TK_COMMA,
    INVALID,
    END
};

// Token struct definition
struct token
{
    enum token_kind kind;
    char *value;
};

// Function declarations
const char *show_token_kind(enum token_kind kind);
void print_token(struct token tok);

// Lexer struct definition
struct lexer
{
    char *buffer;
    unsigned int buffer_len;
    unsigned int pos;
    unsigned int read_pos;
    char ch;
};

// Function declarations for lexer
void lexer_init(struct lexer *l, char *buffer, unsigned int buffer_len);
struct token lexer_next_token(struct lexer *l);
int lexer_tokenize(char *buffer, unsigned int length, ds_dynamic_array *tokens);

#endif // LEXER_H
