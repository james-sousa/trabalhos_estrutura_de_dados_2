#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define COLUNAS 8
#define LINHAS 20
#define MAX_CELULAS (COLUNAS * LINHAS)
#define MAX_FORMULA 100

typedef enum {
    NUMERO,
    REFERENCIA,
    FORMULA_SOMA,
    FORMULA_MAX,
    FORMULA_MIN,
    FORMULA_MEDIA,
    VAZIO
} TipoCelula;

typedef struct {
    TipoCelula tipo;
    double valor;
    char formula[MAX_FORMULA];
    int calculado;
} Celula;

typedef struct{
    int matriz[MAX_CELULAS][MAX_CELULAS];
    Celula celulas[MAX_CELULAS];
    int num_vertices;
}Grafo;

// Converter coordenada para índice
int coordenadaParaIndice(char coluna, int linha) {
    if (coluna < 'A' || coluna > 'H' || linha < 1 || linha > 20) {
        return -1;
    }
    return (linha - 1) * COLUNAS + (coluna - 'A');
}

// Converter índice para coordenada
void indiceParaCoordenada(int indice, char *coluna, int *linha) {
    *coluna = 'A' + (indice % COLUNAS);
    *linha = (indice / COLUNAS) + 1;
}

// Inicializar grafo
void inicializarGrafo(Grafo *g){
    g->num_vertices = MAX_CELULAS;
    for (int i = 0; i < MAX_CELULAS; i++)
    {
        for (int j = 0; j < MAX_CELULAS; j++)
        {
            g->matriz[i][j] = 0;
        }
        g->celulas[i].tipo = VAZIO;
        g->celulas[i].valor = 0.0;
        g->celulas[i].formula[0] = '\0';
        g->celulas[i].calculado = 1;
    }
}

// Verificar se é um número
int ehNumero(const char *str) {
    if (str[0] == '\0') return 0;
    
    int i = 0;
    if (str[0] == '-' || str[0] == '+') i = 1;
    
    int temPonto = 0;
    for (; str[i] != '\0'; i++) {
        if (str[i] == '.' && !temPonto) {
            temPonto = 1;
        } else if (str[i] < '0' || str[i] > '9') {
            return 0;
        }
    }
    return 1;
}

// Converter string para double
double stringParaDouble(const char *str) {
    double resultado = 0.0;
    double fator = 1.0;
    int negativo = 0;
    int i = 0;
    
    if (str[0] == '-') {
        negativo = 1;
        i = 1;
    } else if (str[0] == '+') {
        i = 1;
    }
    
    while (str[i] != '\0' && str[i] != '.') {
        resultado = resultado * 10 + (str[i] - '0');
        i++;
    }
    
    if (str[i] == '.') {
        i++;
        while (str[i] != '\0') {
            fator /= 10.0;
            resultado += (str[i] - '0') * fator;
            i++;
        }
    }
    
    return negativo ? -resultado : resultado;
}

int verticeExiste(int v, int numV){
    if (v >= 0 && v < numV)
    {
        return 1;
    }
    
    return 0;
    
}

// Adicionar aresta (direcionada) - vértice v1 conectado ao vértice v2
void adicionarAresta(Grafo *g, int v1, int v2){
    if (verticeExiste(v1, g->num_vertices) && verticeExiste(v2, g->num_vertices))
    {
        g->matriz[v1][v2] = 1;  // v1 conectado a v2
    }
}


void removerAresta(Grafo *g, int v1, int v2){
    if (verticeExiste(v1, g->num_vertices) && verticeExiste(v2, g->num_vertices))
    {
        g->matriz[v1][v2] = 0;
    }
}

// Declarações forward
double calcularCelula(Grafo *g, int indice);
void definirCelula(Grafo *g, char coluna, int linha, const char *valor);

void exibirMatriz(Grafo *g){
    printf("\n=== PLANILHA ===\n");
    printf("    ");
    for (char c = 'A'; c <= 'H'; c++) {
        printf("%8c", c);
    }
    printf("\n");
    
    // Recalcula todas as células
    for (int i = 0; i < MAX_CELULAS; i++) {
        calcularCelula(g, i);
    }
    
    for (int linha = 1; linha <= LINHAS; linha++) {
        printf("%2d: ", linha);
        for (char coluna = 'A'; coluna <= 'H'; coluna++) {
            int indice = coordenadaParaIndice(coluna, linha);
            Celula *cel = &g->celulas[indice];
            
            if (cel->tipo == VAZIO) {
                printf("%8s", "-");
            } else {
                printf("%8.2f", cel->valor);
            }
        }
        printf("\n");
    }
    printf("\n");
}

// Exibir arestas do grafo (Matriz de Adjacências)
void exibirDependencias(Grafo *g) {
    printf("\n=== ESTRUTURA DO GRAFO (Matriz de Adjacencias) ===\n");
    printf("Mostrando vertices com arestas:\n\n");
    
    for (int i = 0; i < MAX_CELULAS; i++) {
        int temAresta = 0;
        for (int j = 0; j < MAX_CELULAS; j++) {
            if (g->matriz[i][j] == 1) {
                temAresta = 1;
                break;
            }
        }
        
        if (temAresta) {
            char colOrigem, colDestino;
            int linOrigem, linDestino;
            indiceParaCoordenada(i, &colOrigem, &linOrigem);
            
            printf("Vertice %c%d conectado por arestas a: ", colOrigem, linOrigem);
            for (int j = 0; j < MAX_CELULAS; j++) {
                if (g->matriz[i][j] == 1) {
                    indiceParaCoordenada(j, &colDestino, &linDestino);
                    printf("%c%d ", colDestino, linDestino);
                }
            }
            printf("\n");
        }
    }
    printf("\n");
}

// Calcular valor de uma célula com base em sua fórmula
double calcularCelula(Grafo *g, int indice) {
    if (indice < 0 || indice >= MAX_CELULAS) return 0.0;
    
    Celula *cel = &g->celulas[indice];
    
    if (cel->calculado && cel->tipo != VAZIO) {
        return cel->valor;
    }
    
    switch (cel->tipo) {
        case NUMERO:
            cel->calculado = 1;
            return cel->valor;
            
        case REFERENCIA: {
            char coluna = cel->formula[1];
            int linha = 0;
            for (int i = 2; cel->formula[i] != '\0'; i++) {
                linha = linha * 10 + (cel->formula[i] - '0');
            }
            int indiceRef = coordenadaParaIndice(coluna, linha);
            if (indiceRef >= 0) {
                cel->valor = calcularCelula(g, indiceRef);
            }
            cel->calculado = 1;
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
            
            char col1 = inicio[0];
            int lin1 = 0, i = 1;
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
            double max_val = -999999.0;
            double min_val = 999999.0;
            
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
            cel->calculado = 1;
            return resultado;
        }
        
        case VAZIO:
        default:
            return 0.0;
    }
}

// Definir valor de uma célula
void definirCelula(Grafo *g, char coluna, int linha, const char *valor) {
    int indice = coordenadaParaIndice(coluna, linha);
    if (indice < 0) {
        printf("Coordenada invalida: %c%d\n", coluna, linha);
        return;
    }
    
    Celula *cel = &g->celulas[indice];
    
    // Limpar dependências antigas
    for (int i = 0; i < MAX_CELULAS; i++) {
        removerAresta(g, indice, i);
    }
    
    // Marcar para recalcular
    for (int i = 0; i < MAX_CELULAS; i++) {
        if (g->celulas[i].tipo != NUMERO && g->celulas[i].tipo != VAZIO) {
            g->celulas[i].calculado = 0;
        }
    }
    
    if (ehNumero(valor)) {
        cel->tipo = NUMERO;
        cel->valor = stringParaDouble(valor);
        cel->calculado = 1;
        cel->formula[0] = '\0';
        
    } else if (valor[0] == '=') {
        cel->tipo = REFERENCIA;
        cel->calculado = 0;
        
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
        cel->calculado = 0;
        
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
        cel->calculado = 1;
        cel->formula[0] = '\0';
    }
    
    printf("Celula %c%d definida com sucesso!\n", coluna, linha);
}

int main() {
    Grafo g;
    inicializarGrafo(&g);
    
    char entrada[200];
    char coluna;
    int linha;
    char valor[100];
    
    printf("=== PLANILHA DE CALCULO ===\n");
    printf("Colunas: A ate H\n");
    printf("Linhas: 1 ate 20\n");
    printf("Formatos de entrada:\n");
    printf("  - Numero: A5 42.5\n");
    printf("  - Referencia: A5 =B10\n");
    printf("  - Formulas: A5 @soma(B1..D3), @max(A1..A5), @min(C1..C10), @media(B2..E4)\n");
    printf("  - Digite 'mostrar' para exibir a planilha\n");
    printf("  - Digite 'dep' para mostrar dependencias\n");
    printf("  - Digite 'sair' para encerrar\n\n");
        
    exibirMatriz(&g);
    
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
            exibirMatriz(&g);
            continue;
        }
        
        if (entrada[0] == 'd' && entrada[1] == 'e' && entrada[2] == 'p') {
            exibirDependencias(&g);
            continue;
        }
        
        if (entrada[0] >= 'A' && entrada[0] <= 'H') {
            coluna = entrada[0];
            
            int i = 1;
            linha = 0;
            while (entrada[i] >= '0' && entrada[i] <= '9') {
                linha = linha * 10 + (entrada[i] - '0');
                i++;
            }
            
            while (entrada[i] == ' ') i++;
            
            int j = 0;
            while (entrada[i] != '\0' && j < 99) {
                valor[j] = entrada[i];
                i++;
                j++;
            }
            valor[j] = '\0';
            
            if (linha >= 1 && linha <= 20 && valor[0] != '\0') {
                definirCelula(&g, coluna, linha, valor);
                exibirMatriz(&g);
            } else {
                printf("Entrada invalida! Use formato: COLUNA LINHA VALOR (ex: A5 42)\n");
            }
        } else {
            printf("Entrada invalida! Use formato: COLUNA LINHA VALOR (ex: A5 42)\n");
        }
    }
    
    printf("Encerrando planilha...\n");
    return 0;
}
