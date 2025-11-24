#include "hashing.h"

// ===========================================================
//                 FUNÇÃO HASH (A): ROTAÇÃO
// rotacionar 5 primeiros dígitos p/ direita:
// ABCDE → EABCD
// depois extrair 2º, 4º e 5º da matric rotacionada
// índice = (d2 d4 d5) % tamanho
// colisão = índice + primeiro dígito da matric rotacionada
// ===========================================================

int hash_rotacao(char *mat) {
    char rot[12];
    strcpy(rot, mat);

    // Rotação direita dos 5 primeiros caracteres
    char ultimo = rot[4];
    for (int i = 4; i > 0; i--)
        rot[i] = rot[i - 1];
    rot[0] = ultimo;

    // Escolhe 2º, 4º e 5º
    int d2 = rot[1] - '0';
    int d4 = rot[3] - '0';
    int d5 = rot[4] - '0';

    int valor = d2 * 100 + d4 * 10 + d5;
    return valor;
}

int tratar_colisao_rotacao(char *mat, int pos, int tamanho) {
    char rot[12];
    strcpy(rot, mat);

    char ultimo = rot[4];
    for (int i = 4; i > 0; i--)
        rot[i] = rot[i - 1];
    rot[0] = ultimo;

    int d1 = rot[0] - '0';
    return (pos + d1) % tamanho;
}



// ===========================================================
//             FUNÇÃO HASH (B): FOLD SHIFT
// grupos: (6º,7º,11º) e (8º,9º,10º)
// número = grupo1 + grupo2
// colisão = pos + (6º * 10 + 11º)
// ===========================================================

int hash_fold(char *mat) {
    int g1 = (mat[5] - '0') * 100 + (mat[6] - '0') * 10 + (mat[10] - '0');
    int g2 = (mat[7] - '0') * 100 + (mat[8] - '0') * 10 + (mat[9] - '0');
    return g1 + g2;
}

int tratar_colisao_fold(char *mat, int pos, int tamanho) {
    int salto = (mat[5] - '0') * 10 + (mat[10] - '0');
    return (pos + salto) % tamanho;
}



// ===========================================================
//                 SISTEMA DE INSERÇÃO
// ===========================================================

static Aluno *dup_aluno(Aluno *a) {
    Aluno *p = malloc(sizeof(Aluno));
    memcpy(p, a, sizeof(Aluno));
    return p;
}

void inserir_rotacao(Aluno **tab, Aluno aluno, Estatisticas *st) {
    int pos = hash_rotacao(aluno.matricula) % st->tamanho_tabela;
    int inicio = pos;
    int steps = 0;

    while (steps < st->tamanho_tabela) {
        if (tab[pos] == NULL) {
            tab[pos] = dup_aluno(&aluno);
            st->total_insercoes++;
            return;
        }
        st->total_colisoes++;
        pos = tratar_colisao_rotacao(aluno.matricula, pos, st->tamanho_tabela);
        steps++;
    }

    // Se chegou aqui: substitui o primeiro encontrado
    free(tab[inicio]);
    tab[inicio] = dup_aluno(&aluno);
    st->reposicoes++;
}

void inserir_fold(Aluno **tab, Aluno aluno, Estatisticas *st) {
    int pos = hash_fold(aluno.matricula) % st->tamanho_tabela;
    int inicio = pos;
    int steps = 0;

    while (steps < st->tamanho_tabela) {
        if (tab[pos] == NULL) {
            tab[pos] = dup_aluno(&aluno);
            st->total_insercoes++;
            return;
        }
        st->total_colisoes++;
        pos = tratar_colisao_fold(aluno.matricula, pos, st->tamanho_tabela);
        steps++;
    }

    free(tab[inicio]);
    tab[inicio] = dup_aluno(&aluno);
    st->reposicoes++;
}



// ===========================================================
//                 OUTRAS FUNÇÕES
// ===========================================================

void gerar_base(Aluno *v, int qtd) {
    for (int i = 0; i < qtd; i++) {
        for (int d = 0; d < 11; d++)
            v[i].matricula[d] = '0' + rand() % 10;
        v[i].matricula[11] = '\0';
        sprintf(v[i].nome, "Aluno %d", i + 1);
        sprintf(v[i].curso, "Curso %d", (i % 6) + 1);
        v[i].periodo = (rand() % 10) + 1;
        v[i].ano_ingresso = 2015 + rand() % 10;
    }
}

void limpar_tabela(Aluno **tab, int tam) {
    for (int i = 0; i < tam; i++)
        if (tab[i]) free(tab[i]);
}

void exibir_stats(Estatisticas st, const char *nome) {
    printf("\n===== %s =====\n", nome);
    printf("Tamanho tabela: %d\n", st.tamanho_tabela);
    printf("Total inserções: %d\n", st.total_insercoes);
    printf("Total colisões: %d\n", st.total_colisoes);
    printf("Reposições (substituições): %d\n", st.reposicoes);
    printf("=============================\n");
}
