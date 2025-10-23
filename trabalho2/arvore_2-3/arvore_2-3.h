#ifndef ARVORE_2_3_H
#define ARVORE_2_3_H

/*-----------------------------------------------------------
    ESTRUTURAS DE DADOS
-----------------------------------------------------------*/

typedef struct musica {
    char titulo[100];
    float minutos;
    struct musica *prox;
} Musica;

typedef struct album {
    char titulo[100];
    int ano_lancamento;
    int qtd_musicas;
    Musica *lista_musicas;
} Album;

typedef struct artista {
    char nome[100];
    char estilo[50];
    int qtd_albuns;
    struct no23 *albuns;  // Árvore 2-3 de álbuns
} Artista;

/*-------------------- NÓ DA ÁRVORE 2-3 --------------------*/

typedef struct no23 {
    Artista info1, info2;
    struct no23 *esq;
    struct no23 *meio;
    struct no23 *dir;
    int num_chaves;
} Arvore23_art;

/*-----------------------------------------------------------
    FUNÇÕES AUXILIARES DE LISTA (MÚSICAS)
-----------------------------------------------------------*/

Musica* criarMusica(char *titulo, float minutos);

void inserirMusicaLista(Musica **lista, char *titulo, float minutos);

void removerMusicaLista(Musica **lista, char *titulo);

void listarMusicas(Musica *lista);

/*-----------------------------------------------------------
    FUNÇÕES AUXILIARES DE ÁRVORE 2-3 (ARTISTAS)
-----------------------------------------------------------*/

int ehFolha(Arvore23_art *No);

void criaNo(Arvore23_art **No, Artista info, Arvore23_art *esq, Arvore23_art *meio);

void adicionaNo(Arvore23_art **No, Artista info, Arvore23_art *filho);

void quebraNo(Arvore23_art **No, Artista info, Artista *sobe, Arvore23_art **No_maior, Arvore23_art *filho);

/*-----------------------------------------------------------
    INSERÇÃO
-----------------------------------------------------------*/

Arvore23_art* inserirArtista(Arvore23_art *Raiz, Artista novoArtista);

Arvore23_art* inserirAlbum(Arvore23_art *Raiz, char *nomeArtista, Album novoAlbum);

void inserirMusica(Arvore23_art *Raiz, char *nomeArtista, char *tituloAlbum, Musica novaMusica);

/*-----------------------------------------------------------
    BUSCAS
-----------------------------------------------------------*/

Arvore23_art* buscarArtista(Arvore23_art *Raiz, char *nomeArtista);

Arvore23_art* buscarAlbumNoArvore(Arvore23_art *raiz, char *tituloAlbum, Album *resultado);

Album* buscarAlbum(Arvore23_art *Raiz, char *nomeArtista, char *tituloAlbum);

Musica* buscarMusica(Arvore23_art *Raiz, char *nomeArtista, char *tituloAlbum, char *tituloMusica);

/*-----------------------------------------------------------
    REMOÇÕES
-----------------------------------------------------------*/

void removerMusica(Arvore23_art *Raiz, char *nomeArtista, char *tituloAlbum, char *tituloMusica);

void removerAlbum(Arvore23_art *Raiz, char *nomeArtista, char *tituloAlbum);

int removerArtista(Arvore23_art **Raiz, char *nomeArtista);

/*-----------------------------------------------------------
    IMPRESSÕES
-----------------------------------------------------------*/

void imprimirMusicas(Musica *lista);

void imprimirAlbum(Album alb);

void imprimirArtista(Artista art);

void imprimirBiblioteca(Arvore23_art *Raiz, int nivel);

/*-----------------------------------------------------------
    FUNÇÕES AUXILIARES DE ÁRVORE 2-3 (ÁLBUNS)
-----------------------------------------------------------*/

int ehFolhaAlbum(Arvore23_art *No);

Arvore23_art* inserirRecAlbum(Arvore23_art *No, Album info, Album *sobe, Arvore23_art **novo);

Arvore23_art* removerRecAlbum(Arvore23_art **raiz, char *tituloAlbum);

/*-----------------------------------------------------------
    EXPERIMENTO DE BUSCA (30 ARTISTAS)
-----------------------------------------------------------*/

void experimentoBusca(Arvore23_art *Raiz);

/*-----------------------------------------------------------
    IMPRESSÃO EM ORDEM
-----------------------------------------------------------*/

void imprimirInOrder(Arvore23_art *Raiz);

#endif
