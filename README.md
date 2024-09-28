### Documentação Completa do Analisador Léxico para a Linguagem MicroPascal

#### Estrutura do Projeto

O projeto está organizado da seguinte forma:

```
-public
--src
----lexer.c
----lexer.h
----main.c
----symbol_table.c
----symbol_table.h
----token.h
--testes
----[arquivo_de_teste.txt]
```

- **`src/`**: Contém os arquivos-fonte do analisador léxico.
- **`testes/`**: Contém os arquivos de teste que serão analisados.

---

### Arquivos do Projeto

#### 1. **`main.c`**

Este arquivo contém a função principal, responsável por capturar a entrada do usuário (nome do arquivo a ser analisado) e invocar o analisador léxico.

- **Função `main()`**: 
  - Solicita ao usuário o nome do arquivo de teste.
  - Define o caminho completo para o arquivo localizado na pasta `testes/`.
  - Chama a função `analyzeFile` para iniciar o processo de análise léxica do arquivo.

**Principais Trechos de Código:**

```c
printf("Digite o nome do arquivo presente na pasta 'testes': ");
fgets(filePath, MAX_PATH_LENGTH, stdin);
snprintf(fullPath, MAX_PATH_LENGTH, "../testes/%s", filePath);
analyzeFile(fullPath);
```

---

#### 2. **`lexer.c`**

O arquivo **`lexer.c`** contém a lógica do analisador léxico. Ele é responsável por ler o arquivo de entrada e identificar tokens, palavras-chave, operadores, símbolos e erros léxicos.

- **Função `isKeyword()`**: Verifica se uma string é uma palavra-chave da linguagem MicroPascal.

- **Função `isValidIdentifier()`**: Valida se um identificador segue as regras da linguagem. Não pode começar com números e não pode ser uma palavra-chave.

- **Função `getNextToken()`**: 
  - Lê o próximo caractere do arquivo e identifica o tipo de token (palavra-chave, identificador, número, operador, símbolo ou erro).
  - Ignora espaços em branco e comentários.
  - Adiciona identificadores válidos à tabela de símbolos.

**Principais Trechos de Código:**

```c
if (isKeyword(buffer)) {
    token.type = TOKEN_KEYWORD;
} else if (isValidIdentifier(buffer)) {
    token.type = TOKEN_IDENTIFIER;
    addSymbol(table, buffer, buffer);
} else {
    token.type = TOKEN_UNKNOWN;
}
```

---

#### 3. **`symbol_table.c`**

Este arquivo lida com a tabela de símbolos, que armazena identificadores e seus lexemas.

- **Função `initSymbolTable()`**: Inicializa a tabela de símbolos.

- **Função `addSymbol()`**: Adiciona um símbolo à tabela se ele ainda não estiver presente.

- **Função `findSymbol()`**: Verifica se um símbolo já existe na tabela, retornando seu índice.

- **Função `printSymbolTable()`**: Exibe todos os símbolos e seus lexemas armazenados na tabela.

**Principais Trechos de Código:**

```c
strcpy(table->symbols[table->count].name, name);
strcpy(table->symbols[table->count].lexeme, lexeme);
table->count++;
```

---

#### 4. **`token.h`**

Este arquivo define a estrutura de um token e seus tipos.

- **Enumeração `TokenType`**: Define os diferentes tipos de tokens possíveis: `TOKEN_KEYWORD`, `TOKEN_IDENTIFIER`, `TOKEN_NUMBER`, `TOKEN_OPERATOR`, `TOKEN_SYMBOL`, `TOKEN_UNKNOWN`, `TOKEN_EOF`.

- **Estrutura `Token`**: Contém informações sobre o token, incluindo seu tipo, valor, linha e coluna.

**Exemplo:**
```c
typedef struct {
    TokenType type;
    char value[100];
    int line;
    int column;
} Token;
```

---

#### 5. **`symbol_table.h`**

Define a estrutura de uma tabela de símbolos e as funções associadas.

- **Estrutura `Symbol`**: Armazena o nome e o lexema de cada símbolo.

- **Estrutura `SymbolTable`**: Contém um array de símbolos e um contador para rastrear quantos símbolos foram adicionados.

---

#### 6. **`lexer.h`**

Define as funções do analisador léxico.

- **Função `isValidIdentifier()`**: Verifica se um identificador é válido.

- **Função `getNextToken()`**: Retorna o próximo token do arquivo.

- **Função `analyzeFile()`**: Executa a análise léxica de um arquivo completo.

---

### Compilação e Execução

Para compilar o projeto, siga os passos abaixo:

1. Navegue até a pasta `src/`:
   ```bash
   cd src
   ```

2. Compile o projeto usando o comando `gcc`:
   ```bash
   gcc -o lexer main.c lexer.c symbol_table.c -I.
   ```

3. Certifique-se de que o arquivo de teste está na pasta `testes/`.

4. Execute o analisador:
   ```bash
   ./lexer
   ```

O analisador solicitará o nome do arquivo de teste. Após informar o nome, o arquivo será analisado e os tokens serão exibidos no console.

---

### Funcionalidades do Analisador Léxico

- **Identificação de Tokens**: O analisador identifica palavras-chave, identificadores, números, operadores e símbolos da linguagem MicroPascal.
- **Tabela de Símbolos**: Armazena identificadores válidos e seus lexemas, evitando duplicações.
- **Tratamento de Erros Léxicos**: Tokens inválidos são sinalizados durante a análise, com indicação da linha e coluna onde o erro ocorreu.

Essa estrutura e funcionalidade cobrem a base de um analisador léxico simples e eficiente para a linguagem MicroPascal, com suporte para adição de mais regras e tipos de tokens conforme necessário.
