#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
} Arvore23;

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

int ehFolha(Arvore23 *No);

void criaNo(Arvore23 **No, Artista info, Arvore23 *esq, Arvore23 *meio);

void adicionaNo(Arvore23 **No, Artista info, Arvore23 *filho);

void quebraNo(Arvore23 **No, Artista info, Artista *sobe, Arvore23 **No_maior, Arvore23 *filho);

/*-----------------------------------------------------------
    INSERÇÃO
-----------------------------------------------------------*/

Arvore23* inserirArtista(Arvore23 *Raiz, Artista novoArtista);

Arvore23* inserirAlbum(Arvore23 *Raiz, char *nomeArtista, Album novoAlbum);

void inserirMusica(Arvore23 *Raiz, char *nomeArtista, char *tituloAlbum, Musica novaMusica);

/*-----------------------------------------------------------
    BUSCAS
-----------------------------------------------------------*/

Arvore23* buscarArtista(Arvore23 *Raiz, char *nomeArtista);

Album* buscarAlbum(Arvore23 *Raiz, char *nomeArtista, char *tituloAlbum);

Musica* buscarMusica(Arvore23 *Raiz, char *nomeArtista, char *tituloAlbum, char *tituloMusica);

/*-----------------------------------------------------------
    REMOÇÕES
-----------------------------------------------------------*/

void removerMusica(Arvore23 *Raiz, char *nomeArtista, char *tituloAlbum, char *tituloMusica);

void removerAlbum(Arvore23 *Raiz, char *nomeArtista, char *tituloAlbum);

void removerArtista(Arvore23 **Raiz, char *nomeArtista);

/*-----------------------------------------------------------
    IMPRESSÕES
-----------------------------------------------------------*/

void imprimirMusicas(Musica *lista);

void imprimirAlbum(Album alb);

void imprimirArtista(Artista art);

void imprimirBiblioteca(Arvore23 *Raiz, int nivel);

/*-----------------------------------------------------------
    EXPERIMENTO DE BUSCA (30 ARTISTAS)
-----------------------------------------------------------*/

void experimentoBusca(Arvore23 *Raiz);

/*-----------------------------------------------------------
    MENU PRINCIPAL
-----------------------------------------------------------*/

void menuPrincipal();

int main();
