#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_SYMBOL,
    TOKEN_UNKNOWN,
    TOKEN_EOF,
} TokenType;

typedef struct {
    TokenType type;
    char value[100];
    int line;
    int column;
} Token;

#endif // TOKEN_H
