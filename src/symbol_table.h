#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "token.h"

#define MAX_SYMBOLS 100

typedef struct {
    char name[100];
    char lexeme[100];
} Symbol;

typedef struct {
    Symbol symbols[MAX_SYMBOLS];
    int count;
} SymbolTable;

void initSymbolTable(SymbolTable *table);
int addSymbol(SymbolTable *table, const char *name, const char *lexeme);
int findSymbol(SymbolTable *table, const char *name);
void printSymbolTable(SymbolTable *table);

#endif // SYMBOL_TABLE_H
