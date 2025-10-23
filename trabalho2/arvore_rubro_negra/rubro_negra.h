#ifndef RUBRO_NEGRA_H
#define RUBRO_NEGRA_H

/*-----------------------------------------------------------
    ESTRUTURAS DE DADOS
-----------------------------------------------------------*/

// Estrutura de Música (Lista Encadeada Ordenada)
typedef struct musica {
    char titulo[100];
    float minutos;
    struct musica *prox;
} Musica;

// Estrutura de Álbum (Árvore Rubro-Negra)
typedef struct album {
    char titulo[100];
    int ano_lancamento;
    int qtd_musicas;
    Musica *lista_musicas;  // Lista ordenada de músicas
} Album;

// Cores da Árvore Rubro-Negra
typedef enum { VERMELHO, PRETO } Cor;

// Estrutura de Artista (para ser usada na árvore principal - implementação do James)
typedef struct artista {
    char nome[100];
    char estilo[50];
    int qtd_albuns;
    struct no_rb_album *albuns;  // Árvore Rubro-Negra de álbuns
} Artista;

// Nó da Árvore Rubro-Negra de Artistas (implementado pelo James)
typedef struct no_rb_artista {
    Artista info;
    Cor cor;
    struct no_rb_artista *esq;
    struct no_rb_artista *dir;
    struct no_rb_artista *pai;
} NoRB_Artista;

// Nó da Árvore Rubro-Negra de Álbuns (SUA IMPLEMENTAÇÃO - LLRB)
typedef struct no_rb_album {
    Album info;
    Cor cor;
    struct no_rb_album *esq;
    struct no_rb_album *dir;
} NoRB_Album;

/*-----------------------------------------------------------
    FUNÇÕES DE LISTA DE MÚSICAS (SUA PARTE)
-----------------------------------------------------------*/

Musica* criarMusica(char *titulo, float minutos);

void inserirMusicaLista(Musica **lista, char *titulo, float minutos);

void removerMusicaLista(Musica **lista, char *titulo);

void listarMusicas(Musica *lista);

void liberarMusicas(Musica *lista);

/*-----------------------------------------------------------
    FUNÇÕES DA ÁRVORE RUBRO-NEGRA DE ÁLBUNS (SUA PARTE - LLRB)
-----------------------------------------------------------*/

NoRB_Album* criarNoAlbum(Album album);

int ehVermelhoAlbum(NoRB_Album *no);

NoRB_Album* rotacaoEsquerdaAlbum(NoRB_Album *h);

NoRB_Album* rotacaoDireitaAlbum(NoRB_Album *h);

void inverterCoresAlbum(NoRB_Album *h);

NoRB_Album* inserirAlbumRB(NoRB_Album *raiz, Album album);

NoRB_Album* buscarAlbumRB(NoRB_Album *raiz, char *titulo);

NoRB_Album* minimoAlbum(NoRB_Album *no);

NoRB_Album* moverVermelhoEsquerda(NoRB_Album *h);

NoRB_Album* moverVermelhoDireita(NoRB_Album *h);

NoRB_Album* balancearAlbum(NoRB_Album *h);

NoRB_Album* removerAlbumRB(NoRB_Album **raiz, char *titulo);

void imprimirAlbunsInOrder(NoRB_Album *raiz);

void liberarArvoreAlbuns(NoRB_Album *raiz);

/*-----------------------------------------------------------
    FUNÇÕES DE INTEGRAÇÃO COM ARTISTA (Interface para James)
-----------------------------------------------------------*/

void inserirMusicaEmArtista(NoRB_Artista *raiz_artistas, char *nomeArtista, 
                           char *tituloAlbum, char *tituloMusica, float minutos);

void removerMusicaDeArtista(NoRB_Artista *raiz_artistas, char *nomeArtista, 
                           char *tituloAlbum, char *tituloMusica);

Album* buscarAlbumEmArtista(NoRB_Artista *raiz_artistas, char *nomeArtista, char *tituloAlbum);

Musica* buscarMusicaEmArtista(NoRB_Artista *raiz_artistas, char *nomeArtista, 
                              char *tituloAlbum, char *tituloMusica);

/*-----------------------------------------------------------
    FUNÇÕES DE IMPRESSÃO (SUA PARTE)
-----------------------------------------------------------*/

void imprimirMusicas(Musica *lista);

void imprimirAlbum(Album alb);

#endif
