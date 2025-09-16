#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// -- MAIN ---
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
        printf("2. Mostrar Streams\n");
        printf("3. Buscar Stream\n");
        printf("4. Remover Stream\n");
        printf("5. Cadastrar Categoria em uma Stream\n");
        printf("6. Mostrar Categorias de uma Stream\n");
        printf("7. Cadastrar Programa em uma Categoria\n");
        printf("8. Mostrar Programas de uma Categoria\n");
        printf("9. Remover Programa\n");
        printf("10. Cadastrar Apresentador\n");
        printf("11. Mostrar Apresentadores\n");
        printf("12. Alterar Stream de Apresentador\n");
        printf("13. Mostrar Streams por Tipo de Categoria\n");
        printf("14. Mostrar Streams por Nome de Categoria\n"); // Novo item
        printf("15. Mostrar Programas por Dia e Horario\n");    // Novo item
        printf("16. Mostrar Programas por Dia da Semana\n");      // Novo item
        printf("17. Mostrar Apresentadores por Stream\n");      // Novo item
        printf("18. Mostrar Apresentadores por Categoria\n");  // Novo item
        printf("19. Mostrar Dados de um Programa\n");           // Novo item
        printf("20. Remover Categoria\n");                       // Novo item
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
                printf("Digite o nome da Stream para buscar: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                s = buscarStream(raiz, nome);
                if (s != NULL) {
                    printf("Encontrada: %s | Site: %s\n", s->nome, s->site);
                } else {
                    printf("Stream '%s' nao encontrada!\n", nome);
                }
                break;

            case 4:
                printf("Digite o nome da Stream para remover: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                removerStream(&raiz, nome);
                break;

            case 5:
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
                    printf("Stream nao encontrada!\n");
                }
                break;

            case 6:
                printf("Digite o nome da Stream: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                s = buscarStream(raiz, nome);
                if (s != NULL) {
                    printf("\nCategorias da Stream %s:\n", s->nome);
                    mostrarCategorias(s->categorias);
                } else {
                    printf("Stream nao encontrada!\n");
                }
                break;
            
            case 7:
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
                        printf("Digite a periodicidade: ");
                        fgets(periodicidade, 20, stdin);
                        periodicidade[strcspn(periodicidade, "\n")] = 0;
                        printf("Digite a duracao em minutos: ");
                        scanf("%d", &duracao);
                        getchar();
                        printf("Digite o horario de inicio: ");
                        fgets(horario, 20, stdin);
                        horario[strcspn(horario, "\n")] = 0;
                        printf("E ao vivo? (1-Sim, 0-Nao): ");
                        scanf("%d", &aoVivo);
                        getchar();
                        printf("Digite o nome do apresentador: ");
                        fgets(nomeApresentador, 50, stdin);
                        nomeApresentador[strcspn(nomeApresentador, "\n")] = 0;
                        
                        Apresentador *apresentador = buscarApresentador(listaApresentadores, nomeApresentador);
                        if (apresentador == NULL) {
                            printf("Apresentador nao cadastrado. Por favor, cadastre-o primeiro.\n");
                        } else if (strcmp(apresentador->categoria, nomeCategoria) != 0) {
                            printf("Apresentador trabalha em outra categoria ('%s'). Nao foi possivel cadastrar o programa.\n", apresentador->categoria);
                        } else if (strcmp(apresentador->streamAtual, nome) != 0) {
                            printf("Apresentador trabalha em outra stream ('%s'). Nao foi possivel cadastrar o programa.\n", apresentador->streamAtual);
                        } else {
                            cat->programas = cadastrarPrograma(cat->programas, nomePrograma, periodicidade, duracao, horario, aoVivo, nomeApresentador);
                        }
                    } else {
                        printf("Categoria nao encontrada!\n");
                    }
                } else {
                    printf("Stream nao encontrada!\n");
                }
                break;

            case 8:
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
                        printf("Categoria nao encontrada!\n");
                    }
                } else {
                    printf("Stream nao encontrada!\n");
                }
                break;
            
            case 9:
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
                        printf("Categoria nao encontrada!\n");
                    }
                } else {
                    printf("Stream nao encontrada!\n");
                }
                break;

            case 10:
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
                    printf("Stream '%s' nao cadastrada. Nao foi possivel cadastrar o apresentador.\n", nome);
                }
                break;

            case 11:
                printf("\n=== Apresentadores Cadastrados ===\n");
                mostrarApresentadores(listaApresentadores);
                break;

            case 12:
                printf("Digite o nome do apresentador: ");
                fgets(nomeApresentador, 50, stdin);
                nomeApresentador[strcspn(nomeApresentador, "\n")] = 0;
                printf("Digite o nome da nova stream que ele trabalha: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                if (buscarStream(raiz, nome) != NULL) {
                    alterarStreamApresentador(listaApresentadores, nomeApresentador, nome);
                } else {
                    printf("Nova stream '%s' nao encontrada. Nao foi possivel alterar.\n", nome);
                }
                break;

            case 13:   
                printf("Digite o tipo de categoria para buscar streams (ex: Noticias, Esporte): ");
                fgets(tipoCategoria, 30, stdin);
                tipoCategoria[strcspn(tipoCategoria, "\n")] = 0;
                printf("\nStreams com categoria do tipo '%s':\n", tipoCategoria);
                buscarStreamPorTipoCategoria(raiz, tipoCategoria);
                break;

            case 14:
                printf("Digite o nome da categoria para buscar streams: ");
                fgets(nomeCategoria, 30, stdin);
                nomeCategoria[strcspn(nomeCategoria, "\n")] = 0;
                printf("\nStreams que contem a categoria '%s':\n", nomeCategoria);
                buscarStreamPorNomeCategoria(raiz, nomeCategoria);
                break;

            case 15:
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
                        printf("Digite a periodicidade (ex: diario, semanal): ");
                        fgets(periodicidade, 20, stdin);
                        periodicidade[strcspn(periodicidade, "\n")] = 0;
                        printf("Digite o horario (ex: 10:00): ");
                        fgets(horario, 20, stdin);
                        horario[strcspn(horario, "\n")] = 0;
                        printf("\nProgramas da stream '%s' na categoria '%s' no dia/horario '%s'/'%s':\n", nome, nomeCategoria, periodicidade, horario);
                        buscarProgramasPorDiaHorario(cat->programas, periodicidade, horario);
                    } else {
                        printf("Categoria nao encontrada!\n");
                    }
                } else {
                    printf("Stream nao encontrada!\n");
                }
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
                        printf("Digite a periodicidade (ex: diario, semanal, mensal): ");
                        fgets(periodicidade, 20, stdin);
                        periodicidade[strcspn(periodicidade, "\n")] = 0;
                        printf("\nProgramas da categoria '%s' na stream '%s' com periodicidade '%s':\n", nomeCategoria, nome, periodicidade);
                        buscarProgramasPorDiaSemana(cat->programas, periodicidade);
                    } else {
                        printf("Categoria nao encontrada!\n");
                    }
                } else {
                    printf("Stream nao encontrada!\n");
                }
                break;
            
            case 17:
                printf("Digite o nome da Stream para listar os apresentadores: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                printf("\nApresentadores da stream '%s':\n", nome);
                mostrarApresentadoresPorStream(listaApresentadores, nome);
                break;

            case 18:
                printf("Digite o nome da Categoria para listar os apresentadores: ");
                fgets(nomeCategoria, 30, stdin);
                nomeCategoria[strcspn(nomeCategoria, "\n")] = 0;
                printf("\nApresentadores da categoria '%s':\n", nomeCategoria);
                mostrarApresentadoresPorCategoria(listaApresentadores, nomeCategoria);
                break;

            case 19:
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
                        mostrarDadosPrograma(cat->programas, nomePrograma);
                    } else {
                        printf("Categoria nao encontrada!\n");
                    }
                } else {
                    printf("Stream nao encontrada!\n");
                }
                break;

            case 20:
                printf("Digite o nome da Stream: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                s = buscarStream(raiz, nome);
                if (s != NULL) {
                    printf("Digite o nome da Categoria a ser removida: ");
                    fgets(nomeCategoria, 30, stdin);
                    nomeCategoria[strcspn(nomeCategoria, "\n")] = 0;
                    removerCategoria(s, nomeCategoria);
                } else {
                    printf("Stream nao encontrada!\n");
                }
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


