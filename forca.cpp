#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 50
#define MAX_PALAVRA 50
#define ARQ_PALAVRAS "palavras.txt"
#define ARQ_RANKING "ranking.txt"

typedef struct {
    char nome[MAX_NOME];
    int pontos;
} Ranking;

typedef struct {
    char palavra[MAX_PALAVRA];
} Palavra;



int contarPalavrasArquivo() {
    FILE *f = fopen(ARQ_PALAVRAS, "r");
    if (!f) {
        printf("Erro ao abrir arquivo de palavras.\n");
        return 0;
    }

    int count = 0;
    char temp[MAX_PALAVRA];

    while (fgets(temp, MAX_PALAVRA, f))
        count++;

    fclose(f);
    return count;
}


char *carregarPalavraAleatoria() {
    int total = contarPalavrasArquivo();
    if (total == 0) return NULL;

    FILE *f = fopen(ARQ_PALAVRAS, "r");
    if (!f) return NULL;

    Palavra *vetor = (Palavra*) malloc(sizeof(Palavra) * total);

    int i = 0;
    while (fgets(vetor[i].palavra, MAX_PALAVRA, f)) {
        vetor[i].palavra[strcspn(vetor[i].palavra, "\n")] = '\0';
        i++;
    }

    fclose(f);

    int sorteada = rand() % total;

    char *palavra = (char*) malloc(strlen(vetor[sorteada].palavra) + 1);
    strcpy(palavra, vetor[sorteada].palavra);

    free(vetor);

    return palavra;
}



void salvarRanking(const char *nome, int pontos) {
    FILE *f = fopen(ARQ_RANKING, "a");
    if (!f) {
        printf("Erro ao salvar ranking.\n");
        return;
    }

    Ranking r;
    strcpy(r.nome, nome);
    r.pontos = pontos;

    fwrite(&r, sizeof(Ranking), 1, f);
    fclose(f);
}


void verRanking() {
    FILE *f = fopen(ARQ_RANKING, "r");
    if (!f) {
        printf("Ranking vazio.\n");
        return;
    }

    Ranking r;

    printf("\n===== RANKING =====\n");

    while (fread(&r, sizeof(Ranking), 1, f)) {
        printf("Jogador: %s - Pontos: %d\n", r.nome, r.pontos);
    }

    fclose(f);
}


void cadastrarPalavra() {
    char palavra[MAX_PALAVRA];
    printf("Digite a nova palavra: ");
    scanf("%s", palavra);

    FILE *f = fopen(ARQ_PALAVRAS, "a");
    fprintf(f, "%s\n", palavra);
    fclose(f);

    printf("Palavra cadastrada com sucesso!\n");
}


void jogar() {
    char nome[MAX_NOME];
    printf("Digite seu nome: ");
    scanf("%s", nome);

    char *palavra = carregarPalavraAleatoria();
    if (!palavra) {
        printf("Erro ao carregar palavra.\n");
        return;
    }

    int tamanho = strlen(palavra);

    char *visivel = (char*) malloc(tamanho + 1);

    for (int i = 0; i < tamanho; i++)
        visivel[i] = '_';
    visivel[tamanho] = '\0';

    int erros = 0;
    int pontos = 0;
    char letra;
    int pos;

    printf("\n=== JOGO DA FORCA ===\n");

    while (erros < 6) {
        printf("\nPalavra: %s\n", visivel);
        printf("Erros: %d/6\n", erros);

        printf("Digite uma posição (0-%d): ", tamanho - 1);
        scanf("%d", &pos);

        if (pos < 0 || pos >= tamanho) {
            printf("Posição inválida!\n");
            continue;
        }

        printf("Digite uma letra: ");
        scanf(" %c", &letra);

        if (palavra[pos] == letra) {
            visivel[pos] = letra;
            pontos += 10;
        } else {
            erros++;
        }

        if (strcmp(visivel, palavra) == 0) {
            printf("\nVOCÊ VENCEU!\n");
            printf("Palavra: %s\n", palavra);
            break;
        }
    }

    if (erros >= 6) {
        printf("\nVOCÊ PERDEU!\nA palavra era: %s\n", palavra);
    }

    printf("Pontuação final: %d\n", pontos);

    salvarRanking(nome, pontos);

    free(visivel);
    free(palavra);
}


void menu() {
    int opc;

    do {
        printf("\n===== MENU =====\n");
        printf("1 - Jogar\n");
        printf("2 - Ver Ranking\n");
        printf("3 - Cadastrar Palavras\n");
        printf("4 - Créditos\n");
        printf("5 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opc);

        switch (opc) {
            case 1: jogar(); break;
            case 2: verRanking(); break;
            case 3: cadastrarPalavra(); break;
            case 4: printf("\nJogo da Forca - Feito por Você!\n"); break;
            case 5: printf("Saindo...\n"); break;
            default: printf("Opção inválida.\n");
        }

    } while (opc != 5);
}

int main() {
    srand(time(NULL));
    menu();
    return 0;
}

