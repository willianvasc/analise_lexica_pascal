#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

#define MAX_PATH_LENGTH 260

int main() {
    char filePath[MAX_PATH_LENGTH];
    char fullPath[MAX_PATH_LENGTH];

    printf("Digite o nome do arquivo presente na pasta 'testes': ");

    // Lê o nome do arquivo do usuário e remove o caractere de nova linha
    fgets(filePath, MAX_PATH_LENGTH, stdin);
    filePath[strcspn(filePath, "\n")] = 0; // Remove a quebra de linha do final

    // Define o caminho completo do arquivo dentro da pasta "teste"
    snprintf(fullPath, MAX_PATH_LENGTH, "../testes/%s", filePath);

    // Chama a função de análise léxica com o caminho ajustado
    analyzeFile(fullPath);

    // Pausa antes de fechar o programa
    printf("\nPressione Enter para sair...");
    getchar();

    return 0;
}
