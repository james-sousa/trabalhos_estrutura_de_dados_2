/*
 * Árvore Rubro-Negra - Questão 1
 * RESPONSÁVEL: Implementação de Álbuns e Músicas
 * - Lista encadeada ordenada de Músicas
 * - Árvore Rubro-Negra de Álbuns
 * - Funções de inserção, remoção e busca
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "rubro_negra.h"

/*═══════════════════════════════════════════════════════════════════════════
    PARTE 1: LISTA ENCADEADA ORDENADA DE MÚSICAS
═══════════════════════════════════════════════════════════════════════════*/

/**
 * Cria uma nova música
 */
Musica* criarMusica(char *titulo, float minutos) {
    Musica *nova = (Musica *)malloc(sizeof(Musica));
    if (nova == NULL) {
        printf("Erro ao alocar memoria para musica!\n");
        return NULL;
    }
    
    strcpy(nova->titulo, titulo);
    nova->minutos = minutos;
    nova->prox = NULL;
    
    return nova;
}

/**
 * Insere uma música na lista ordenada por título
 */
void inserirMusicaLista(Musica **lista, char *titulo, float minutos) {
    Musica *nova = criarMusica(titulo, minutos);
    if (nova == NULL) return;
    
    // Lista vazia ou inserir no início
    if (*lista == NULL || strcmp(titulo, (*lista)->titulo) < 0) {
        nova->prox = *lista;
        *lista = nova;
        return;
    }
    
    // Buscar posição correta (ordenada por título)
    Musica *atual = *lista;
    while (atual->prox != NULL && strcmp(titulo, atual->prox->titulo) > 0) {
        atual = atual->prox;
    }
    
    // Inserir na posição encontrada
    nova->prox = atual->prox;
    atual->prox = nova;
}

/**
 * Remove uma música da lista
 */
void removerMusicaLista(Musica **lista, char *titulo) {
    if (*lista == NULL) {
        printf("[ERRO] Lista de musicas vazia!\n");
        return;
    }
    
    Musica *atual = *lista;
    
    // Remover do início
    if (strcmp(atual->titulo, titulo) == 0) {
        *lista = atual->prox;
        free(atual);
        return;
    }
    
    // Buscar música para remover
    while (atual->prox != NULL && strcmp(atual->prox->titulo, titulo) != 0) {
        atual = atual->prox;
    }
    
    if (atual->prox != NULL) {
        Musica *temp = atual->prox;
        atual->prox = temp->prox;
        free(temp);
    } else {
        printf("[ERRO] Musica '%s' nao encontrada!\n", titulo);
    }
}

/**
 * Lista todas as músicas em ordem
 */
void listarMusicas(Musica *lista) {
    if (lista == NULL) {
        printf("  Nenhuma musica cadastrada.\n");
        return;
    }
    
    Musica *atual = lista;
    int i = 1;
    while (atual != NULL) {
        printf("  %d. %s (%.2f min)\n", i++, atual->titulo, atual->minutos);
        atual = atual->prox;
    }
}

/**
 * Libera toda a memória da lista de músicas
 */
void liberarMusicas(Musica *lista) {
    Musica *atual = lista;
    while (atual != NULL) {
        Musica *temp = atual;
        atual = atual->prox;
        free(temp);
    }
}

/**
 * Imprime músicas  para listarMusicas)
 */
void imprimirMusicas(Musica *lista) {
    listarMusicas(lista);
}

/*═══════════════════════════════════════════════════════════════════════════
    PARTE 2: ÁRVORE RUBRO-NEGRA DE ÁLBUNS
═══════════════════════════════════════════════════════════════════════════*/

/**
 * Cria um novo nó de álbum (sempre VERMELHO)
 */
NoRB_Album* criarNoAlbum(Album album) {
    NoRB_Album *novo = (NoRB_Album *)malloc(sizeof(NoRB_Album));
    if (novo == NULL) {
        printf("Erro ao alocar memoria para album!\n");
        return NULL;
    }
    
    novo->info = album;
    novo->cor = VERMELHO;  // Novos nós sempre começam vermelhos
    novo->esq = NULL;
    novo->dir = NULL;
    
    return novo;
}

/**
 * Verifica se um nó é vermelho
 */
int ehVermelhoAlbum(NoRB_Album *no) {
    if (no == NULL) return 0;
    return no->cor == VERMELHO;
}

/**
 * Rotação à esquerda (LLRB)
 */
NoRB_Album* rotacaoEsquerdaAlbum(NoRB_Album *h) {
    NoRB_Album *x = h->dir;
    h->dir = x->esq;
    x->esq = h;
    x->cor = h->cor;
    h->cor = VERMELHO;
    return x;
}

/**
 * Rotação à direita (LLRB)
 */
NoRB_Album* rotacaoDireitaAlbum(NoRB_Album *h) {
    NoRB_Album *x = h->esq;
    h->esq = x->dir;
    x->dir = h;
    x->cor = h->cor;
    h->cor = VERMELHO;
    return x;
}

/**
 * Inverte as cores (LLRB)
 */
void inverterCoresAlbum(NoRB_Album *h) {
    h->cor = VERMELHO;
    h->esq->cor = PRETO;
    h->dir->cor = PRETO;
}

/**
 * Insere um álbum (recursivo - LLRB)
 */
NoRB_Album* inserirAlbumRB_recursivo(NoRB_Album *h, Album album, int *inserido) {
    // Inserção padrão BST
    if (h == NULL) {
        *inserido = 1;
        return criarNoAlbum(album);
    }
    
    int cmp = strcmp(album.titulo, h->info.titulo);
    
    if (cmp < 0)
        h->esq = inserirAlbumRB_recursivo(h->esq, album, inserido);
    else if (cmp > 0)
        h->dir = inserirAlbumRB_recursivo(h->dir, album, inserido);
    else {
        // Álbum já existe
        printf("[AVISO] Album '%s' ja existe!\n", album.titulo);
        *inserido = 0;
        return h;
    }
    
    // Balanceamento LLRB (pendente à esquerda)
    // 1. Se filho direito é vermelho e esquerdo é preto: rotacionar à esquerda
    if (ehVermelhoAlbum(h->dir) && !ehVermelhoAlbum(h->esq))
        h = rotacaoEsquerdaAlbum(h);
    
    // 2. Se filho esquerdo e neto esquerdo são vermelhos: rotacionar à direita
    if (ehVermelhoAlbum(h->esq) && ehVermelhoAlbum(h->esq->esq))
        h = rotacaoDireitaAlbum(h);
    
    // 3. Se ambos os filhos são vermelhos: inverter cores
    if (ehVermelhoAlbum(h->esq) && ehVermelhoAlbum(h->dir))
        inverterCoresAlbum(h);
    
    return h;
}

/**
 * Insere um álbum na árvore Rubro-Negra (LLRB)
 */
NoRB_Album* inserirAlbumRB(NoRB_Album *raiz, Album album) {
    int inserido = 0;
    raiz = inserirAlbumRB_recursivo(raiz, album, &inserido);
    
    if (raiz != NULL)
        raiz->cor = PRETO;  // Raiz sempre preta
    
    if (inserido)
        printf("[OK] Album '%s' inserido com sucesso!\n", album.titulo);
    
    return raiz;
}

/**
 * Busca um álbum na árvore
 */
NoRB_Album* buscarAlbumRB(NoRB_Album *raiz, char *titulo) {
    NoRB_Album *atual = raiz;
    
    while (atual != NULL) {
        int cmp = strcmp(titulo, atual->info.titulo);
        
        if (cmp == 0)
            return atual;
        else if (cmp < 0)
            atual = atual->esq;
        else
            atual = atual->dir;
    }
    
    return NULL;  // Não encontrado
}

/**
 * Encontra o nó com valor mínimo
 */
NoRB_Album* minimoAlbum(NoRB_Album *no) {
    while (no->esq != NULL)
        no = no->esq;
    return no;
}

/**
 * Move vermelho à esquerda (LLRB)
 */
NoRB_Album* moverVermelhoEsquerda(NoRB_Album *h) {
    inverterCoresAlbum(h);
    if (h->dir != NULL && ehVermelhoAlbum(h->dir->esq)) {
        h->dir = rotacaoDireitaAlbum(h->dir);
        h = rotacaoEsquerdaAlbum(h);
        inverterCoresAlbum(h);
    }
    return h;
}

/**
 * Move vermelho à direita (LLRB)
 */
NoRB_Album* moverVermelhoDireita(NoRB_Album *h) {
    inverterCoresAlbum(h);
    if (h->esq != NULL && ehVermelhoAlbum(h->esq->esq)) {
        h = rotacaoDireitaAlbum(h);
        inverterCoresAlbum(h);
    }
    return h;
}

/**
 * Balanceia a árvore após remoção (LLRB)
 */
NoRB_Album* balancearAlbum(NoRB_Album *h) {
    if (ehVermelhoAlbum(h->dir))
        h = rotacaoEsquerdaAlbum(h);
    
    if (ehVermelhoAlbum(h->esq) && ehVermelhoAlbum(h->esq->esq))
        h = rotacaoDireitaAlbum(h);
    
    if (ehVermelhoAlbum(h->esq) && ehVermelhoAlbum(h->dir))
        inverterCoresAlbum(h);
    
    return h;
}

/**
 * Remove o menor nó (LLRB)
 */
NoRB_Album* removerMinimoAlbum(NoRB_Album *h) {
    if (h->esq == NULL) {
        free(h);
        return NULL;
    }
    
    if (!ehVermelhoAlbum(h->esq) && !ehVermelhoAlbum(h->esq->esq))
        h = moverVermelhoEsquerda(h);
    
    h->esq = removerMinimoAlbum(h->esq);
    
    return balancearAlbum(h);
}

/**
 * Remove um álbum (recursivo - LLRB)
 */
NoRB_Album* removerAlbumRB_recursivo(NoRB_Album *h, char *titulo, int *removido) {
    if (h == NULL) return NULL;
    
    int cmp = strcmp(titulo, h->info.titulo);
    
    if (cmp < 0) {
        if (h->esq != NULL) {
            if (!ehVermelhoAlbum(h->esq) && !ehVermelhoAlbum(h->esq->esq))
                h = moverVermelhoEsquerda(h);
            h->esq = removerAlbumRB_recursivo(h->esq, titulo, removido);
        }
    } else {
        if (ehVermelhoAlbum(h->esq))
            h = rotacaoDireitaAlbum(h);
        
        if (strcmp(titulo, h->info.titulo) == 0 && h->dir == NULL) {
            // Encontrou e é folha ou tem só filho esquerdo
            printf("[AVISO] Todas as %d musicas do album '%s' serao removidas!\n", 
                   h->info.qtd_musicas, h->info.titulo);
            liberarMusicas(h->info.lista_musicas);
            free(h);
            *removido = 1;
            return NULL;
        }
        
        if (h->dir != NULL) {
            if (!ehVermelhoAlbum(h->dir) && !ehVermelhoAlbum(h->dir->esq))
                h = moverVermelhoDireita(h);
            
            if (strcmp(titulo, h->info.titulo) == 0) {
                // Encontrou no meio da árvore
                NoRB_Album *min = minimoAlbum(h->dir);
                
                printf("[AVISO] Todas as %d musicas do album '%s' serao removidas!\n", 
                       h->info.qtd_musicas, h->info.titulo);
                liberarMusicas(h->info.lista_musicas);
                
                h->info = min->info;
                h->dir = removerMinimoAlbum(h->dir);
                *removido = 1;
            } else {
                h->dir = removerAlbumRB_recursivo(h->dir, titulo, removido);
            }
        }
    }
    
    return balancearAlbum(h);
}

/**
 * Remove um álbum da árvore Rubro-Negra (LLRB)
 */
NoRB_Album* removerAlbumRB(NoRB_Album **raiz, char *titulo) {
    int removido = 0;
    *raiz = removerAlbumRB_recursivo(*raiz, titulo, &removido);
    
    if (*raiz != NULL)
        (*raiz)->cor = PRETO;
    
    if (removido)
        printf("[OK] Album '%s' removido com sucesso!\n", titulo);
    else
        printf("[ERRO] Album '%s' nao encontrado!\n", titulo);
    
    return *raiz;
}

/**
 * Imprime álbuns em ordem
 */
void imprimirAlbunsInOrder(NoRB_Album *raiz) {
    if (raiz == NULL) return;
    
    imprimirAlbunsInOrder(raiz->esq);
    printf("  >> %s (%d) - %d musicas\n", 
           raiz->info.titulo, 
           raiz->info.ano_lancamento,
           raiz->info.qtd_musicas);
    imprimirAlbunsInOrder(raiz->dir);
}

/**
 * Libera toda a árvore de álbuns
 */
void liberarArvoreAlbuns(NoRB_Album *raiz) {
    if (raiz == NULL) return;
    
    liberarArvoreAlbuns(raiz->esq);
    liberarArvoreAlbuns(raiz->dir);
    
    // Liberar músicas do álbum
    liberarMusicas(raiz->info.lista_musicas);
    
    free(raiz);
}

/**
 * Imprime informações de um álbum
 */
void imprimirAlbum(Album alb) {
    printf("\n  >> Album: %s (%d)\n", alb.titulo, alb.ano_lancamento);
    printf("  Quantidade de musicas: %d\n", alb.qtd_musicas);
    printf("  Musicas:\n");
    listarMusicas(alb.lista_musicas);
}

/*═══════════════════════════════════════════════════════════════════════════
    PARTE 3: FUNÇÕES DE INTEGRAÇÃO COM ARTISTA
    (Interface para o James usar com a árvore principal de Artistas)
═══════════════════════════════════════════════════════════════════════════*/

/**
 * STUB TEMPORÁRIO - Busca um artista na árvore (será implementado pelo James)
 * 
 * Para compilar sem o código do James, esta é uma implementação temporária.
 * O James deve substituir isso pela implementação real.
 */
#ifndef JAMES_IMPLEMENTACAO
NoRB_Artista* buscarArtistaRB(NoRB_Artista *raiz, char *nome) {
    (void)raiz;
    (void)nome;
    printf("[AVISO] Funcao buscarArtistaRB() ainda nao implementada pelo James!\n");
    return NULL;
}
#endif

/**
 * Insere uma música em um álbum de um artista
 */
void inserirMusicaEmArtista(NoRB_Artista *raiz_artistas, char *nomeArtista, 
                           char *tituloAlbum, char *tituloMusica, float minutos) {
    // Buscar artista (função do James)
    NoRB_Artista *artista = buscarArtistaRB(raiz_artistas, nomeArtista);
    
    if (artista == NULL) {
        printf("[ERRO] Artista '%s' nao encontrado!\n", nomeArtista);
        return;
    }
    
    // Buscar álbum
    NoRB_Album *album = buscarAlbumRB(artista->info.albuns, tituloAlbum);
    
    if (album == NULL) {
        printf("[ERRO] Album '%s' nao encontrado para o artista '%s'!\n", 
               tituloAlbum, nomeArtista);
        return;
    }
    
    // Inserir música na lista
    inserirMusicaLista(&album->info.lista_musicas, tituloMusica, minutos);
    album->info.qtd_musicas++;
    
    printf("[OK] Musica '%s' inserida no album '%s'!\n", tituloMusica, tituloAlbum);
}

/**
 * Remove uma música de um álbum de um artista
 */
void removerMusicaDeArtista(NoRB_Artista *raiz_artistas, char *nomeArtista, 
                           char *tituloAlbum, char *tituloMusica) {
    NoRB_Artista *artista = buscarArtistaRB(raiz_artistas, nomeArtista);
    
    if (artista == NULL) {
        printf("[ERRO] Artista '%s' nao encontrado!\n", nomeArtista);
        return;
    }
    
    NoRB_Album *album = buscarAlbumRB(artista->info.albuns, tituloAlbum);
    
    if (album == NULL) {
        printf("[ERRO] Album '%s' nao encontrado!\n", tituloAlbum);
        return;
    }
    
    removerMusicaLista(&album->info.lista_musicas, tituloMusica);
    album->info.qtd_musicas--;
    
    printf("[OK] Musica '%s' removida do album '%s'!\n", tituloMusica, tituloAlbum);
}

/**
 * Busca um álbum em um artista
 */
Album* buscarAlbumEmArtista(NoRB_Artista *raiz_artistas, char *nomeArtista, char *tituloAlbum) {
    NoRB_Artista *artista = buscarArtistaRB(raiz_artistas, nomeArtista);
    
    if (artista == NULL) return NULL;
    
    NoRB_Album *album = buscarAlbumRB(artista->info.albuns, tituloAlbum);
    
    if (album == NULL) return NULL;
    
    return &album->info;
}

/**
 * Busca uma música em um álbum de um artista
 */
Musica* buscarMusicaEmArtista(NoRB_Artista *raiz_artistas, char *nomeArtista, 
                              char *tituloAlbum, char *tituloMusica) {
    Album *album = buscarAlbumEmArtista(raiz_artistas, nomeArtista, tituloAlbum);
    
    if (album == NULL) return NULL;
    
    Musica *atual = album->lista_musicas;
    while (atual != NULL) {
        if (strcmp(atual->titulo, tituloMusica) == 0)
            return atual;
        atual = atual->prox;
    }
    
    return NULL;
}

/*═══════════════════════════════════════════════════════════════════════════
    FIM DA SUA IMPLEMENTAÇÃO
    
    RESUMO DO QUE FOI IMPLEMENTADO:
    ✅ Lista encadeada ordenada de Músicas
    ✅ Árvore Rubro-Negra de Álbuns
    ✅ Inserção, remoção e busca de músicas
    ✅ Inserção, remoção e busca de álbuns
    ✅ Rotações e correções da árvore Rubro-Negra
    ✅ Funções de integração com a parte do James (Artistas)
    ✅ Liberação de memória
    
    O JAMES PRECISA IMPLEMENTAR:
    - Árvore Rubro-Negra de Artistas
    - Função buscarArtistaRB()
    - Inserção e remoção de Artistas
    - Main com menu
    - Experimento de busca (30 artistas)
═══════════════════════════════════════════════════════════════════════════*/
