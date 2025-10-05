#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore_2-3.h"


int ehFolha(Arvore23_art *No) {
    return (No && No->esq == NULL && No->meio == NULL && No->dir == NULL);
}

void criaNo(Arvore23_art **No, Artista info, Arvore23_art *esq, Arvore23_art *meio) {
    *No = (Arvore23_art *) malloc(sizeof(Arvore23_art));
    (*No)->info1 = info;
    (*No)->num_chaves = 1;
    (*No)->esq = esq;
    (*No)->meio = meio;
    (*No)->dir = NULL;
}

void adicionaNo(Arvore23_art **No, Artista info, Arvore23_art *filho) {
    if (strcmp(info.nome, (*No)->info1.nome) > 0) {
        (*No)->info2 = info;
        (*No)->dir = filho;
    } else {
        (*No)->info2 = (*No)->info1;
        (*No)->info1 = info;
        (*No)->dir = (*No)->meio;
        (*No)->meio = filho;
    }
    (*No)->num_chaves = 2;
}

void quebraNo(Arvore23_art **No, Artista info, Artista *sobe, Arvore23_art **No_maior, Arvore23_art *filho) {
    if (strcmp(info.nome, (*No)->info1.nome) < 0) {
        *sobe = (*No)->info1;
        (*No)->info1 = info;
        criaNo(No_maior, (*No)->info2, (*No)->meio, (*No)->dir);
        (*No)->meio = filho;
    } else if ((*No)->num_chaves == 2 && strcmp(info.nome, (*No)->info2.nome) < 0) {
        *sobe = info;
        criaNo(No_maior, (*No)->info2, filho, (*No)->dir);
    } else {
        *sobe = (*No)->info2;
        criaNo(No_maior, info, (*No)->dir, filho);
    }
    (*No)->dir = NULL;
    (*No)->num_chaves = 1;
}

/*-----------------------------------------------------------
    INSERÇÃO DE ARTISTA
-----------------------------------------------------------*/

Arvore23_art *inserirRec_art(Arvore23_art *No, Artista info, Artista *sobe, Arvore23_art **novo) {
    // Caso base: nó nulo → cria novo nó folha
    if (No == NULL) {
        criaNo(novo, info, NULL, NULL);
        *sobe = info;
        return NULL;
    }

    // Caso folha → insere ou divide
    if (ehFolha(No)) {
        if (No->num_chaves == 1) {
            adicionaNo(&No, info, NULL);
            return NULL;
        } else {
            Arvore23_art *No_maior = NULL;
            quebraNo(&No, info, sobe, &No_maior, NULL);
            *novo = No_maior;
            return No;
        }
    }

    // Caso interno → escolher subárvore correta
    Arvore23_art *ret = NULL;
    if (strcmp(info.nome, No->info1.nome) < 0)
        ret = inserirRec_art(No->esq, info, sobe, novo);
    else if (No->num_chaves == 1 || strcmp(info.nome, No->info2.nome) < 0)
        ret = inserirRec_art(No->meio, info, sobe, novo);
    else
        ret = inserirRec_art(No->dir, info, sobe, novo);

    // Se não houve quebra abaixo → retorna
    if (ret == NULL) return NULL;

    // Se houve quebra → insere ou divide o nó atual
    if (No->num_chaves == 1) {
        if (ret == No->esq)
            adicionaNo(&No, *sobe, *novo);
        else
            adicionaNo(&No, *sobe, *novo);
        return NULL;
    } else {
        Arvore23_art *No_maior = NULL;
        quebraNo(&No, *sobe, sobe, &No_maior, *novo);
        *novo = No_maior;
        return No;
    }
}

Arvore23_art *inserirArtista(Arvore23_art *Raiz, Artista novoArtista) {
    Artista sobe;
    Arvore23_art *novo = NULL;
    Arvore23_art *ret = inserirRec_art(Raiz, novoArtista, &sobe, &novo);

    // Caso a raiz tenha se dividido → cria nova raiz
    if (ret != NULL) {
        Arvore23_art *novaRaiz;
        criaNo(&novaRaiz, sobe, ret, novo);
        return novaRaiz;
    }
    return (Raiz ? Raiz : novo);
}

/*-----------------------------------------------------------
    EXEMPLO DE USO
-----------------------------------------------------------*/

void imprimirInOrder(Arvore23_art *Raiz) {
    if (!Raiz) return;
    imprimirInOrder(Raiz->esq);
    printf("%s (%s)\n", Raiz->info1.nome, Raiz->info1.estilo);
    imprimirInOrder(Raiz->meio);
    if (Raiz->num_chaves == 2)
        printf("%s (%s)\n", Raiz->info2.nome, Raiz->info2.estilo);
    imprimirInOrder(Raiz->dir);
}

void imprimirBiblioteca(Arvore23_art *Raiz, int nivel) {
    if (!Raiz) return;

    // Se tiver 1 chave: só dois filhos (esq, meio)
    if (Raiz->num_chaves == 1) {
        imprimirBiblioteca(Raiz->meio, nivel + 1);
        for (int i = 0; i < nivel; i++) printf("    ");
        printf("[%s]\n", Raiz->info1.nome);
        imprimirBiblioteca(Raiz->esq, nivel + 1);
    } 
    // Se tiver 2 chaves: três filhos (esq, meio, dir)
    else if (Raiz->num_chaves == 2) {
        imprimirBiblioteca(Raiz->dir, nivel + 1);
        for (int i = 0; i < nivel; i++) printf("    ");
        printf("[%s | %s]\n", Raiz->info1.nome, Raiz->info2.nome);
        imprimirBiblioteca(Raiz->meio, nivel + 1);
        imprimirBiblioteca(Raiz->esq, nivel + 1);
    }
}


void liberaNo(Arvore23_art** no){
    if (*no)
    {
        free(*no);
    }
    *no = NULL;
}

int balancear(Arvore23_art** raiz){
    Arvore23_art* no = *raiz;
    if (no->num_chaves == 0)
    {
        Arvore23_art* filho = no->esq ? no->esq : no->meio ? no->meio : no->dir;
        free(no);
        *raiz = filho;
        return 1; 
    }
    
    return 0;
}


int removeMenor(Arvore23_art** raiz, Artista* menor) {
    if (!*raiz) return 0;

    if ((*raiz)->esq) {
        int desb = removeMenor(&(*raiz)->esq, menor);
        if (desb) return balancear(raiz);
        return 0;
    }

    // Encontrou o menor (nó folha)
    *menor = (*raiz)->info1;
    if ((*raiz)->num_chaves == 2) {
        (*raiz)->info1 = (*raiz)->info2;
        (*raiz)->num_chaves = 1;
        return 0;
    } else {
        liberaNo(raiz);
        return 1;
    }
}

// Função principal de remoção
int removerArtista(Arvore23_art **raiz, char *nomeArtista) {
    if (!*raiz) return 0;

    Arvore23_art *no = *raiz;
    int desb = 0;

    // Caso 1: encontrou o artista
    if (strcmp(no->info1.nome, nomeArtista) == 0 ||
        (no->num_chaves == 2 && strcmp(no->info2.nome, nomeArtista) == 0)) {

        // Caso folha
        if (ehFolha(no)) {
            if (no->num_chaves == 2) {
                // Tem duas chaves — remove uma
                if (strcmp(nomeArtista, no->info1.nome) == 0)
                    no->info1 = no->info2;
                no->num_chaves = 1;
                return 0;
            } else {
                // Tem uma só — o nó fica vazio
                liberaNo(raiz);
                return 1;
            }
        }

        // Caso interno — substitui pela menor chave da subárvore à direita
        Artista menor;
        if (strcmp(nomeArtista, no->info1.nome) == 0)
            desb = removeMenor(&no->meio, &menor), no->info1 = menor;
        else
            desb = removeMenor(&no->dir, &menor), no->info2 = menor;

        if (desb) return balancear(raiz);
        return 0;
    }

    // Caso 2: busca recursivamente onde está o artista
    if (strcmp(nomeArtista, no->info1.nome) < 0)
        desb = removerArtista(&no->esq, nomeArtista);
    else if (no->num_chaves == 2 && strcmp(nomeArtista, no->info2.nome) > 0)
        desb = removerArtista(&no->dir, nomeArtista);
    else
        desb = removerArtista(&no->meio, nomeArtista);

    // Caso 3: o filho ficou vazio — precisa reequilibrar
    if (desb) return balancear(raiz);
    return 0;
}


int main() {
    Arvore23_art *raiz = NULL;
    int opcao;
    Artista novo;
    char nomeRemover[100];

    do {
        printf("\n========== BIBLIOTECA DE ARTISTAS (ÁRVORE 2-3) ==========\n");
        printf("1 - Inserir artista\n");
        printf("2 - Remover artista\n");
        printf("3 - Listar artistas (ordem alfabética)\n");
        printf("4 - Mostrar hierarquia da árvore\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar(); // limpa buffer

        switch (opcao) {
            case 1:
                printf("\nNome do artista: ");
                fgets(novo.nome, 100, stdin);
                novo.nome[strcspn(novo.nome, "\n")] = '\0';
                printf("Estilo musical: ");
                fgets(novo.estilo, 50, stdin);
                novo.estilo[strcspn(novo.estilo, "\n")] = '\0';
                novo.qtd_albuns = 0;
                novo.albuns = NULL;
                raiz = inserirArtista(raiz, novo);
                printf("✅ Artista '%s' inserido com sucesso!\n", novo.nome);
                break;

            case 2:
                printf("\nDigite o nome do artista para remover: ");
                fgets(nomeRemover, 100, stdin);
                nomeRemover[strcspn(nomeRemover, "\n")] = '\0';
                removerArtista(&raiz, nomeRemover);
                printf("🗑️  Artista '%s' removido (se existia).\n", nomeRemover);
                break;

            case 3:
                printf("\n--- Lista de Artistas (em ordem) ---\n");
                imprimirInOrder(raiz);
                break;

            case 4:
                printf("\n--- Estrutura da Árvore 2-3 ---\n");
                imprimirBiblioteca(raiz, 0);
                printf("---------------------------------\n");
                break;
        }
    } while (opcao != 0);

    printf("\nPrograma encerrado.\n");
    return 0;
}
