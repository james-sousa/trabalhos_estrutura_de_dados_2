#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_CELULAS 160  // 8 colunas * 20 linhas
#define MAX_FORMULA 256
#define COLUNAS 8       // A-H
#define LINHAS 20       // 1-20

// Tipos de célula
typedef enum {
    NUMERO,
    REFERENCIA,
    FORMULA_SOMA,
    FORMULA_MAX,
    FORMULA_MIN,
    FORMULA_MEDIA,
    VAZIO
} TipoCelula;

// Nó da lista de adjacências
typedef struct NoAresta {
    int destino;              // índice do vértice destino
    struct NoAresta *proximo;
} NoAresta;

// Estrutura da célula
typedef struct {
    double valor;
    TipoCelula tipo;
    char formula[MAX_FORMULA];
    int calculada;
    NoAresta *arestas;        // lista de adjacências (dependências)
} Celula;

// Estrutura do grafo com listas de adjacências
typedef struct {
    Celula *celulas;
    int numVertices;
} GrafoLista;

// Converter coordenada (coluna, linha) para índice linear
int coordenadaParaIndice(char coluna, int linha) {
    if (coluna < 'A' || coluna > 'H' || linha < 1 || linha > 20) {
        return -1;
    }
    return (coluna - 'A') * 20 + (linha - 1);
}

// Converter índice linear para coordenada
void indiceParaCoordenada(int indice, char *coluna, int *linha) {
    *coluna = 'A' + (indice / 20);
    *linha = (indice % 20) + 1;
}

// Inicializar grafo com listas de adjacências
GrafoLista* inicializarGrafo() {
    GrafoLista *g = (GrafoLista*)malloc(sizeof(GrafoLista));
    g->numVertices = MAX_CELULAS;
    g->celulas = (Celula*)malloc(MAX_CELULAS * sizeof(Celula));
    
    for (int i = 0; i < MAX_CELULAS; i++) {
        g->celulas[i].valor = 0.0;
        g->celulas[i].tipo = VAZIO;
        g->celulas[i].formula[0] = '\0';
        g->celulas[i].calculada = 0;
        g->celulas[i].arestas = NULL;  // lista vazia
    }
    
    return g;
}

// Adicionar aresta (dependência) na lista de adjacências
void adicionarAresta(GrafoLista *g, int origem, int destino) {
    if (origem < 0 || origem >= MAX_CELULAS || destino < 0 || destino >= MAX_CELULAS) {
        return;
    }
    
    // Verificar se aresta já existe
    NoAresta *temp = g->celulas[origem].arestas;
    while (temp != NULL) {
        if (temp->destino == destino) {
            return;  // aresta já existe
        }
        temp = temp->proximo;
    }
    
    // Inserir nova aresta no início da lista
    NoAresta *novaAresta = (NoAresta*)malloc(sizeof(NoAresta));
    novaAresta->destino = destino;
    novaAresta->proximo = g->celulas[origem].arestas;
    g->celulas[origem].arestas = novaAresta;
}

// Remover aresta (dependência) da lista de adjacências
void removerAresta(GrafoLista *g, int origem, int destino) {
    if (origem < 0 || origem >= MAX_CELULAS) {
        return;
    }
    
    NoAresta *temp = g->celulas[origem].arestas;
    NoAresta *anterior = NULL;
    
    while (temp != NULL) {
        if (temp->destino == destino) {
            if (anterior == NULL) {
                g->celulas[origem].arestas = temp->proximo;
            } else {
                anterior->proximo = temp->proximo;
            }
            free(temp);
            return;
        }
        anterior = temp;
        temp = temp->proximo;
    }
}

// Limpar todas as arestas de um vértice
void limparArestas(GrafoLista *g, int indice) {
    if (indice < 0 || indice >= MAX_CELULAS) {
        return;
    }
    
    NoAresta *temp = g->celulas[indice].arestas;
    while (temp != NULL) {
        NoAresta *proximo = temp->proximo;
        free(temp);
        temp = proximo;
    }
    g->celulas[indice].arestas = NULL;
}

// Verificar se string é número
int ehNumero(const char *str) {
    if (str == NULL || str[0] == '\0') return 0;
    if (str[0] == '-' && strlen(str) == 1) return 0;
    
    for (int i = (str[0] == '-' ? 1 : 0); str[i] != '\0'; i++) {
        if (!isdigit(str[i]) && str[i] != '.') return 0;
    }
    return 1;
}

// Converter string para double
double stringParaDouble(const char *str) {
    return atof(str);
}

// Forward declaration
double calcularCelula(GrafoLista *g, int indice);

// Calcular valor de uma célula
double calcularCelula(GrafoLista *g, int indice) {
    if (indice < 0 || indice >= MAX_CELULAS) {
        return 0.0;
    }
    
    Celula *cel = &g->celulas[indice];
    
    if (cel->calculada) {
        return cel->valor;
    }
    
    switch (cel->tipo) {
        case NUMERO:
            cel->calculada = 1;
            return cel->valor;
            
        case REFERENCIA: {
            const char *ref = cel->formula + 1;  // pular '='
            char colRef = ref[0];
            int linRef = 0;
            for (int j = 1; ref[j] != '\0'; j++) {
                linRef = linRef * 10 + (ref[j] - '0');
            }
            int indiceRef = coordenadaParaIndice(colRef, linRef);
            if (indiceRef >= 0) {
                cel->valor = calcularCelula(g, indiceRef);
            }
            cel->calculada = 1;
            return cel->valor;
        }
        
        case FORMULA_SOMA:
        case FORMULA_MAX:
        case FORMULA_MIN:
        case FORMULA_MEDIA: {
            char *inicio = cel->formula;
            while (*inicio != '(' && *inicio != '\0') inicio++;
            if (*inicio == '\0') return 0.0;
            inicio++;
            
            int i = 0;
            char col1 = inicio[i];
            int lin1 = 0;
            i++;
            while (inicio[i] != '.' && inicio[i] != '\0') {
                lin1 = lin1 * 10 + (inicio[i] - '0');
                i++;
            }
            i += 2;
            
            char col2 = inicio[i];
            int lin2 = 0;
            i++;
            while (inicio[i] != ')' && inicio[i] != '\0') {
                lin2 = lin2 * 10 + (inicio[i] - '0');
                i++;
            }
            
            double resultado = 0.0;
            int contador = 0;
            double max_val = -INFINITY;
            double min_val = INFINITY;
            
            for (char c = col1; c <= col2; c++) {
                for (int l = lin1; l <= lin2; l++) {
                    int idx = coordenadaParaIndice(c, l);
                    if (idx >= 0 && idx != indice) {
                        // Ignorar células vazias
                        if (g->celulas[idx].tipo == VAZIO) {
                            continue;
                        }
                        
                        double val = calcularCelula(g, idx);
                        contador++;
                        
                        switch (cel->tipo) {
                            case FORMULA_SOMA:
                                resultado += val;
                                break;
                            case FORMULA_MAX:
                                if (contador == 1 || val > max_val) max_val = val;
                                break;
                            case FORMULA_MIN:
                                if (contador == 1 || val < min_val) min_val = val;
                                break;
                            case FORMULA_MEDIA:
                                resultado += val;
                                break;
                        }
                    }
                }
            }
            
            if (cel->tipo == FORMULA_MEDIA && contador > 0) {
                resultado /= contador;
            } else if (cel->tipo == FORMULA_MAX) {
                resultado = max_val;
            } else if (cel->tipo == FORMULA_MIN) {
                resultado = min_val;
            }
            
            cel->valor = resultado;
            cel->calculada = 1;
            return resultado;
        }
        
        case VAZIO:
        default:
            return 0.0;
    }
}

// Definir valor de uma célula
void definirCelula(GrafoLista *g, char coluna, int linha, const char *valor) {
    int indice = coordenadaParaIndice(coluna, linha);
    if (indice < 0) {
        printf("Coordenada invalida: %c%d\n", coluna, linha);
        return;
    }
    
    Celula *cel = &g->celulas[indice];
    
    // Limpar dependências antigas
    limparArestas(g, indice);
    
    // Marcar para recalcular
    for (int i = 0; i < MAX_CELULAS; i++) {
        if (g->celulas[i].tipo != NUMERO && g->celulas[i].tipo != VAZIO) {
            g->celulas[i].calculada = 0;
        }
    }
    
    if (ehNumero(valor)) {
        cel->tipo = NUMERO;
        cel->valor = stringParaDouble(valor);
        cel->calculada = 1;
        cel->formula[0] = '\0';
        
    } else if (valor[0] == '=') {
        cel->tipo = REFERENCIA;
        cel->calculada = 0;
        
        int i;
        for (i = 0; i < MAX_FORMULA - 1 && valor[i] != '\0'; i++) {
            cel->formula[i] = valor[i];
        }
        cel->formula[i] = '\0';
        
        char colRef = valor[1];
        int linRef = 0;
        for (int j = 2; valor[j] != '\0'; j++) {
            linRef = linRef * 10 + (valor[j] - '0');
        }
        int indiceRef = coordenadaParaIndice(colRef, linRef);
        if (indiceRef >= 0) {
            adicionarAresta(g, indice, indiceRef);
        }
        
    } else if (valor[0] == '@') {
        cel->calculada = 0;
        
        int i;
        for (i = 0; i < MAX_FORMULA - 1 && valor[i] != '\0'; i++) {
            cel->formula[i] = valor[i];
        }
        cel->formula[i] = '\0';
        
        if (valor[1] == 's' && valor[2] == 'o' && valor[3] == 'm' && valor[4] == 'a') {
            cel->tipo = FORMULA_SOMA;
        } else if (valor[1] == 'm' && valor[2] == 'a' && valor[3] == 'x') {
            cel->tipo = FORMULA_MAX;
        } else if (valor[1] == 'm' && valor[2] == 'i' && valor[3] == 'n') {
            cel->tipo = FORMULA_MIN;
        } else if (valor[1] == 'm' && valor[2] == 'e' && valor[3] == 'd' && valor[4] == 'i' && valor[5] == 'a') {
            cel->tipo = FORMULA_MEDIA;
        }
        
        char *inicio = cel->formula;
        while (*inicio != '(' && *inicio != '\0') inicio++;
        if (*inicio != '\0') {
            inicio++;
            
            char col1 = inicio[0];
            int lin1 = 0, j = 1;
            while (inicio[j] != '.' && inicio[j] != '\0') {
                lin1 = lin1 * 10 + (inicio[j] - '0');
                j++;
            }
            j += 2;
            
            char col2 = inicio[j];
            int lin2 = 0;
            j++;
            while (inicio[j] != ')' && inicio[j] != '\0') {
                lin2 = lin2 * 10 + (inicio[j] - '0');
                j++;
            }
            
            for (char c = col1; c <= col2; c++) {
                for (int l = lin1; l <= lin2; l++) {
                    int idx = coordenadaParaIndice(c, l);
                    if (idx >= 0 && idx != indice) {
                        adicionarAresta(g, indice, idx);
                    }
                }
            }
        }
    } else {
        cel->tipo = VAZIO;
        cel->valor = 0.0;
        cel->calculada = 0;
        cel->formula[0] = '\0';
    }
    
    printf("Celula %c%d definida com sucesso!\n", coluna, linha);
}

// Exibir planilha
void exibirMatriz(GrafoLista *g) {
    printf("\n=== PLANILHA ===\n");
    printf("     ");
    for (char c = 'A'; c < 'A' + COLUNAS; c++) {
        printf("  %c     ", c);
    }
    printf("\n");
    
    for (int l = 1; l <= LINHAS; l++) {
        printf("%2d: ", l);
        for (char c = 'A'; c < 'A' + COLUNAS; c++) {
            int idx = coordenadaParaIndice(c, l);
            if (g->celulas[idx].tipo == VAZIO) {
                printf("   -    ");
            } else {
                double val = calcularCelula(g, idx);
                printf("%7.2f ", val);
            }
        }
        printf("\n");
    }
    printf("\n");
}

// Exibir dependências (grafo com listas de adjacências)
void exibirDependencias(GrafoLista *g) {
    printf("\n=== ESTRUTURA DO GRAFO (Listas de Adjacencias) ===\n");
    printf("Mostrando vertices com arestas:\n\n");
    
    for (int i = 0; i < MAX_CELULAS; i++) {
        if (g->celulas[i].arestas != NULL) {
            char col;
            int lin;
            indiceParaCoordenada(i, &col, &lin);
            printf("Vertice %c%d conectado por arestas a: ", col, lin);
            
            NoAresta *temp = g->celulas[i].arestas;
            while (temp != NULL) {
                char colDest;
                int linDest;
                indiceParaCoordenada(temp->destino, &colDest, &linDest);
                printf("%c%d ", colDest, linDest);
                temp = temp->proximo;
            }
            printf("\n");
        }
    }
    printf("\n");
}

// Liberar memória do grafo
void liberarGrafo(GrafoLista *g) {
    for (int i = 0; i < MAX_CELULAS; i++) {
        limparArestas(g, i);
    }
    free(g->celulas);
    free(g);
}

int main() {
    GrafoLista *g = inicializarGrafo();
    
    char entrada[200];
    char coluna;
    int linha;
    char valor[100];
    
    printf("=== PLANILHA DE CALCULO (Listas de Adjacencias) ===\n");
    printf("Colunas: A ate H\n");
    printf("Linhas: 1 ate 20\n");
    printf("Formatos de entrada:\n");
    printf("  - Numero: A5 42.5\n");
    printf("  - Referencia: A5 =B10\n");
    printf("  - Formulas: A5 @soma(B1..D3), @max(A1..A5), @min(C1..C10), @media(B2..E4)\n");
    printf("  - Digite 'mostrar' para exibir a planilha\n");
    printf("  - Digite 'dep' para mostrar dependencias\n");
    printf("  - Digite 'sair' para encerrar\n\n");
        
    exibirMatriz(g);
    
    while (1) {
        printf("Digite comando (ex: A5 42 ou 'mostrar'): ");
        if (fgets(entrada, sizeof(entrada), stdin) == NULL) break;
        
        for (int i = 0; entrada[i] != '\0'; i++) {
            if (entrada[i] == '\n') {
                entrada[i] = '\0';
                break;
            }
        }
        
        if (entrada[0] == 's' && entrada[1] == 'a' && entrada[2] == 'i' && entrada[3] == 'r') {
            break;
        }
        
        if (entrada[0] == 'm' && entrada[1] == 'o' && entrada[2] == 's' && entrada[3] == 't') {
            exibirMatriz(g);
            continue;
        }
        
        if (entrada[0] == 'd' && entrada[1] == 'e' && entrada[2] == 'p') {
            exibirDependencias(g);
            continue;
        }
        
        if (entrada[0] >= 'A' && entrada[0] <= 'H') {
            coluna = entrada[0];
            int i = 1;
            
            linha = 0;
            while (entrada[i] != ' ' && entrada[i] != '\0') {
                linha = linha * 10 + (entrada[i] - '0');
                i++;
            }
            
            while (entrada[i] == ' ') i++;
            
            strcpy(valor, entrada + i);
            
            definirCelula(g, coluna, linha, valor);
            exibirMatriz(g);
        }
    }
    
    printf("Encerrando planilha...\n");
    liberarGrafo(g);
    
    return 0;
}
