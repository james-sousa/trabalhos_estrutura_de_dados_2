#ifndef ARVORE_AVL_H
#define ARVORE_AVL_H



// --- James ESTRUTURAS ---
typedef struct categorias {
    char tipo[30];
    char nome[30];
    struct programa *programas;
    struct categorias *prox;
} Categoria;

typedef struct Stream {
    char nome[50];
    char site[100];
    int altura;
    Categoria *categorias;
    struct Stream *esq;
    struct Stream *dir;
} Stream;

// --- Eu  ESTRUTURAS ---
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

// declaracao de prototipo
Categoria* buscarCategoria(Categoria *lista, char nome[]);
void mostrarDadosPrograma(Programa *raiz, char nomePrograma[]);
Categoria* buscarCategoria(Categoria *lista, char nome[]);
Apresentador* buscarApresentador(Apresentador *lista, char nome[]);
void mostrarProgramas(Programa *raiz);
void mostrarApresentadores(Apresentador *lista);
void buscarStreamPorTipoCategoria(Stream *raiz, const char tipo[]);
void buscarProgramasPorDiaHorario(Programa *raiz, char periodicidade[], char horario[]);
void buscarProgramasPorDiaSemana(Programa *raiz, char periodicidade[]);
void mostrarProgramasCategoriaStream(Stream *s, char nomeCategoria[]);
void mostrarApresentadoresPorCategoria(Apresentador *lista, char nomeCategoria[]);
void mostrarApresentadoresPorStream(Apresentador *lista, char nomeStream[]);
void removerCategoria(Stream *s, char nomeCategoria[]);
// Funções que estavam faltando
void cadastrarStream(Stream **raiz, char nome[], char site[]);
void mostrarStream(Stream *raiz);
Stream* buscarStream(Stream *raiz, char nome[]);
void removerStream(Stream **raiz, char nome[]);
Categoria* cadastrarCategoria(Categoria *lista, char nome[], char tipo[]);
void mostrarCategorias(Categoria *lista);
Programa* cadastrarPrograma(Programa *raiz, char nome[], char periodicidade[], int duracao, char horario[], int aoVivo, char apresentador[]);
Programa* removerPrograma(Programa *raiz, char nome[]);
Apresentador* cadastrarApresentador(Apresentador *lista, char nomeApresentador[], char nomeCategoria[], char nomeStream[]);
void alterarStreamApresentador(Apresentador *lista, char nomeApresentador[], char novaStream[]);
void buscarStreamPorNomeCategoria(Stream *raiz, char nomeCategoria[]);

#endif