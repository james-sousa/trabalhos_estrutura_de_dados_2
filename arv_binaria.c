#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


// Parte de James -  FUNÇÕES ---
Stream* criarStream(char nome[], char site[]) {
    Stream *novo = (Stream*)malloc(sizeof(Stream));
    strcpy(novo->nome, nome);
    strcpy(novo->site, site);
    novo->categorias = NULL;
    novo->esq = NULL;
    novo->dir = NULL;
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
        }
    }
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

Stream* maiorNo(Stream *raiz) {
    while (raiz->dir != NULL) {
        raiz = raiz->dir;
    }
    return raiz;
}


// ATUALIZADA p/ evitar vazamento
void removerStream(Stream **raiz, char nome[]) {
    if (*raiz == NULL) {
        printf("Stream '%s' nao encontrada.\n", nome);
    } else {
        int cmp = strcmp(nome, (*raiz)->nome);
        if (cmp < 0) {
            removerStream(&((*raiz)->esq), nome);
        } else if (cmp > 0) {
            removerStream(&((*raiz)->dir), nome);
        } else {
            // Libera as categorias e programas antes de remover a stream
            liberarCategorias((*raiz)->categorias);

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
                Stream *aux = maiorNo((*raiz)->esq);
                strcpy((*raiz)->nome, aux->nome);
                strcpy((*raiz)->site, aux->site);
                removerStream(&((*raiz)->esq), aux->nome);
            }
            printf("Stream '%s' removida com sucesso!\n", nome);
        }
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
    Categoria *nova = criarCategoria(nome, tipo);
    if (lista == NULL) {
        return nova;
    }
    Categoria *atual = lista;
    do {
        if (strcmp(atual->nome, nome) == 0) {
            printf("Categoria '%s' ja existe!\n", nome);
            free(nova);
            return lista;
        }
        atual = atual->prox;
    } while (atual != lista);
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

// - programas FUNÇÕES -
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

Programa* cadastrarPrograma(Programa *raiz, char nome[], char periodicidade[], int duracao, char horario[], int aoVivo, char apresentador[], Apresentador *listaApresentadores, char nomeStream[], char nomeCategoria[]) {
    Apresentador *apresentadorExiste = buscarApresentador(listaApresentadores, apresentador);
    if (apresentadorExiste == NULL) {
        printf("Erro: Apresentador '%s' nao cadastrado. Por favor, cadastre-o primeiro.\n", apresentador);
        return raiz;
    }
    if (strcmp(apresentadorExiste->streamAtual, nomeStream) != 0 || strcmp(apresentadorExiste->categoria, nomeCategoria) != 0) {
        printf("Erro: Apresentador '%s' nao trabalha na stream '%s' com a categoria '%s'.\n", apresentador, nomeStream, nomeCategoria);
        return raiz;
    }

    if (raiz == NULL) {
        return criarPrograma(nome, periodicidade, duracao, horario, aoVivo, apresentador);
    }
    int cmp = strcmp(nome, raiz->nome);
    if (cmp < 0) {
        raiz->esq = cadastrarPrograma(raiz->esq, nome, periodicidade, duracao, horario, aoVivo, apresentador, listaApresentadores, nomeStream, nomeCategoria);
    } else if (cmp > 0) {
        raiz->dir = cadastrarPrograma(raiz->dir, nome, periodicidade, duracao, horario, aoVivo, apresentador, listaApresentadores, nomeStream, nomeCategoria);
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
// Funcao para liberar a arvore de programas
void liberarProgramas(Programa *raiz) {
    if (raiz == NULL) return;
    liberarProgramas(raiz->esq);
    liberarProgramas(raiz->dir);
    free(raiz);
}

// Funcao para liberar a lista de categorias de forma segura
void liberarCategorias(Categoria *lista) {
    if (lista == NULL) return;
    Categoria *atual = lista->prox;
    while (atual != lista) {
        Categoria *temp = atual;
        atual = atual->prox;
        liberarProgramas(temp->programas);
        free(temp);
    }
    liberarProgramas(lista->programas);
    free(lista);
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
    // Verificação para se o nome ser o mesmo do nó atual 
    if (strcmp(nome, atual->nome) == 0 || (atual->prox != NULL && strcmp(nome, atual->prox->nome) == 0)) {
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

// 
void buscarProgramasPorApresentador(Stream *raiz, char nomeApresentador[], int *encontrado) {
    if (raiz == NULL || *encontrado) return;

    // Percorre a árvore de streams
    buscarProgramasPorApresentador(raiz->esq, nomeApresentador, encontrado);
    if (*encontrado) return;

    // Verifica as categorias e programas da stream atual
    Categoria *cat = raiz->categorias;
    if (cat != NULL) {
        do {
            // Percorre a árvore de programas de cada categoria
            Programa* buscarEmProgramas(Programa* p, char nomeA[]){
                if (p == NULL || *encontrado) return NULL;
                if (strcmp(p->apresentador, nomeA) == 0) {
                    *encontrado = 1;
                    return p; // Programa encontrado.
                }
                buscarEmProgramas(p->esq, nomeA);
                if (*encontrado) return p;
                buscarEmProgramas(p->dir, nomeA);
                return NULL;
            }
            buscarEmProgramas(cat->programas, nomeApresentador);
            if (*encontrado) return;
            cat = cat->prox;
        } while (cat != raiz->categorias);
    }
    buscarProgramasPorApresentador(raiz->dir, nomeApresentador, encontrado);
}

// 
void alterarStreamApresentador(Stream *raizStreams, Apresentador *listaApresentadores, char nomeApresentador[], char novaStream[]) {
    Apresentador *apresentador = buscarApresentador(listaApresentadores, nomeApresentador);
    if (apresentador == NULL) {
        printf("Apresentador '%s' nao encontrado.\n", nomeApresentador);
        return;
    }

    int temProgramaAtivo = 0;
    buscarProgramasPorApresentador(raizStreams, apresentador->streamAtual, &temProgramaAtivo);

    if (temProgramaAtivo) {
        printf("Erro: Não é possível alterar a stream. O apresentador ainda possui programas ativos na stream atual.\n");
    } else {
        if (apresentador->qtdHistorico < 20) {
            strcpy(apresentador->historico[apresentador->qtdHistorico].nomeStream, apresentador->streamAtual);
            apresentador->qtdHistorico++;
        }
        strcpy(apresentador->streamAtual, novaStream);
        printf("Stream de '%s' alterada para '%s' com sucesso.\n", nomeApresentador, novaStream);
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

// --- Novas Funcoes ---
void buscarStreamsPorCategoria(Stream *raiz, char nomeCategoria[]) { // Item viii
    if (raiz == NULL) return;
    buscarStreamsPorCategoria(raiz->esq, nomeCategoria);
    Categoria *cat = raiz->categorias;
    if (cat != NULL) {
        do {
            if (strcmp(cat->nome, nomeCategoria) == 0) {
                printf("Stream: %s\n", raiz->nome);
                break;
            }
            cat = cat->prox;
        } while (cat != raiz->categorias);
    }
    buscarStreamsPorCategoria(raiz->dir, nomeCategoria);
}

void mostrarProgramasPorHorario(Stream *raiz,char horario[]) { // Item ix
    if (raiz == NULL) return;
    mostrarProgramasPorHorario(raiz->esq, horario);
    Categoria *cat = raiz->categorias;
    if (cat != NULL) {
        do {
            Programa *prog = cat->programas;
            Programa* buscarEmostrar(Programa* p, char h[]) {
                if (p == NULL) return NULL;
                buscarEmostrar(p->esq, h);
                if (strcmp(p->horario, h) == 0) {
                    printf("  - Programa: %s | Categoria: %s | Stream: %s\n", p->nome, cat->nome, raiz->nome);
                }
                buscarEmostrar(p->dir, h);
                return NULL;
            }
            buscarEmostrar(prog, horario);
            cat = cat->prox;
        } while (cat != raiz->categorias);
    }
    mostrarProgramasPorHorario(raiz->dir, horario);
}

void buscarStreamPorTipoCategoria(Stream *raiz,char tipo[]) { // Item x
    if (raiz == NULL) {
        return;
    }
    buscarStreamPorTipoCategoria(raiz->esq, tipo);
    Categoria *cat = raiz->categorias;
    if (cat != NULL) {
        do {
            if (strcmp(cat->tipo, tipo) == 0) {
                printf("Stream: %s\n", raiz->nome);
                break;
            }
            cat = cat->prox;
        } while (cat != raiz->categorias);
    }
    buscarStreamPorTipoCategoria(raiz->dir, tipo);
}

void mostrarProgramasPorDiaSemanaCategoria(Stream *raiz, char nomeCategoria[], char horario[]) { // Item xi
    if (raiz == NULL) return;
    mostrarProgramasPorDiaSemanaCategoria(raiz->esq, nomeCategoria, horario);
    Categoria *cat = raiz->categorias;
    if (cat != NULL) {
        do {
            if (strcmp(cat->nome, nomeCategoria) == 0) {
                Programa *prog = cat->programas;
                Programa* buscarEmostrar(Programa* p, char h[]){
                    if (p == NULL) return NULL;
                    buscarEmostrar(p->esq, h);
                    if (strcmp(p->horario, h) == 0) {
                        printf("  - Programa: %s | Periodicidade: %s | Horario: %s\n", p->nome, p->periodicidade, p->horario);
                    }
                    buscarEmostrar(p->dir, h);
                    return NULL;
                }
                printf("\nProgramas da categoria '%s' na stream '%s' que ocorrem as %s:\n", nomeCategoria, raiz->nome, horario);
                buscarEmostrar(prog, horario);
            }
            cat = cat->prox;
        } while (cat != raiz->categorias);
    }
    mostrarProgramasPorDiaSemanaCategoria(raiz->dir, nomeCategoria, horario);
}

void mostrarApresentadoresPorStream(Apresentador *lista,char nomeStream[]) { // Item xii
    Apresentador *atual = lista;
    int encontrado = 0;
    while (atual != NULL) {
        if (strcmp(atual->streamAtual, nomeStream) == 0) {
            printf(" - %s\n", atual->nome);
            encontrado = 1;
        }
        atual = atual->prox;
    }
    if (!encontrado) {
        printf("Nenhum apresentador encontrado para a stream '%s'.\n", nomeStream);
    }
}

void mostrarApresentadoresPorCategoria(Apresentador *lista, char nomeCategoria[]) { // Item xiii
    Apresentador *atual = lista;
    int encontrado = 0;
    while (atual != NULL) {
        if (strcmp(atual->categoria, nomeCategoria) == 0) {
            printf(" - %s (Trabalha na stream '%s')\n", atual->nome, atual->streamAtual);
            encontrado = 1;
        }
        atual = atual->prox;
    }
    if (!encontrado) {
        printf("Nenhum apresentador encontrado para a categoria '%s'.\n", nomeCategoria);
    }
}

// atualizada p/ verif prrograms asssociados antes de remover
void removerCategoria(Stream *raiz, char nomeStream[], char nomeCategoria[]) { // Item xvi
    Stream *s = buscarStream(raiz, nomeStream);
    if (s == NULL) {
        printf("Erro: Stream '%s' nao encontrada.\n", nomeStream);
        return;
    }
    Categoria *atual = s->categorias;
    Categoria *anterior = NULL;
    if (atual == NULL) {
        printf("Erro: Categoria '%s' nao encontrada.\n", nomeCategoria);
        return;
    }
    do {
        if (strcmp(atual->nome, nomeCategoria) == 0) {
            if (atual->programas != NULL) {
                printf("Erro: Nao eh possivel remover a categoria. Ainda ha programas cadastrados nela.\n");
                return;
            }
            if (anterior == NULL) {
                if (atual->prox == atual) {
                    s->categorias = NULL;
                } else {
                    s->categorias = atual->prox;
                    anterior = s->categorias;
                    while (anterior->prox != atual) {
                        anterior = anterior->prox;
                    }
                    anterior->prox = s->categorias;
                }
            } else {
                anterior->prox = atual->prox;
            }
            free(atual);
            printf("Categoria '%s' removida com sucesso!\n", nomeCategoria);
            return;
        }
        anterior = atual;
        atual = atual->prox;
    } while (atual != s->categorias);
    printf("Erro: Categoria '%s' nao encontrada.\n", nomeCategoria);
}

// 
void mostrarDadosPrograma(Stream *raiz, char nomeStream[], char nomeCategoria[], char nomePrograma[]) { // Item xiv
    Stream *s = buscarStream(raiz, nomeStream);
    if (s == NULL) {
        printf("Erro: Stream '%s' nao encontrada.\n", nomeStream);
        return;
    }
    Categoria *cat = buscarCategoria(s->categorias, nomeCategoria);
    if (cat == NULL) {
        printf("Erro: Categoria '%s' nao encontrada na stream.\n", nomeCategoria);
        return;
    }
    Programa *prog = buscarPrograma(cat->programas, nomePrograma);
    if (prog == NULL) {
        printf("Erro: Programa '%s' nao encontrado na categoria.\n", nomePrograma);
        return;
    }
    printf("\nDados do Programa '%s':\n", prog->nome);
    printf("  Periodicidade: %s\n", prog->periodicidade);
    printf("  Duracao: %d minutos\n", prog->duracao);
    printf("  Horario de inicio: %s\n", prog->horario);
    printf("  Ao vivo: %s\n", prog->aoVivo ? "Sim" : "Nao");
    printf("  Apresentador: %s\n", prog->apresentador);
}

/// MENU ATUALIZADO
int main() {
    Stream *raiz = NULL;
    Apresentador *listaApresentadores = NULL;
    int opcao;
    char nome[50], site[100];
    char nomeCategoria[30], tipoCategoria[30];
    char nomePrograma[30], periodicidade[20], horario[20], nomeApresentador[50];
    int duracao, aoVivo;
    Stream *s;
    Categoria *cat;
    
    do {
        printf("\n===== MENU STREAMING =====\n");
        printf("1. Cadastrar Stream\n");
        printf("2. Mostrar todas as streams cadastradas\n");
        printf("3. Cadastrar Categoria para uma stream\n");
        printf("4. Mostrar categorias de uma stream\n");
        printf("5. Cadastrar Programa para uma categoria\n");
        printf("6. Mostrar programas de uma categoria\n");
        printf("7. Cadastrar Apresentador\n");
        printf("8. Mostrar todos os apresentadores\n");
        printf("9. Mostrar streams com uma categoria especifica\n");
        printf("10. Mostrar programas de uma stream por dia e horario\n");
        printf("11. Mostrar streams por tipo de categoria\n");
        printf("12. Mostrar programas de uma categoria por dia da semana e horario\n");
        printf("13. Mostrar apresentadores de uma stream\n");
        printf("14. Mostrar apresentadores de uma categoria\n");
        printf("15. Mostrar dados de um programa\n");
        printf("16. Remover um programa\n");
        printf("17. Remover uma categoria\n");
        printf("18. Alterar stream de um apresentador\n");
        printf("19. Remover Stream\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                printf("Digite o nome da Stream: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                printf("Digite o site da Stream: ");
                fgets(site, 100, stdin);
                site[strcspn(site, "\n")] = 0;
                cadastrarStream(&raiz, nome, site);
                break;

            case 2:
                printf("\n=== Streams cadastradas (ordem alfabetica) ===\n");
                mostrarStream(raiz);
                break;

            case 3:
                printf("Digite o nome da Stream: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                s = buscarStream(raiz, nome);
                if (s != NULL) {
                    printf("Digite o nome da categoria: ");
                    fgets(nomeCategoria, 30, stdin);
                    nomeCategoria[strcspn(nomeCategoria, "\n")] = 0;
                    printf("Digite o tipo da categoria: ");
                    fgets(tipoCategoria, 30, stdin);
                    tipoCategoria[strcspn(tipoCategoria, "\n")] = 0;
                    s->categorias = cadastrarCategoria(s->categorias, nomeCategoria, tipoCategoria);
                } else {
                    printf("Erro: Stream nao encontrada!\n");
                }
                break;

            case 4:
                printf("Digite o nome da Stream: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                s = buscarStream(raiz, nome);
                if (s != NULL) {
                    printf("\nCategorias da Stream %s:\n", s->nome);
                    mostrarCategorias(s->categorias);
                } else {
                    printf("Erro: Stream nao encontrada!\n");
                }
                break;
            
            case 5:
                printf("Digite o nome da Stream: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                s = buscarStream(raiz, nome);
                if (s != NULL) {
                    printf("Digite o nome da Categoria: ");
                    fgets(nomeCategoria, 30, stdin);
                    nomeCategoria[strcspn(nomeCategoria, "\n")] = 0;
                    cat = buscarCategoria(s->categorias, nomeCategoria);
                    if (cat != NULL) {
                        printf("Digite o nome do Programa: ");
                        fgets(nomePrograma, 30, stdin);
                        nomePrograma[strcspn(nomePrograma, "\n")] = 0;
                        printf("Digite a periodicidade (diario, semanal, mensal): ");
                        fgets(periodicidade, 20, stdin);
                        periodicidade[strcspn(periodicidade, "\n")] = 0;
                        printf("Digite a duracao em minutos: ");
                        scanf("%d", &duracao);
                        getchar();
                        printf("Digite o horario de inicio (HH:MM): ");
                        fgets(horario, 20, stdin);
                        horario[strcspn(horario, "\n")] = 0;
                        printf("E ao vivo? (1-Sim, 0-Nao): ");
                        scanf("%d", &aoVivo);
                        getchar();
                        printf("Digite o nome do apresentador: ");
                        fgets(nomeApresentador, 50, stdin);
                        nomeApresentador[strcspn(nomeApresentador, "\n")] = 0;
                        
                        if (buscarApresentador(listaApresentadores, nomeApresentador) == NULL) {
                            printf("Erro: Apresentador nao cadastrado. Por favor, cadastre-o primeiro.\n");
                        } else {
                            cat->programas = cadastrarPrograma(cat->programas, nomePrograma, periodicidade, duracao, horario, aoVivo, nomeApresentador, listaApresentadores, nome, nomeCategoria);
                        }
                    } else {
                        printf("Erro: Categoria nao encontrada!\n");
                    }
                } else {
                    printf("Erro: Stream nao encontrada!\n");
                }
                break;

            case 6:
                printf("Digite o nome da Stream: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                s = buscarStream(raiz, nome);
                if (s != NULL) {
                    printf("Digite o nome da Categoria: ");
                    fgets(nomeCategoria, 30, stdin);
                    nomeCategoria[strcspn(nomeCategoria, "\n")] = 0;
                    cat = buscarCategoria(s->categorias, nomeCategoria);
                    if (cat != NULL) {
                        printf("\nProgramas da categoria '%s' da stream '%s':\n", nomeCategoria, nome);
                        mostrarProgramas(cat->programas);
                    } else {
                        printf("Erro: Categoria nao encontrada!\n");
                    }
                } else {
                    printf("Erro: Stream nao encontrada!\n");
                }
                break;
            
            case 7:
                printf("Digite o nome do Apresentador: ");
                fgets(nomeApresentador, 50, stdin);
                nomeApresentador[strcspn(nomeApresentador, "\n")] = 0;
                printf("Digite a categoria que ele trabalha: ");
                fgets(nomeCategoria, 30, stdin);
                nomeCategoria[strcspn(nomeCategoria, "\n")] = 0;
                printf("Digite a stream que ele trabalha atualmente: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                if (buscarStream(raiz, nome) != NULL) {
                    listaApresentadores = cadastrarApresentador(listaApresentadores, nomeApresentador, nomeCategoria, nome);
                } else {
                    printf("Erro: Stream '%s' nao cadastrada. Nao foi possivel cadastrar o apresentador.\n", nome);
                }
                break;

            case 8:
                printf("\n=== Apresentadores Cadastrados ===\n");
                mostrarApresentadores(listaApresentadores);
                break;

            case 9:
                printf("Digite o nome da categoria: ");
                fgets(nomeCategoria, 30, stdin);
                nomeCategoria[strcspn(nomeCategoria, "\n")] = 0;
                printf("\nStreams que contem a categoria '%s':\n", nomeCategoria);
                buscarStreamsPorCategoria(raiz, nomeCategoria);
                break;

            case 10:
                printf("Digite o horario de inicio (ex: 20:00): ");
                fgets(horario, 20, stdin);
                horario[strcspn(horario, "\n")] = 0;
                printf("\nProgramas que ocorrem as %s:\n", horario);
                mostrarProgramasPorHorario(raiz, horario);
                break;
            
            case 11:
                printf("Digite o tipo de categoria (ex: Noticias, Esporte): ");
                fgets(tipoCategoria, 30, stdin);
                tipoCategoria[strcspn(tipoCategoria, "\n")] = 0;
                printf("\nStreams com categoria do tipo '%s':\n", tipoCategoria);
                buscarStreamPorTipoCategoria(raiz, tipoCategoria);
                break;

            case 12:
                printf("Digite o nome da categoria: ");
                fgets(nomeCategoria, 30, stdin);
                nomeCategoria[strcspn(nomeCategoria, "\n")] = 0;
                printf("Digite o horario de inicio (ex: 20:00): ");
                fgets(horario, 20, stdin);
                horario[strcspn(horario, "\n")] = 0;
                mostrarProgramasPorDiaSemanaCategoria(raiz, nomeCategoria, horario);
                break;
            
            case 13:
                printf("Digite o nome da Stream: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                printf("\nApresentadores que trabalham na stream '%s':\n", nome);
                mostrarApresentadoresPorStream(listaApresentadores, nome);
                break;

            case 14:
                printf("Digite o nome da Categoria: ");
                fgets(nomeCategoria, 30, stdin);
                nomeCategoria[strcspn(nomeCategoria, "\n")] = 0;
                printf("\nApresentadores que trabalham na categoria '%s':\n", nomeCategoria);
                mostrarApresentadoresPorCategoria(listaApresentadores, nomeCategoria);
                break;
            
            case 15:
                printf("Digite o nome da Stream: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                printf("Digite o nome da Categoria: ");
                fgets(nomeCategoria, 30, stdin);
                nomeCategoria[strcspn(nomeCategoria, "\n")] = 0;
                printf("Digite o nome do Programa: ");
                fgets(nomePrograma, 30, stdin);
                nomePrograma[strcspn(nomePrograma, "\n")] = 0;
                mostrarDadosPrograma(raiz, nome, nomeCategoria, nomePrograma);
                break;
            
            case 16:
                printf("Digite o nome da Stream: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                s = buscarStream(raiz, nome);
                if (s != NULL) {
                    printf("Digite o nome da Categoria: ");
                    fgets(nomeCategoria, 30, stdin);
                    nomeCategoria[strcspn(nomeCategoria, "\n")] = 0;
                    cat = buscarCategoria(s->categorias, nomeCategoria);
                    if (cat != NULL) {
                        printf("Digite o nome do Programa a ser removido: ");
                        fgets(nomePrograma, 30, stdin);
                        nomePrograma[strcspn(nomePrograma, "\n")] = 0;
                        cat->programas = removerPrograma(cat->programas, nomePrograma);
                    } else {
                        printf("Erro: Categoria nao encontrada!\n");
                    }
                } else {
                    printf("Erro: Stream nao encontrada!\n");
                }
                break;

            case 17:
                printf("Digite o nome da Stream: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                printf("Digite o nome da Categoria a ser removida: ");
                fgets(nomeCategoria, 30, stdin);
                nomeCategoria[strcspn(nomeCategoria, "\n")] = 0;
                removerCategoria(raiz, nome, nomeCategoria);
                break;

            case 18:
                printf("Digite o nome do apresentador: ");
                fgets(nomeApresentador, 50, stdin);
                nomeApresentador[strcspn(nomeApresentador, "\n")] = 0;
                printf("Digite o nome da nova stream que ele trabalha: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                if (buscarStream(raiz, nome) != NULL) {
                    alterarStreamApresentador(raiz, listaApresentadores, nomeApresentador, nome);
                } else {
                    printf("Erro: Nova stream '%s' nao encontrada. Nao foi possivel alterar.\n", nome);
                }
                break;

            case 19:
                printf("Digite o nome da Stream para remover: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                removerStream(&raiz, nome);
                break;
            
            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}