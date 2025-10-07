#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#define Repeticoes 30

typedef struct {
    int I_bloco, F_bloco;
    char estado; // 'o' = ocupado, 'l' = livre
} bloco;

// Nó da árvore 4-5
typedef struct b {
    bloco info1, info2, info3, info4;
    struct b *filho1, *filho2, *filho3, *filho4, *filho5;
    int Ninfos;
} arvore45;

// ===========================================
// FUNÇÕES DE SUPORTE DA ÁRVORE 4-5
// ===========================================
int ehfolha(arvore45 *No){
    if (No != NULL){
        return (No->filho1 == NULL && No->filho2 == NULL && No->filho3 == NULL && 
                No->filho4 == NULL && No->filho5 == NULL);
    }
    return 0;
}

void criaNo(arvore45 **No, bloco info, arvore45 *esq, arvore45 *dir){
    *No = (arvore45 *)malloc(sizeof(arvore45));
    (*No)->info1 = info;
    (*No)->Ninfos = 1;
    (*No)->filho1 = esq;
    (*No)->filho2 = dir;
    (*No)->filho3 = (*No)->filho4 = (*No)->filho5 = NULL;
}

void adicionaNo(arvore45 **No, bloco info, arvore45 *filho){
    if ((*No)->Ninfos == 1){
        if (info.I_bloco > (*No)->info1.I_bloco){
            (*No)->info2 = info;
            (*No)->filho3 = filho;
        } else {
            (*No)->info2 = (*No)->info1;
            (*No)->info1 = info;
            (*No)->filho3 = (*No)->filho2;
            (*No)->filho2 = filho;
        }
    } else if ((*No)->Ninfos == 2){
        if (info.I_bloco > (*No)->info2.I_bloco){
            (*No)->info3 = info;
            (*No)->filho4 = filho;
        } else if (info.I_bloco > (*No)->info1.I_bloco){
            (*No)->info3 = (*No)->info2;
            (*No)->info2 = info;
            (*No)->filho4 = (*No)->filho3;
            (*No)->filho3 = filho;
        } else {
            (*No)->info3 = (*No)->info2;
            (*No)->info2 = (*No)->info1;
            (*No)->info1 = info;
            (*No)->filho4 = (*No)->filho3;
            (*No)->filho3 = (*No)->filho2;
            (*No)->filho2 = filho;
        }
    } else if ((*No)->Ninfos == 3){
        if (info.I_bloco > (*No)->info3.I_bloco){
            (*No)->info4 = info;
            (*No)->filho5 = filho;
        } else if (info.I_bloco > (*No)->info2.I_bloco){
            (*No)->info4 = (*No)->info3;
            (*No)->info3 = info;
            (*No)->filho5 = (*No)->filho4;
            (*No)->filho4 = filho;
        } else if (info.I_bloco > (*No)->info1.I_bloco){
            (*No)->info4 = (*No)->info3;
            (*No)->info3 = (*No)->info2;
            (*No)->info2 = info;
            (*No)->filho5 = (*No)->filho4;
            (*No)->filho4 = (*No)->filho3;
            (*No)->filho3 = filho;
        } else {
            (*No)->info4 = (*No)->info3;
            (*No)->info3 = (*No)->info2;
            (*No)->info2 = (*No)->info1;
            (*No)->info1 = info;
            (*No)->filho5 = (*No)->filho4;
            (*No)->filho4 = (*No)->filho3;
            (*No)->filho3 = (*No)->filho2;
            (*No)->filho2 = filho;
        }
    }
    (*No)->Ninfos++;
}

void quebraNo(arvore45 **No, bloco info, bloco *sobe, arvore45 **No_maior, arvore45 *filho){
    bloco v[5];
    arvore45 *filhos[6];
    int i, pos = 0;
    
    v[0] = (*No)->info1;
    v[1] = (*No)->info2;
    v[2] = (*No)->info3;
    v[3] = (*No)->info4;
    
    filhos[0] = (*No)->filho1;
    filhos[1] = (*No)->filho2;
    filhos[2] = (*No)->filho3;
    filhos[3] = (*No)->filho4;
    filhos[4] = (*No)->filho5;
    
    for (i = 3; i >= 0; i--){
        if (info.I_bloco > v[i].I_bloco){
            pos = i + 1;
            break;
        }
        if (i == 0) pos = 0;
    }
    
    for (i = 4; i > pos; i--){
        v[i] = v[i-1];
        filhos[i+1] = filhos[i];
    }
    v[pos] = info;
    filhos[pos+1] = filho;
    
    *sobe = v[2];
    
    (*No)->info1 = v[0];
    (*No)->info2 = v[1];
    (*No)->Ninfos = 2;
    (*No)->filho1 = filhos[0];
    (*No)->filho2 = filhos[1];
    (*No)->filho3 = filhos[2];
    (*No)->filho4 = NULL;
    (*No)->filho5 = NULL;
    
    *No_maior = (arvore45 *)malloc(sizeof(arvore45));
    (*No_maior)->info1 = v[3];
    (*No_maior)->info2 = v[4];
    (*No_maior)->Ninfos = 2;
    (*No_maior)->filho1 = filhos[3];
    (*No_maior)->filho2 = filhos[4];
    (*No_maior)->filho3 = filhos[5];
    (*No_maior)->filho4 = (*No_maior)->filho5 = NULL;
}

int insereRec(arvore45 **raiz, bloco info, bloco *sobe, arvore45 **novo){
    if (*raiz == NULL){
        criaNo(raiz, info, NULL, NULL);
        return 0;
    }
    if (ehfolha(*raiz)){
        if ((*raiz)->Ninfos < 4){
            adicionaNo(raiz, info, NULL);
            return 0;
        } else {
            quebraNo(raiz, info, sobe, novo, NULL);
            return 1;
        }
    } else {
        arvore45 **filhoEscolhido;
        if (info.I_bloco < (*raiz)->info1.I_bloco) filhoEscolhido = &(*raiz)->filho1;
        else if ((*raiz)->Ninfos == 1 || info.I_bloco < (*raiz)->info2.I_bloco) filhoEscolhido = &(*raiz)->filho2;
        else if ((*raiz)->Ninfos == 2 || info.I_bloco < (*raiz)->info3.I_bloco) filhoEscolhido = &(*raiz)->filho3;
        else if ((*raiz)->Ninfos == 3 || info.I_bloco < (*raiz)->info4.I_bloco) filhoEscolhido = &(*raiz)->filho4;
        else filhoEscolhido = &(*raiz)->filho5;

        bloco sobeFilho;
        arvore45 *novoFilho = NULL;
        if (insereRec(filhoEscolhido, info, &sobeFilho, &novoFilho)){
            if ((*raiz)->Ninfos < 4){
                adicionaNo(raiz, sobeFilho, novoFilho);
                return 0;
            } else {
                quebraNo(raiz, sobeFilho, sobe, novo, novoFilho);
                return 1;
            }
        }
        return 0;
    }
}

void insere(arvore45 **raiz, bloco info){
    bloco sobe;
    arvore45 *novo = NULL;
    if (insereRec(raiz, info, &sobe, &novo)){
        arvore45 *novaRaiz;
        criaNo(&novaRaiz, sobe, *raiz, novo);
        *raiz = novaRaiz;
    }
}

void imprimeArvore(arvore45 *raiz, int nivel){
    if (raiz == NULL) return;
    for (int i = 0; i < nivel; i++) printf("    ");
    printf("Nivel %d: [", nivel);
    for (int i = 0; i < raiz->Ninfos; i++){
        bloco b;
        if (i == 0) b = raiz->info1;
        else if (i == 1) b = raiz->info2;
        else if (i == 2) b = raiz->info3;
        else b = raiz->info4;
        printf("(%d-%d, %c)", b.I_bloco, b.F_bloco, toupper(b.estado));
        if (i < raiz->Ninfos - 1) printf(", ");
    }
    printf("]\n");
    if (raiz->filho1) imprimeArvore(raiz->filho1, nivel + 1);
    if (raiz->filho2) imprimeArvore(raiz->filho2, nivel + 1);
    if (raiz->filho3) imprimeArvore(raiz->filho3, nivel + 1);
    if (raiz->filho4) imprimeArvore(raiz->filho4, nivel + 1);
    if (raiz->filho5) imprimeArvore(raiz->filho5, nivel + 1);
}

// ===========================================
// FUNÇÕES DE GERENCIAMENTO DE MEMÓRIA
// ===========================================

// (a) Cadastro inicial da memória
void cadastraNos(arvore45 **raiz) {
    bloco b;
    int totalMemoria, enderecoInicial, enderecoFinal;
    char estado;

    printf("\nInforme o tamanho total da memória (em blocos): ");
    scanf("%d", &totalMemoria);

    printf("\nInforme o estado inicial (L = livre, O = ocupado): ");
    scanf(" %c", &estado);
    estado = tolower(estado);

    printf("Informe o endereço inicial do primeiro bloco: ");
    scanf("%d", &enderecoInicial);

    do {
        printf("Informe o endereço final desse nó: ");
        scanf("%d", &enderecoFinal);
        if (enderecoFinal > totalMemoria - 1) {
            printf("O endereço final não pode ultrapassar o tamanho da memória.\n");
        }
    } while (enderecoFinal > totalMemoria - 1);

    b.I_bloco = enderecoInicial;
    b.F_bloco = enderecoFinal;
    b.estado = estado;
    insere(raiz, b);

    enderecoInicial = enderecoFinal + 1;
    estado = (estado == 'l') ? 'o' : 'l';

    while (enderecoInicial < totalMemoria) {
        printf("\nInforme o endereço final do próximo nó (%c): ", toupper(estado));
        scanf("%d", &enderecoFinal);
        if (enderecoFinal >= totalMemoria) enderecoFinal = totalMemoria - 1;

        b.I_bloco = enderecoInicial;
        b.F_bloco = enderecoFinal;
        b.estado = estado;
        insere(raiz, b);

        enderecoInicial = enderecoFinal + 1;
        estado = (estado == 'l') ? 'o' : 'l';
    }

    printf("\nCadastro concluído!\n");
    imprimeArvore(*raiz, 0);
}

// (b) Alocar memória
void alocaMemoria(arvore45 *raiz, int qtdBlocos) {
    if (raiz == NULL) return;

    for (int i = 0; i < raiz->Ninfos; i++) {
        bloco *b = (i == 0) ? &raiz->info1 :
                   (i == 1) ? &raiz->info2 :
                   (i == 2) ? &raiz->info3 : &raiz->info4;

        int tamanho = b->F_bloco - b->I_bloco + 1;

        if (b->estado == 'l' && tamanho >= qtdBlocos) {
            printf("\nAlocando %d blocos no intervalo [%d - %d]\n",
                   qtdBlocos, b->I_bloco, b->I_bloco + qtdBlocos - 1);

            if (tamanho == qtdBlocos) {
                b->estado = 'o';
            } else {
                // Divide o bloco
                bloco novoLivre = *b;
                novoLivre.I_bloco = b->I_bloco + qtdBlocos;
                b->F_bloco = b->I_bloco + qtdBlocos - 1;
                b->estado = 'o';
                insere(&raiz, novoLivre);
            }
            return;
        }
    }

    alocaMemoria(raiz->filho1, qtdBlocos);
    alocaMemoria(raiz->filho2, qtdBlocos);
    alocaMemoria(raiz->filho3, qtdBlocos);
    alocaMemoria(raiz->filho4, qtdBlocos);
    alocaMemoria(raiz->filho5, qtdBlocos);
}

// (c) Liberar memória
void liberaMemoria(arvore45 *raiz, int blocoInicial) {
    if (raiz == NULL) return;

    for (int i = 0; i < raiz->Ninfos; i++) {
        bloco *b = (i == 0) ? &raiz->info1 :
                   (i == 1) ? &raiz->info2 :
                   (i == 2) ? &raiz->info3 : &raiz->info4;

        if (b->estado == 'o' && blocoInicial >= b->I_bloco && blocoInicial <= b->F_bloco) {
            printf("\nLiberando bloco [%d - %d]\n", b->I_bloco, b->F_bloco);
            b->estado = 'l';
            return;
        }
    }

    liberaMemoria(raiz->filho1, blocoInicial);
    liberaMemoria(raiz->filho2, blocoInicial);
    liberaMemoria(raiz->filho3, blocoInicial);
    liberaMemoria(raiz->filho4, blocoInicial);
    liberaMemoria(raiz->filho5, blocoInicial);
}

// ===========================================
// PROGRAMA PRINCIPAL
// ===========================================
int main() {
    arvore45 *raiz = NULL;
    int opcao, qtd, blocoInicial;

    do {
        printf("\n===== GERENCIADOR DE MEMÓRIA =====");
        printf("\n1 - Cadastrar nós iniciais");
        printf("\n2 - Alocar memória");
        printf("\n3 - Liberar memória");
        printf("\n4 - Mostrar árvore");
        printf("\n0 - Sair");
        printf("\nEscolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastraNos(&raiz);
                break;
            case 2:
                printf("Quantidade de blocos a alocar: ");
                scanf("%d", &qtd);
                alocaMemoria(raiz, qtd);
                imprimeArvore(raiz, 0);
                break;
            case 3:
                printf("Informe o bloco inicial a ser liberado: ");
                scanf("%d", &blocoInicial);
                liberaMemoria(raiz, blocoInicial);
                imprimeArvore(raiz, 0);
                break;
            case 4:
                imprimeArvore(raiz, 0);
                break;
            case 0:
                printf("Encerrando...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}
