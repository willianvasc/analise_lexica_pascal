#include <stdio.h>
#include <string.h>
#include "symbol_table.h"

void initSymbolTable(SymbolTable *table) {
    table->count = 0;
}

int addSymbol(SymbolTable *table, const char *name, const char *lexeme) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->symbols[i].name, name) == 0) {
            return 0;
        }
    }
    strcpy(table->symbols[table->count].name, name);
    strcpy(table->symbols[table->count].lexeme, lexeme);
    table->count++;
    return 1;
}

int findSymbol(SymbolTable *table, const char *name) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->symbols[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void printSymbolTable(SymbolTable *table) {
    printf("Symbol Table:\n");
    for (int i = 0; i < table->count; i++) {
        printf("<%s, %s>\n", table->symbols[i].name, table->symbols[i].lexeme);
    }
}
