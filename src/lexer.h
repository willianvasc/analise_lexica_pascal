#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include "symbol_table.h"

int isValidIdentifier(const char *id);

// Declaração da função para obter o próximo token
Token getNextToken(FILE *file, int *line, int *column, SymbolTable *table);

// Declaração da função de análise léxica
void analyzeFile(const char *filePath); // Corrigido para o nome correto

#endif // LEXER_H
