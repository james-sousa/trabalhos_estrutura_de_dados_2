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

// --- SUA PARTE: FUNÇÕES ---
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
            
            // ---adcionado ao MENU
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
                        
                        if (buscarApresentador(listaApresentadores, nomeApresentador) == NULL) {
                            printf("Apresentador nao cadastrado. Por favor, cadastre-o primeiro.\n");
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

            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}