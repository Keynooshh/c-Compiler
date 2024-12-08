#define PARSER

enum instr_kind{
    INSTR_ASSIGN,     
    INSTR_ADD,      
    INSTR_SUB,        
    INSTR_MUL,        
    INSTR_DIV,       
    INSTR_IF,         
    INSTR_GOTO,       
    INSTR_INPUT,      
    INSTR_OUTPUT,     
    INSTR_LABEL,      
    INSTR_IDENTIFIER, 
    INSTR_INTEGER,    
    INSTR_EQUAL,      
    INSTR_PLUS,       
    INSTR_MINUS,      
    INSTR_MULTIPLY,   
    INSTR_DIVIDE,     
    INSTR_LTHAN,      
    INSTR_GTHAN,      
    INSTR_LPAREN,     
    INSTR_RPAREN,     
    INSTR_PRINT,      
    INSTR_END   
}

struct instr_node{
    enum instr_kind kind;
    union{

    };
};

struct program_node{
    ds_dynamic_array instrs;
};