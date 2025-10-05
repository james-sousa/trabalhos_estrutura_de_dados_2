#ifndef ARVORE_BINARIA_H
#define ARVORE_BINARIA_H

//  James streams e categorias ESTRUTURAS
typedef struct categorias {
    char tipo[30];
    char nome[30];
    struct programa *programas;
    struct categorias *prox;
} Categoria;

typedef struct stream {
    char nome[30];
    char site[100];
    Categoria *categorias;
    struct stream *esq;
    struct stream *dir;
} Stream;

// - programas e apresentadores e historico ESTRUTURAS.
typedef struct programa {
    char nome[30];
    char periodicidade[20];
    int duracao;
    char horario[20];
    int aoVivo;
    char apresentador[50];
    
    struct programa *esq;
    struct programa *dir;

} Programa;

typedef struct historico {
    char nomeStream[50];
    char dataInicio[11];
    char dataFim[11];
} Historico;

typedef struct apresentador {
    char nome[50];
    char categoria[30];
    char streamAtual[50];

    Historico historico[20];
    int qtdHistorico;

    struct apresentador *ant;
    struct apresentador *prox;
} Apresentador;

// prototipos de func
void liberarProgramas(Programa *raiz);
void liberarCategorias(Categoria *lista);
Apresentador* buscarApresentador(Apresentador *lista, char nome[]);
Stream* buscarStream(Stream *raiz, char nome[]);
Categoria* buscarCategoria(Categoria *lista, char nome[]);
void buscarProgramasPorApresentador(Stream *raiz, char nomeApresentador[], int *encontrado);
void cadastrarStream(Stream **raiz, char *nome, char *site);
void mostrarStream(Stream *raiz);
Categoria *cadastrarCategoria(Categoria *categorias, char *nomeCategoria, char *tipoCategoria);
void mostrarCategorias(Categoria *categorias);
void mostrarProgramas(Programa *programas);
Apresentador *cadastrarApresentador(Apresentador *apresentadores, char *nomeApresentador, char *nomeCategoria, char *nomeStream);
void mostrarApresentadores(Apresentador *apresentadores);
void buscarStreamsPorCategoria(Stream *raiz, char *nomeCategoria);
void mostrarProgramasPorHorario(Stream *raiz,char horario[]);
Programa* cadastrarPrograma(Programa *raiz, char nome[], char periodicidade[], int duracao, char horario[], int aoVivo, char apresentador[], Apresentador *listaApresentadores, char nomeStream[], char nomeCategoria[]);
void buscarStreamPorTipoCategoria(Stream *raiz, char *tipoCategoria);
void mostrarApresentadoresPorStream(Apresentador *apresentadores, char *nomeStream);
void mostrarApresentadoresPorCategoria(Apresentador *apresentadores, char *nomeCategoria);
void mostrarDadosPrograma(Stream *raiz, char *nomeStream, char *nomeCategoria, char *nomePrograma);
Programa *removerPrograma(Programa *programas, char *nomePrograma);
void removerCategoria(Stream *raiz, char *nomeStream, char *nomeCategoria);
void alterarStreamApresentador(Stream *raiz, Apresentador *listaApresentadores, char *nomeApresentador, char *nomeStream);
void removerStream(Stream **raiz, char *nomeStream);
void mostrarProgramasPorDiaSemanaCategoria(Stream *raiz, char nomeCategoria[], char horario[]);
#endif