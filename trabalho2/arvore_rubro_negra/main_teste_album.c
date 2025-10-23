/*
 * TESTE - ÁRVORE RUBRO-NEGRA (LLRB)
 * Main para testar apenas Álbuns e Músicas
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rubro_negra.h"

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void exibirMenu() {
    printf("\n===============================================================\n");
    printf("        TESTE - ARVORE RUBRO-NEGRA DE ALBUNS (LLRB)       \n");
    printf("===============================================================\n");
    printf("  1. Inserir Album                                         \n");
    printf("  2. Remover Album                                         \n");
    printf("  3. Buscar Album                                          \n");
    printf("  4. Listar Todos os Albuns                                \n");
    printf("  5. Inserir Musica em Album                               \n");
    printf("  6. Remover Musica de Album                               \n");
    printf("  7. Buscar Musica em Album                                \n");
    printf("  8. Listar Musicas de um Album                            \n");
    printf("  9. Visualizar Estrutura da Arvore                        \n");
    printf("  0. Sair                                                  \n");
    printf("===============================================================\n");
    printf("Escolha: ");
}

void inserirAlbum(NoRB_Album **raiz) {
    Album novo;
    
    printf("\n>> INSERIR NOVO ALBUM\n");
    printf("Titulo do album: ");
    limparBuffer();
    fgets(novo.titulo, 100, stdin);
    novo.titulo[strcspn(novo.titulo, "\n")] = 0;
    
    printf("Ano de lancamento: ");
    scanf("%d", &novo.ano_lancamento);
    
    novo.qtd_musicas = 0;
    novo.lista_musicas = NULL;
    
    *raiz = inserirAlbumRB(*raiz, novo);
}

void removerAlbum(NoRB_Album **raiz) {
    char titulo[100];
    
    printf("\n>> REMOVER ALBUM\n");
    printf("Titulo do album: ");
    limparBuffer();
    fgets(titulo, 100, stdin);
    titulo[strcspn(titulo, "\n")] = 0;
    
    *raiz = removerAlbumRB(raiz, titulo);
}

void buscarAlbum(NoRB_Album *raiz) {
    char titulo[100];
    
    printf("\n>> BUSCAR ALBUM\n");
    printf("Titulo do album: ");
    limparBuffer();
    fgets(titulo, 100, stdin);
    titulo[strcspn(titulo, "\n")] = 0;
    
    NoRB_Album *encontrado = buscarAlbumRB(raiz, titulo);
    
    if (encontrado != NULL) {
        printf("\n[OK] ALBUM ENCONTRADO:\n");
        imprimirAlbum(encontrado->info);
    } else {
        printf("[ERRO] Album '%s' nao encontrado!\n", titulo);
    }
}

void listarAlbuns(NoRB_Album *raiz) {
    printf("\n>> LISTA DE ALBUNS (em ordem alfabetica):\n");
    if (raiz == NULL) {
        printf("  Nenhum album cadastrado.\n");
    } else {
        imprimirAlbunsInOrder(raiz);
    }
}

void inserirMusica(NoRB_Album *raiz) {
    char tituloAlbum[100], tituloMusica[100];
    float minutos;
    
    printf("\n>> INSERIR MUSICA\n");
    printf("Titulo do album: ");
    limparBuffer();
    fgets(tituloAlbum, 100, stdin);
    tituloAlbum[strcspn(tituloAlbum, "\n")] = 0;
    
    NoRB_Album *album = buscarAlbumRB(raiz, tituloAlbum);
    
    if (album == NULL) {
        printf("[ERRO] Album '%s' nao encontrado!\n", tituloAlbum);
        return;
    }
    
    printf("Titulo da musica: ");
    fgets(tituloMusica, 100, stdin);
    tituloMusica[strcspn(tituloMusica, "\n")] = 0;
    
    printf("Duracao (minutos): ");
    scanf("%f", &minutos);
    
    inserirMusicaLista(&album->info.lista_musicas, tituloMusica, minutos);
    album->info.qtd_musicas++;
    
    printf("[OK] Musica '%s' inserida no album '%s'!\n", tituloMusica, tituloAlbum);
}

void removerMusica(NoRB_Album *raiz) {
    char tituloAlbum[100], tituloMusica[100];
    
    printf("\n>> REMOVER MUSICA\n");
    printf("Titulo do album: ");
    limparBuffer();
    fgets(tituloAlbum, 100, stdin);
    tituloAlbum[strcspn(tituloAlbum, "\n")] = 0;
    
    NoRB_Album *album = buscarAlbumRB(raiz, tituloAlbum);
    
    if (album == NULL) {
        printf("[ERRO] Album '%s' nao encontrado!\n", tituloAlbum);
        return;
    }
    
    printf("Titulo da musica: ");
    fgets(tituloMusica, 100, stdin);
    tituloMusica[strcspn(tituloMusica, "\n")] = 0;
    
    removerMusicaLista(&album->info.lista_musicas, tituloMusica);
    album->info.qtd_musicas--;
    
    printf("[OK] Musica '%s' removida do album '%s'!\n", tituloMusica, tituloAlbum);
}

void buscarMusica(NoRB_Album *raiz) {
    char tituloAlbum[100], tituloMusica[100];
    
    printf("\n>> BUSCAR MUSICA\n");
    printf("Titulo do album: ");
    limparBuffer();
    fgets(tituloAlbum, 100, stdin);
    tituloAlbum[strcspn(tituloAlbum, "\n")] = 0;
    
    NoRB_Album *album = buscarAlbumRB(raiz, tituloAlbum);
    
    if (album == NULL) {
        printf("[ERRO] Album '%s' nao encontrado!\n", tituloAlbum);
        return;
    }
    
    printf("Titulo da musica: ");
    fgets(tituloMusica, 100, stdin);
    tituloMusica[strcspn(tituloMusica, "\n")] = 0;
    
    Musica *atual = album->info.lista_musicas;
    while (atual != NULL) {
        if (strcmp(atual->titulo, tituloMusica) == 0) {
            printf("\n[OK] MUSICA ENCONTRADA:\n");
            printf("  -> %s (%.2f min)\n", atual->titulo, atual->minutos);
            printf("  -> Album: %s (%d)\n", album->info.titulo, album->info.ano_lancamento);
            return;
        }
        atual = atual->prox;
    }
    
    printf("[ERRO] Musica '%s' nao encontrada no album '%s'!\n", tituloMusica, tituloAlbum);
}

void listarMusicasAlbum(NoRB_Album *raiz) {
    char tituloAlbum[100];
    
    printf("\n>> LISTAR MUSICAS DE ALBUM\n");
    printf("Titulo do album: ");
    limparBuffer();
    fgets(tituloAlbum, 100, stdin);
    tituloAlbum[strcspn(tituloAlbum, "\n")] = 0;
    
    NoRB_Album *album = buscarAlbumRB(raiz, tituloAlbum);
    
    if (album == NULL) {
        printf("[ERRO] Album '%s' nao encontrado!\n", tituloAlbum);
        return;
    }
    
    imprimirAlbum(album->info);
}

void imprimirArvoreVisual(NoRB_Album *raiz, int nivel, char *prefixo) {
    if (raiz == NULL) return;
    
    printf("%s", prefixo);
    
    if (nivel > 0) {
        printf("+-- ");
    }
    
    char *cor = (raiz->cor == VERMELHO) ? "[R]" : "[B]";
    printf("%s %s (%d) - %d musicas\n", 
           cor, raiz->info.titulo, raiz->info.ano_lancamento, raiz->info.qtd_musicas);
    
    char novoPrefixo[200];
    sprintf(novoPrefixo, "%s%s", prefixo, (nivel > 0) ? "|   " : "");
    
    if (raiz->esq != NULL || raiz->dir != NULL) {
        if (raiz->esq != NULL)
            imprimirArvoreVisual(raiz->esq, nivel + 1, novoPrefixo);
        if (raiz->dir != NULL)
            imprimirArvoreVisual(raiz->dir, nivel + 1, novoPrefixo);
    }
}

void visualizarEstrutura(NoRB_Album *raiz) {
    printf("\n>> ESTRUTURA DA ARVORE RUBRO-NEGRA (LLRB):\n");
    printf("Legenda: [R] = VERMELHO | [B] = PRETO\n\n");
    
    if (raiz == NULL) {
        printf("  Arvore vazia.\n");
    } else {
        imprimirArvoreVisual(raiz, 0, "");
    }
}

void carregarDadosTeste(NoRB_Album **raiz) {
    printf("\n>> Carregando dados de teste...\n");
    
    // Albuns do The Beatles
    Album alb1 = {"Abbey Road", 1969, 0, NULL};
    *raiz = inserirAlbumRB(*raiz, alb1);
    
    Album alb2 = {"Let It Be", 1970, 0, NULL};
    *raiz = inserirAlbumRB(*raiz, alb2);
    
    Album alb3 = {"Sgt Peppers Lonely Hearts Club Band", 1967, 0, NULL};
    *raiz = inserirAlbumRB(*raiz, alb3);
    
    // Albuns de Pink Floyd
    Album alb4 = {"The Dark Side of the Moon", 1973, 0, NULL};
    *raiz = inserirAlbumRB(*raiz, alb4);
    
    Album alb5 = {"The Wall", 1979, 0, NULL};
    *raiz = inserirAlbumRB(*raiz, alb5);
    
    // Inserir algumas musicas
    NoRB_Album *abbey = buscarAlbumRB(*raiz, "Abbey Road");
    if (abbey) {
        inserirMusicaLista(&abbey->info.lista_musicas, "Come Together", 4.20);
        inserirMusicaLista(&abbey->info.lista_musicas, "Something", 3.03);
        inserirMusicaLista(&abbey->info.lista_musicas, "Here Comes the Sun", 3.05);
        abbey->info.qtd_musicas = 3;
    }
    
    NoRB_Album *darkside = buscarAlbumRB(*raiz, "The Dark Side of the Moon");
    if (darkside) {
        inserirMusicaLista(&darkside->info.lista_musicas, "Time", 6.53);
        inserirMusicaLista(&darkside->info.lista_musicas, "Money", 6.23);
        inserirMusicaLista(&darkside->info.lista_musicas, "Us and Them", 7.49);
        darkside->info.qtd_musicas = 3;
    }
    
    printf("[OK] Dados de teste carregados com sucesso!\n");
    printf("   5 albuns e 6 musicas adicionados.\n");
}

int main() {
    NoRB_Album *raiz = NULL;
    int opcao;
    
    printf("\n===============================================================\n");
    printf("     BEM-VINDO AO TESTE DE ARVORE RUBRO-NEGRA (LLRB)      \n");
    printf("              Albuns e Musicas - Sua Parte                 \n");
    printf("===============================================================\n");
    
    // Opcao de carregar dados de teste
    printf("\nDeseja carregar dados de teste? (1=Sim, 0=Nao): ");
    int carregar;
    scanf("%d", &carregar);
    
    if (carregar == 1) {
        carregarDadosTeste(&raiz);
    }
    
    do {
        exibirMenu();
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                inserirAlbum(&raiz);
                break;
            case 2:
                removerAlbum(&raiz);
                break;
            case 3:
                buscarAlbum(raiz);
                break;
            case 4:
                listarAlbuns(raiz);
                break;
            case 5:
                inserirMusica(raiz);
                break;
            case 6:
                removerMusica(raiz);
                break;
            case 7:
                buscarMusica(raiz);
                break;
            case 8:
                listarMusicasAlbum(raiz);
                break;
            case 9:
                visualizarEstrutura(raiz);
                break;
            case 0:
                printf("\n>> Liberando memoria e encerrando...\n");
                liberarArvoreAlbuns(raiz);
                printf("[OK] Programa finalizado com sucesso!\n");
                break;
            default:
                printf("[ERRO] Opcao invalida!\n");
        }
        
    } while (opcao != 0);
    
    return 0;
}
