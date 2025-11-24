#include "hashing.h"

int main() {
    srand(time(NULL));

    const int QTDE = 4000;

    Aluno *base = malloc(sizeof(Aluno) * QTDE);
    gerar_base(base, QTDE);

    // ==========================================================
    // TABELA DE TAMANHO 1211
    // ==========================================================
    const int TAM1 = 1211;
    printf("\n\n========================\n");
    printf(" TABELA COM TAMANHO %d\n", TAM1);
    printf("========================\n");

    // ---------- HASH ROTACAO ----------
    Aluno **tab1a = calloc(TAM1, sizeof(Aluno *));
    Estatisticas st1a = {TAM1, 0, 0, 0};

    for (int i = 0; i < QTDE; i++)
        inserir_rotacao(tab1a, base[i], &st1a);

    exibir_stats(st1a, "Método A: Rotação (1211)");
    limpar_tabela(tab1a, TAM1);
    free(tab1a);

    // ---------- HASH FOLD ----------
    Aluno **tab1b = calloc(TAM1, sizeof(Aluno *));
    Estatisticas st1b = {TAM1, 0, 0, 0};

    for (int i = 0; i < QTDE; i++)
        inserir_fold(tab1b, base[i], &st1b);

    exibir_stats(st1b, "Método B: Fold Shift (1211)");
    limpar_tabela(tab1b, TAM1);
    free(tab1b);

    // ==========================================================
    // TABELA DE TAMANHO 1280
    // ==========================================================
    const int TAM2 = 1280;
    printf("\n\n========================\n");
    printf(" TABELA COM TAMANHO %d\n", TAM2);
    printf("========================\n");

    // ---------- HASH ROTACAO ----------
    Aluno **tab2a = calloc(TAM2, sizeof(Aluno *));
    Estatisticas st2a = {TAM2, 0, 0, 0};

    for (int i = 0; i < QTDE; i++)
        inserir_rotacao(tab2a, base[i], &st2a);

    exibir_stats(st2a, "Método A: Rotação (1280)");
    limpar_tabela(tab2a, TAM2);
    free(tab2a);

    // ---------- HASH FOLD ----------
    Aluno **tab2b = calloc(TAM2, sizeof(Aluno *));
    Estatisticas st2b = {TAM2, 0, 0, 0};

    for (int i = 0; i < QTDE; i++)
        inserir_fold(tab2b, base[i], &st2b);

    exibir_stats(st2b, "Método B: Fold Shift (1280)");
    limpar_tabela(tab2b, TAM2);
    free(tab2b);

    free(base);
    return 0;
}
