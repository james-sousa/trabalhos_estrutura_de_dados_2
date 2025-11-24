#ifndef HASHING_H
#define HASHING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char matricula[12];  // 11 dígitos + \0
    char nome[100];
    char curso[50];
    int periodo;
    int ano_ingresso;
} Aluno;

typedef struct {
    int tamanho_tabela;
    int total_insercoes;
    int total_colisoes;
    int reposicoes;   // quando um elemento é removido para inserir outro
} Estatisticas;

// -------- HASH (A): Rotação --------
int hash_rotacao(char *mat);
int tratar_colisao_rotacao(char *mat, int pos, int tamanho);

// -------- HASH (B): Fold Shift --------
int hash_fold(char *mat);
int tratar_colisao_fold(char *mat, int pos, int tamanho);

// -------- Funções de operação --------
void inserir_rotacao(Aluno **tab, Aluno aluno, Estatisticas *st);
void inserir_fold(Aluno **tab, Aluno aluno, Estatisticas *st);

void gerar_base(Aluno *v, int qtd);
void limpar_tabela(Aluno **tab, int tam);
void exibir_stats(Estatisticas st, const char *nome);

#endif
