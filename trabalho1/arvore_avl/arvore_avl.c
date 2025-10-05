#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore_avl.h"


// -- James : FUNÇÕES AVL ---
int altura(Stream *raiz) {
    if (raiz == NULL) {
        return 0;
    } else {
        int ae = altura(raiz->esq);
        int ad = altura(raiz->dir);
        return (ae > ad ? ae : ad) + 1;
    }
}

void rotacao_dir(Stream **raiz) {
    if ((*raiz)->esq == NULL) {
        return;
    }
    Stream *aux = (*raiz)->esq;
    (*raiz)->esq = aux->dir;
    aux->dir = (*raiz);
    (*raiz)->altura = altura(*raiz);
    aux->altura = altura(aux);
    *raiz = aux;
}

void rotacao_esq(Stream **raiz) {
    if ((*raiz)->dir == NULL) {
        return;
    }
    Stream *aux = (*raiz)->dir;
    (*raiz)->dir = aux->esq;
    aux->esq = (*raiz);
    (*raiz)->altura = altura(*raiz);
    aux->altura = altura(aux);
    *raiz = aux;
}

int getAltura(Stream *raiz) {
    return raiz != NULL ? raiz->altura : 0;
}

void fatorBalanciamento(Stream **raiz) {
    int fb = getAltura((*raiz)->esq) - getAltura((*raiz)->dir);
    if (fb >= 2) {
        Stream *aux = (*raiz)->esq;
        int fb_aux = getAltura(aux->esq) - getAltura(aux->dir);
        if (fb_aux < 0) {
            rotacao_esq(&((*raiz)->esq));
        }
        rotacao_dir(raiz);
    } else if (fb <= -2) {
        Stream *aux = (*raiz)->dir;
        int fb_aux = getAltura(aux->esq) - getAltura(aux->dir);
        if (fb_aux > 0) {
            rotacao_dir(&((*raiz)->dir));
        }
        rotacao_esq(raiz);
    }
}

Stream* criarStream(char nome[], char site[]) {
    Stream *novo = (Stream*)malloc(sizeof(Stream));
    strcpy(novo->nome, nome);
    strcpy(novo->site, site);
    novo->categorias = NULL;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->altura = 1;
    return novo;
}

void cadastrarStream(Stream **raiz, char nome[], char site[]) {
    if (*raiz == NULL) {
        *raiz = criarStream(nome, site);
        return;
    } else {
        int cmp = strcmp(nome, (*raiz)->nome);
        if (cmp < 0) {
            cadastrarStream(&((*raiz)->esq), nome, site);
        } else if (cmp > 0) {
            cadastrarStream(&((*raiz)->dir), nome, site);
        } else {
            printf("Stream '%s' ja cadastrado!\n", nome);
            return;
        }
    }
    (*raiz)->altura = altura(*raiz);
    fatorBalanciamento(raiz);
}

Stream* buscarStream(Stream *raiz, char nome[]) {
    if (raiz == NULL) return NULL;
    int cmp = strcmp(nome, raiz->nome);
    if (cmp == 0) return raiz;
    else if (cmp < 0) return buscarStream(raiz->esq, nome);
    else return buscarStream(raiz->dir, nome);
}

void mostrarStream(Stream *raiz) {
    if (raiz != NULL) {
        mostrarStream(raiz->esq);
        printf("Stream: %s | Site: %s\n", raiz->nome, raiz->site);
        mostrarStream(raiz->dir);
    }
}



Stream* encontrarMinimo(Stream *raiz) {
    Stream *atual = raiz;
    while (atual && atual->esq != NULL) {
        atual = atual->esq;
    }
    return atual;
}

void removerStream(Stream **raiz, char nome[]) {
    if (*raiz == NULL) {
        printf("Stream não encontrada!\n");
        return;
    }
    int cmp = strcmp(nome, (*raiz)->nome);
    if (cmp < 0) {
        removerStream(&((*raiz)->esq), nome);
    } else if (cmp > 0) {
        removerStream(&((*raiz)->dir), nome);
    } else {
        if ((*raiz)->esq == NULL && (*raiz)->dir == NULL) {
            free(*raiz);
            *raiz = NULL;
        } else if ((*raiz)->esq == NULL) {
            Stream *aux = (*raiz)->dir;
            free(*raiz);
            *raiz = aux;
        } else if ((*raiz)->dir == NULL) {
            Stream *aux = (*raiz)->esq;
            free(*raiz);
            *raiz = aux;
        } else {
            Stream *aux = encontrarMinimo((*raiz)->dir);
            strcpy((*raiz)->nome, aux->nome);
            strcpy((*raiz)->site, aux->site);
            removerStream(&((*raiz)->dir), aux->nome);
        }
    }
    if (*raiz != NULL) {
        (*raiz)->altura = altura(*raiz);
        fatorBalanciamento(raiz);
    }
}

Categoria* criarCategoria(char nome[], char tipo[]) {
    Categoria *nova = (Categoria*)malloc(sizeof(Categoria));
    strcpy(nova->nome, nome);
    strcpy(nova->tipo, tipo);
    nova->programas = NULL;
    nova->prox = nova;
    return nova;
}

Categoria* cadastrarCategoria(Categoria *lista, char nome[], char tipo[]) {
    if (buscarCategoria(lista, nome) != NULL) {
        printf("Categoria '%s' ja existe!\n", nome);
        return lista;
    }
    Categoria *nova = criarCategoria(nome, tipo);
    if (lista == NULL) {
        return nova;
    }
    nova->prox = lista->prox;
    lista->prox = nova;
    return lista;
}

Categoria* buscarCategoria(Categoria *lista, char nome[]) {
    if (lista == NULL) return NULL;
    Categoria *atual = lista;
    do {
        if (strcmp(atual->nome, nome) == 0) return atual;
        atual = atual->prox;
    } while (atual != lista);
    return NULL;
}

void mostrarCategorias(Categoria *lista) {
    if (lista == NULL) {
        printf("Nenhuma categoria cadastrada.\n");
        return;
    }
    Categoria *atual = lista;
    do {
        printf("Categoria: %s | Tipo: %s\n", atual->nome, atual->tipo);
        atual = atual->prox;
    } while (atual != lista);
}

// Implementacao da funcao de remocao de categoria
void removerCategoria(Stream *s, char nomeCategoria[]) {
    if (s == NULL || s->categorias == NULL) {
        printf("Stream ou categoria nao encontrada.\n");
        return;
    }
    Categoria *atual = s->categorias;
    Categoria *anterior = NULL;
    
    // Procura a categoria e seu anterior
    do {
        if (strcmp(atual->nome, nomeCategoria) == 0) {
            if (atual->programas != NULL) {
                printf("Não é possível remover a categoria. Ela ainda contém programas.\n");
                return;
            }
            break;
        }
        anterior = atual;
        atual = atual->prox;
    } while (atual != s->categorias);

    if (strcmp(atual->nome, nomeCategoria) != 0) {
        printf("Categoria '%s' nao encontrada.\n", nomeCategoria);
        return;
    }

    if (atual == s->categorias && atual->prox == s->categorias) { // Unico elemento
        s->categorias = NULL;
    } else {
        if (atual == s->categorias) {
            Categoria *ultimo = s->categorias;
            while(ultimo->prox != s->categorias) {
                ultimo = ultimo->prox;
            }
            s->categorias = atual->prox;
            ultimo->prox = s->categorias;
        } else {
            anterior->prox = atual->prox;
        }
    }
    free(atual);
    printf("Categoria '%s' removida com sucesso.\n", nomeCategoria);
}

// Implementacao da funcao de busca de streams por nome de categoria
void buscarStreamPorNomeCategoria(Stream *raiz, char nomeCategoria[]) {
    if (raiz == NULL) {
        return;
    }
    buscarStreamPorNomeCategoria(raiz->esq, nomeCategoria);
    Categoria *cat = raiz->categorias;
    if (cat != NULL) {
        Categoria *atual = cat;
        do {
            if (strcmp(atual->nome, nomeCategoria) == 0) {
                printf("Stream: %s\n", raiz->nome);
                break;
            }
            atual = atual->prox;
        } while (atual != cat);
    }
    buscarStreamPorNomeCategoria(raiz->dir, nomeCategoria);
}

// ---Eu : FUNÇÕES ---
Programa* criarPrograma(char nome[], char periodicidade[], int duracao, char horario[], int aoVivo, char apresentador[]) {
    Programa *novo = (Programa*) malloc(sizeof(Programa));
    strcpy(novo->nome, nome);
    strcpy(novo->periodicidade, periodicidade);
    novo->duracao = duracao;
    strcpy(novo->horario, horario);
    novo->aoVivo = aoVivo;
    strcpy(novo->apresentador, apresentador);
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

Programa* cadastrarPrograma(Programa *raiz, char nome[], char periodicidade[], int duracao, char horario[], int aoVivo, char apresentador[]) {
    if (raiz == NULL) {
        return criarPrograma(nome, periodicidade, duracao, horario, aoVivo, apresentador);
    }
    int cmp = strcmp(nome, raiz->nome);
    if (cmp < 0) {
        raiz->esq = cadastrarPrograma(raiz->esq, nome, periodicidade, duracao, horario, aoVivo, apresentador);
    } else if (cmp > 0) {
        raiz->dir = cadastrarPrograma(raiz->dir, nome, periodicidade, duracao, horario, aoVivo, apresentador);
    } else {
        printf("Programa '%s' ja cadastrado!\n", nome);
    }
    return raiz;
}

Programa* buscarPrograma(Programa *raiz, char nome[]) {
    if (raiz == NULL) return NULL;
    int cmp = strcmp(nome, raiz->nome);
    if (cmp == 0) return raiz;
    else if (cmp < 0) return buscarPrograma(raiz->esq, nome);
    else return buscarPrograma(raiz->dir, nome);
}

Programa* encontrarMinimoPrograma(Programa *raiz) {
    while (raiz && raiz->esq != NULL) raiz = raiz->esq;
    return raiz;
}

void mostrarDadosPrograma(Programa *raiz, char nomePrograma[]) {
    Programa* programa = buscarPrograma(raiz, nomePrograma);
    if (programa != NULL) {
        printf("\n--- Dados do Programa: %s ---\n", programa->nome);
        printf("Periodicidade: %s\n", programa->periodicidade);
        printf("Duracao: %d minutos\n", programa->duracao);
        printf("Horario: %s\n", programa->horario);
        printf("Ao Vivo: %s\n", programa->aoVivo ? "Sim" : "Nao");
        printf("Apresentador: %s\n", programa->apresentador);
        printf("-------------------------------\n");
    } else {
        printf("Programa '%s' nao encontrado.\n", nomePrograma);
    }
}


Programa* removerPrograma(Programa* raiz, char nome[]) {
    if (raiz == NULL) {
        printf("Programa '%s' nao encontrado.\n", nome);
        return raiz;
    }
    int cmp = strcmp(nome, raiz->nome);
    if (cmp < 0) {
        raiz->esq = removerPrograma(raiz->esq, nome);
    } else if (cmp > 0) {
        raiz->dir = removerPrograma(raiz->dir, nome);
    } else {
        if (raiz->esq == NULL) {
            Programa *temp = raiz->dir;
            free(raiz);
            return temp;
        } else if (raiz->dir == NULL) {
            Programa *temp = raiz->esq;
            free(raiz);
            return temp;
        }
        Programa* temp = encontrarMinimoPrograma(raiz->dir);
        strcpy(raiz->nome, temp->nome);
        strcpy(raiz->periodicidade, temp->periodicidade);
        raiz->duracao = temp->duracao;
        strcpy(raiz->horario, temp->horario);
        raiz->aoVivo = temp->aoVivo;
        strcpy(raiz->apresentador, temp->apresentador);
        raiz->dir = removerPrograma(raiz->dir, temp->nome);
    }
    return raiz;
}

Apresentador* criarApresentador(char nome[], char categoria[], char streamAtual[]) {
    Apresentador *novo = (Apresentador*)malloc(sizeof(Apresentador));
    strcpy(novo->nome, nome);
    strcpy(novo->categoria, categoria);
    strcpy(novo->streamAtual, streamAtual);
    novo->qtdHistorico = 0;
    novo->ant = NULL;
    novo->prox = NULL;
    return novo;
}

Apresentador* cadastrarApresentador(Apresentador *lista, char nome[], char categoria[], char streamAtual[]) {
    Apresentador *novo = criarApresentador(nome, categoria, streamAtual);
    if (lista == NULL || strcmp(nome, lista->nome) < 0) {
        novo->prox = lista;
        if (lista != NULL) {
            lista->ant = novo;
        }
        return novo;
    }
    Apresentador *atual = lista;
    while (atual->prox != NULL && strcmp(nome, atual->prox->nome) > 0) {
        atual = atual->prox;
    }
    if (strcmp(nome, atual->nome) == 0) {
        printf("Apresentador '%s' ja cadastrado.\n", nome);
        free(novo);
        return lista;
    }
    novo->prox = atual->prox;
    if (atual->prox != NULL) {
        atual->prox->ant = novo;
    }
    novo->ant = atual;
    atual->prox = novo;
    return lista;
}

Apresentador* buscarApresentador(Apresentador *lista, char nome[]) {
    Apresentador *atual = lista;
    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

void alterarStreamApresentador(Apresentador *lista, char nomeApresentador[], char novaStream[]) {
    Apresentador *apresentador = buscarApresentador(lista, nomeApresentador);
    if (apresentador != NULL) {
        if (apresentador->qtdHistorico < 20) {
            strcpy(apresentador->historico[apresentador->qtdHistorico].nomeStream, apresentador->streamAtual);
            apresentador->qtdHistorico++;
        }
        strcpy(apresentador->streamAtual, novaStream);
        printf("Stream de '%s' alterada para '%s'.\n", nomeApresentador, novaStream);
    } else {
        printf("Apresentador '%s' nao encontrado.\n", nomeApresentador);
    }
}

void mostrarProgramas(Programa *raiz) {
    if (raiz != NULL) {
        mostrarProgramas(raiz->esq);
        printf("   - Programa: %s | Periodicidade: %s | Horario: %s | Ao Vivo: %s | Apresentador: %s\n",
               raiz->nome, raiz->periodicidade, raiz->horario, raiz->aoVivo ? "Sim" : "Nao", raiz->apresentador);
        mostrarProgramas(raiz->dir);
    }
}

void mostrarApresentadores(Apresentador *lista) {
    if (lista == NULL) {
        printf("Nenhum apresentador cadastrado.\n");
        return;
    }
    Apresentador *atual = lista;
    while (atual != NULL) {
        printf("Apresentador: %s | Categoria: %s | Stream Atual: %s\n",
               atual->nome, atual->categoria, atual->streamAtual);
        atual = atual->prox;
    }
}

// Funcao de Bsucar stream por categoria
void buscarStreamPorTipoCategoria(Stream *raiz, const char tipo[]) {
    if (raiz == NULL) {
        return;
    }
    buscarStreamPorTipoCategoria(raiz->esq, tipo);
    Categoria *cat = raiz->categorias;
    if (cat != NULL) {
        do {
            if (strcmp(cat->tipo, tipo) == 0) {
                printf("Stream: %s\n", raiz->nome);
                break; // Apenas a primeira ocorrência é suficiente.
            }
            cat = cat->prox;
        } while (cat != raiz->categorias);
    }
    buscarStreamPorTipoCategoria(raiz->dir, tipo);
}
// Funcao para buscar programas por dia e horario
void buscarProgramasPorDiaHorario(Programa *raiz, char periodicidade[], char horario[]) {
    if (raiz == NULL) {
        return;
    }
    buscarProgramasPorDiaHorario(raiz->esq, periodicidade, horario);
    if (strcmp(raiz->periodicidade, periodicidade) == 0 && strcmp(raiz->horario, horario) == 0) {
        printf("   - Programa: %s | Apresentador: %s\n", raiz->nome, raiz->apresentador);
    }
    buscarProgramasPorDiaHorario(raiz->dir, periodicidade, horario);
}

// Funcao para buscar programas por dia da semana
void buscarProgramasPorDiaSemana(Programa *raiz, char periodicidade[]) {
    if (raiz == NULL) {
        return;
    }
    buscarProgramasPorDiaSemana(raiz->esq, periodicidade);
    if (strcmp(raiz->periodicidade, periodicidade) == 0) {
        printf("   - Programa: %s | Horario: %s | Apresentador: %s\n", raiz->nome, raiz->horario, raiz->apresentador);
    }
    buscarProgramasPorDiaSemana(raiz->dir, periodicidade);
}

// Funcao para mostrar todos os apresentadores de uma determinada stream
void mostrarApresentadoresPorStream(Apresentador *lista, char nomeStream[]) {
    if (lista == NULL) {
        printf("Nenhum apresentador cadastrado.\n");
        return;
    }
    int encontrado = 0;
    Apresentador *atual = lista;
    while (atual != NULL) {
        if (strcmp(atual->streamAtual, nomeStream) == 0) {
            printf("Apresentador: %s | Categoria: %s\n", atual->nome, atual->categoria);
            encontrado = 1;
        }
        atual = atual->prox;
    }
    if (!encontrado) {
        printf("Nenhum apresentador encontrado para a stream '%s'.\n", nomeStream);
    }
}

// Funcao para mostrar todos os apresentadores de uma determinada categoria
void mostrarApresentadoresPorCategoria(Apresentador *lista, char nomeCategoria[]) {
    if (lista == NULL) {
        printf("Nenhum apresentador cadastrado.\n");
        return;
    }
    int encontrado = 0;
    Apresentador *atual = lista;
    while (atual != NULL) {
        if (strcmp(atual->categoria, nomeCategoria) == 0) {
            printf("Apresentador: %s | Stream Atual: %s\n", atual->nome, atual->streamAtual);
            encontrado = 1;
        }
        atual = atual->prox;
    }
    if (!encontrado) {
        printf("Nenhum apresentador encontrado para a categoria '%s'.\n", nomeCategoria);
    }
}




