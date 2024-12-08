#define LEXER

enum token_kind {
    TK_INPUT,
    TK_OUTPUT,
    TK_IF,
    TK_THEN,
    TK_GOTO,
    TK_LABLE,
    TK_IDENT,
    TK_INT,
    TK_EQUAL,
    TK_PLUS,
    TK_MINUS, 
    TK_MULTIPLY, 
    TK_DIVIDE,
    TK_LTHAN,
    TK_GTHAN,
    TK_LPAREN,
    TK_RPAREN,
    INVALID,
    END
};

struct token {
    enum token_kind kind;
    char *value;
};

const char * show_token_kind(enum token_kind kind) {
    switch(kind){
        case TK_INPUT:     return "input";
        case TK_OUTPUT:    return "output";
        case TK_IF:        return "if";
        case TK_THEN:      return "then";
        case TK_GOTO:      return "goto";
        case TK_LABLE:     return "label";
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

void print_token(struct token tok){
    const char *kind = show_token_kind(tok.kind);
    printf("%s",kind);
    if(tok.value != NULL){
       printf("(%s)", tok.value); 
    }
    printf("\n");
}

struct lexer {
    char *buffer;
    unsigned int buffer_len;
    unsigned int pos;
    unsigned int read_pos;
    char ch;
};

static char lexer_peek_char(struct lexer*l){
    if (l->read_pos >= l->buffer_len){
        return EOF;
    }

    return l->buffer[l->read_pos];
}

static char lexer_read_char(struct lexer*l){
    l-> ch = lexer_peek_char(l);
    l->pos = l->read_pos;
    l->read_pos++;
    return l->ch;
}

static void lexer_init(struct lexer*l, char *buffer, unsigned int buffer_len){
    l->buffer = buffer;
    l->buffer_len = buffer_len;
    l->pos = 0;
    l->read_pos = 0;
    l->ch = 0;

    lexer_read_char(l);
}

//white skip
static void skip_whitespaces(struct lexer*l){
    while(isspace(l->ch)){
        lexer_read_char(l);
    }
}

static struct token lexer_next_token(struct lexer*l){
    skip_whitespaces(l);
    
    if(l->ch == EOF){
        lexer_read_char(l);
        return (struct token){.kind = END, .value =NULL};
    }


    //:label
     else if (l->ch == ':'){
        lexer_read_char(l);
        ds_string_slice slice = {.str = l->buffer + l ->pos, .len=0};
        while(isalnum(l->ch) || l->ch == '_'){
            slice.len += 1;
            lexer_read_char(l);
        }
        char *value = NULL;
        ds_string_slice_to_owned(&slice, &value);
        return (struct token){.kind = TK_LABLE, .value = value};
     }


    //digit
     else if (isdigit(l->ch)){
        ds_string_slice slice = {.str = l->buffer + l ->pos, .len=0};
        while(isdigit(l->ch)){
            slice.len += 1;
            lexer_read_char(l);
        }
        char *value = NULL;
        ds_string_slice_to_owned(&slice, &value);
        return (struct token){.kind = TK_INT, .value = value};
     }


    //keywords
     else if (isalnum(l->ch) || l->ch == '_') {
        ds_string_slice slice = {.str = l->buffer + l ->pos, .len=0};
        while(isalnum(l->ch) || l->ch == '_'){
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
            return (struct token){.kind = TK_LABLE, .value = NULL};
        } else{
            return (struct token){.kind = TK_IDENT, .value = value};
        }

    }
   
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
    } else if (l->ch == '\0') {  
        return (struct token){.kind = END, .value = NULL};
    } else {
        ds_string_slice slice = {.str = l->buffer + l ->pos, .len=1};
        char *value = NULL;
        ds_string_slice_to_owned(&slice, &value);
        lexer_read_char(l);
        return (struct token){.kind = INVALID, .value = value};
    }
}

int lexer_tokenize(char *buffer, unsigned int length,
                   ds_dynamic_array *tokens) {
    struct lexer lexer;
    lexer_init(&lexer, (char *)buffer, length);

    struct token tok;
    do {
        tok = lexer_next_token(&lexer);
        if (ds_dynamic_array_append(tokens, &tok) != 0) {
            DS_PANIC("Failed to append token to array");
        }
    } while (tok.kind != END);

    return 0;
}