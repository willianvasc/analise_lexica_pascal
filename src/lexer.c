#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

#define MAX_KEYWORDS 10

const char *keywords[] = {
    "program", "var", "integer", "real",
    "begin", "end", "if", "then", "else", "while", "do", "char"
};

// Verifica se a palavra e uma palavra-chave
int isKeyword(const char *word) {
    for (int i = 0; i < MAX_KEYWORDS; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Verifica se o identificador começa com uma palavra-chave, o que nao e permitido
int startsWithKeyword(const char *id) {
    for (int i = 0; i < MAX_KEYWORDS; i++) {
        if (strncmp(id, keywords[i], strlen(keywords[i])) == 0) {
            return 1;
        }
    }
    return 0;
}

// Verifica se o identificador é válido
int isValidIdentifier(const char *id) {
    // Verifica se o primeiro caractere é um número, o que não é permitido
    if (isdigit(id[0])) {
        return 0; // Retorna inválido se começar com um número
    }

    // Verifica se o primeiro caractere é uma letra. Não permitimos underline como primeiro caractere
    if (!isalpha(id[0])) {
        return 0;
    }

    // Verifica se o identificador começa com uma palavra-chave
    if (startsWithKeyword(id)) {
        return 0;
    }

    // Verifica se todos os outros caracteres são letras, números ou underline
    for (int i = 1; id[i] != '\0'; i++) {
        if (!isalnum(id[i]) && id[i] != '_') {
            return 0;
        }
    }
    return 1;
}

// Obtem o próximo token
Token getNextToken(FILE *file, int *line, int *column, SymbolTable *table) {
    Token token;
    char ch;
    char buffer[100];
    int index;

    while ((ch = fgetc(file)) != EOF) {
        (*column)++;

        // Ignora espaços em branco e novas linhas
        if (isspace(ch)) {
            if (ch == '\n') {
                (*line)++;
                *column = 0;
            }
            continue;
        }
        memset(token.value, 0, sizeof(token.value));
        token.line = *line;
        token.column = *column;

        // Ignora comentarios de linha
        if (ch == '/') {
            if ((ch = fgetc(file)) == '/') {
                (*column)++;
                while (ch != '\n' && ch != EOF) {
                    ch = fgetc(file);
                    (*column)++;
                }
                if (ch == '\n') {
                    (*line)++;
                    *column = 0;
                }
                continue;
            } else {
                ungetc(ch, file);
                (*column)--;
            }
        }

        // Ignora comentarios de bloco
        if (ch == '(') {
            if ((ch = fgetc(file)) == '*') {
                (*column)++;
                while (1) {
                    while (ch != '*' && ch != EOF) {
                        ch = fgetc(file);
                        (*column)++;
                        if (ch == '\n') {
                            (*line)++;
                            *column = 0;
                        }
                    }
                    if (ch == EOF) {
                        printf("Erro de sintaxe: Comentario de bloco nao fechado na linha %d, coluna %d\n", *line, *column);
                        token.type = TOKEN_EOF;
                        strcpy(token.value, "EOF");
                        return token;
                    }
                    ch = fgetc(file);
                    (*column)++;
                    if (ch == ')') break;
                }
                continue;
            } else {
                ungetc(ch, file);
                (*column)--;
            }
        }

        index = 0;

        // Identificaçao de números
        if (isdigit(ch)) {
            buffer[index++] = ch;
            while (isdigit(ch = fgetc(file))) {
                buffer[index++] = ch;
                (*column)++;
            }
            if (ch == '.') {
                buffer[index++] = ch;
                (*column)++;
                while (isdigit(ch = fgetc(file))) {
                    buffer[index++] = ch;
                    (*column)++;
                }
            }
            ungetc(ch, file);
            (*column)--;
            buffer[index] = '\0';
            token.type = TOKEN_NUMBER;
            strcpy(token.value, buffer);
            return token;
        }

        // Reconhecimento de identificadores e palavras-chave
        if (isalpha(ch) || ch == '_') {
            buffer[index++] = ch;
            while (isalnum(ch = fgetc(file)) || ch == '_') {
                buffer[index++] = ch;
                (*column)++;
            }
            ungetc(ch, file);
            (*column)--;
            buffer[index] = '\0';

            if (isKeyword(buffer)) {
                token.type = TOKEN_KEYWORD;

                if (strcmp(buffer, "end") == 0) {
                    ch = fgetc(file);
                    (*column)++;
                    if (ch == '.') {
                        strcpy(token.value, "end.");
                        token.type = TOKEN_KEYWORD;
                        return token;
                    } else {
                        ungetc(ch, file);
                        (*column)--;
                    }
                }
            } else if (isValidIdentifier(buffer)) {
                token.type = TOKEN_IDENTIFIER;
                addSymbol(table, buffer, buffer);
            } else {
                token.type = TOKEN_UNKNOWN;
                // Exibe erro caso o identificador seja invalido (começa com número ou outra regra)
                printf("Erro lexico: Identificador invalido: %s na linha %d, coluna %d\n", buffer, *line, *column);
            }
            strcpy(token.value, buffer);
            return token;
        }

        // Reconhecimento de operadores e símbolos
        if (strchr("+-*/:=<>", ch)) {
            token.type = TOKEN_OPERATOR;
            token.value[0] = ch;
            token.value[1] = '\0';
            return token;
        }

        if (strchr("(){};,", ch)) {
            token.type = TOKEN_SYMBOL;
            token.value[0] = ch;
            token.value[1] = '\0';
            return token;
        }

        // Caractere desconhecido
        token.type = TOKEN_UNKNOWN;
        token.value[0] = ch;
        token.value[1] = '\0';
        return token;
    }

    token.type = TOKEN_EOF;
    strcpy(token.value, "EOF");
    return token;
}

// Funçao para analisar o arquivo
void analyzeFile(const char *filePath) {
    FILE *file = fopen(filePath, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    SymbolTable table;
    initSymbolTable(&table);

    Token token;
    int line = 1, column = 0;

    do {
        token = getNextToken(file, &line, &column, &table);
        if (token.type != TOKEN_EOF) {
            printf("Token: %-10s Valor: %-20s Linha: %d Coluna: %d\n",
                   token.type == TOKEN_KEYWORD ? "PALAVRA CHAVE" :
                   token.type == TOKEN_IDENTIFIER ? "IDENTIFICADOR" :
                   token.type == TOKEN_NUMBER ? "NUMERO" :
                   token.type == TOKEN_OPERATOR ? "OPERADOR" :
                   token.type == TOKEN_SYMBOL ? "SIMBOLO" : "DESCONHECIDO",
                   token.value, token.line, token.column);
        }

        // Verificaçao para `end.` que pode finalizar o programa
        if (token.type == TOKEN_KEYWORD && strcmp(token.value, "end.") == 0) {
            break;
        }

        // Verificaçao de ponto e vírgula após `begin`
        if (token.type == TOKEN_KEYWORD && strcmp(token.value, "begin") == 0) {
            // Nao precisa de verificaçao de `;` logo após o `begin`, apenas continue
            continue;
        }

        // Se for `end`, verifique se ha `;` opcionalmente após ele.
        if (token.type == TOKEN_KEYWORD && strcmp(token.value, "end") == 0) {
            token = getNextToken(file, &line, &column, &table);
            if (token.type == TOKEN_SYMBOL && strcmp(token.value, ".") == 0) {
                printf("Token: %-10s Valor: %-20s Linha: %d Coluna: %d\n",
                       "SIMBOLO", token.value, token.line, token.column);
                break; // Fim do programa
            } else if (token.type == TOKEN_SYMBOL && strcmp(token.value, ";") == 0) {
                // End do bloco; continue normalmente
                continue;
            } else {
                printf("Erro de sintaxe: Esperado `;` ou `.` após `end` na linha %d, coluna %d\n", token.line, token.column);
                continue;
            }
        }
    } while (token.type != TOKEN_EOF);

    fclose(file);
    printSymbolTable(&table);
}
