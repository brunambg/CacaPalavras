#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100 // Tamanho máximo de palavras e da matriz

// Estrutura para guardar as palavras e a quantidade de vezes que aparecem
typedef struct {
    char texto[MAX];
    int contagem;
} Palavra;

// Variáveis globais (porque facilita a minha vida nesse caso hehe)
char matriz[MAX][MAX];
int linhas = 0, colunas = 0;
Palavra palavras[MAX];
int totalPalavras = 0;

// Direções para procurar (8 direções: diagonais, horizontais e verticais)
int dx[] = {-1,-1,-1, 0, 0, 1, 1, 1};
int dy[] = {-1, 0, 1,-1, 1,-1, 0, 1};

// Função para transformar tudo em maiúsculo (pra comparar certinho)
void paraMaiusculo(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
}

// Lê o arquivo com a lista de palavras (palavras.txt)
void lerPalavras() {
    FILE *f = fopen("palavras.txt", "r");
    if (!f) {
        printf("Erro ao abrir palavras.txt\n");
        exit(1);
    }

    while (fgets(palavras[totalPalavras].texto, MAX, f)) {
        palavras[totalPalavras].texto[strcspn(palavras[totalPalavras].texto, "\n")] = '\0'; // remove \n
        paraMaiusculo(palavras[totalPalavras].texto);
        palavras[totalPalavras].contagem = 0;
        totalPalavras++;
    }

    fclose(f);
}

// Lê a matriz de letras do arquivo (matriz.txt)
void lerMatriz() {
    FILE *f = fopen("matriz.txt", "r");
    if (!f) {
        printf("Erro ao abrir matriz.txt\n");
        exit(1);
    }

    while (fgets(matriz[linhas], MAX, f)) {
        matriz[linhas][strcspn(matriz[linhas], "\n")] = '\0'; // Remove o \n do final da linha
        paraMaiusculo(matriz[linhas]);                         // Deixa tudo maiúsculo pra facilitar
        linhas++;
    }

    colunas = strlen(matriz[0]); // Considera o tamanho da primeira linha pra colunas
    fclose(f);
}

// Procura uma palavra a partir da posição (x, y) em todas as direções possíveis
int encontrouPalavra(char *palavra, int x, int y) {
    int len = strlen(palavra);

    // Testa cada uma das 8 direções
    for (int dir = 0; dir < 8; dir++) {
        int nx = x, ny = y;
        int i;

        // Verifica letra a letra se a palavra cabe nessa direção
        for (i = 0; i < len; i++) {
            // Se sair da matriz, para
            if (nx < 0 || ny < 0 || nx >= linhas || ny >= colunas)
                break;

            // Se a letra da matriz não for igual à da palavra, para
            if (matriz[nx][ny] != palavra[i])
                break;

            // Avança para próxima posição na direção atual
            nx += dx[dir];
            ny += dy[dir];
        }

        // Se chegou ao fim da palavra, achou ela!
        if (i == len)
            return 1; // Verdadeiro, palavra encontrada nessa direção
    }

    // Se nenhuma direção funcionou, palavra não está aqui
    return 0;
}

// Conta quantas vezes cada palavra aparece na matriz
void contarPalavras() {
    for (int p = 0; p < totalPalavras; p++) {    // Para cada palavra
        for (int i = 0; i < linhas; i++) {       // Para cada linha da matriz
            for (int j = 0; j < colunas; j++) { // Para cada coluna da matriz
                // Só tenta encontrar a palavra se a primeira letra bater
                if (matriz[i][j] == palavras[p].texto[0]) {
                    if (encontrouPalavra(palavras[p].texto, i, j)) {
                        palavras[p].contagem++; // Achou mais uma ocorrência
                    }
                }
            }
        }
    }
}

// Compara para ordenar por contagem decrescente e, em caso de empate, por ordem alfabética
int comparar(const void *a, const void *b) {
    Palavra *p1 = (Palavra *)a;
    Palavra *p2 = (Palavra *)b;

    if (p2->contagem != p1->contagem)
        return p2->contagem - p1->contagem;

    return strcmp(p1->texto, p2->texto);
}

// Escreve o resultado no arquivo resultado.txt
void escreverResultado() {
    FILE *f = fopen("resultado.txt", "w");
    if (!f) {
        printf("Erro ao criar resultado.txt\n");
        exit(1);
    }

    // Escreve só as palavras que apareceram pelo menos uma vez
    for (int i = 0; i < totalPalavras; i++) {
        if (palavras[i].contagem > 0) {
            fprintf(f, "%s, %d\n", palavras[i].texto, palavras[i].contagem);
        }
    }

    fclose(f);
}

//  Função principal do programinha
int main() {
    printf("\nSeja bem-vindo ao Caça-Palavras!\n\n");

    lerPalavras(); // Lê as palavras a serem buscadas
    lerMatriz();   // Lê a matriz de letras

    printf("Buscando palavras na matriz...\n\n");
    contarPalavras(); // Procura tudo

    qsort(palavras, totalPalavras, sizeof(Palavra), comparar); // Ordena resultado

    escreverResultado();
    printf("\nProntinho! Resultado salvo em 'resultado.txt' \n");

    return 0;
}
