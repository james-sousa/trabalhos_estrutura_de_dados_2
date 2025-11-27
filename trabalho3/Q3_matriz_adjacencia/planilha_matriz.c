#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#define COLUNAS 8
#define LINHAS 20
#define MAX_CELULAS (COLUNAS * LINHAS)
#define MAX_FORMULA 100
#define NUM_REPETICOES_Q6 30 // p/ os testes.

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

// verifica se é um número
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

// Busca em Profundidade (DFS - Depth-First Search)
// Encontra todas as células que dependem (direta ou indiretamente) de uma célula
void dfs(Grafo *g, int vertice, int visitados[]) {
    if (vertice < 0 || vertice >= g->num_vertices) return;
    
    visitados[vertice] = 1;
    char col;
    int lin;
    indiceParaCoordenada(vertice, &col, &lin);
    printf("%c%d ", col, lin);
    
    // Recursivamente visita os vizinhos não visitados
    for (int i = 0; i < g->num_vertices; i++) {
        if (g->matriz[vertice][i] == 1 && !visitados[i]) {
            dfs(g, i, visitados);
        }
    }
}

void buscarDFS(Grafo *g, char coluna, int linha) {
    int indice = coordenadaParaIndice(coluna, linha);
    if (indice < 0) {
        printf("Célula inválida: %c%d\n", coluna, linha);
        return;
    }
    
    int visitados[MAX_CELULAS] = {0};
    printf("\n=== BUSCA EM PROFUNDIDADE (DFS) - Dependências de %c%d ===\n", coluna, linha);
    printf("Células que %c%d depende (ordem DFS): ", coluna, linha);
    dfs(g, indice, visitados);
    printf("\n\n");
}

// Busca em Largura (BFS - Breadth-First Search)
// Encontra todas as células que dependem (direta ou indiretamente) de uma célula
void bfs(Grafo *g, int verticeInicio) {
    if (verticeInicio < 0 || verticeInicio >= g->num_vertices) return;
    
    int visitados[MAX_CELULAS] = {0};
    int fila[MAX_CELULAS];
    int inicio = 0, fim = 0;
    
    // Enfileira o vértice inicial
    fila[fim++] = verticeInicio;
    visitados[verticeInicio] = 1;
    
    while (inicio < fim) {
        int vertice = fila[inicio++];
        char col;
        int lin;
        indiceParaCoordenada(vertice, &col, &lin);
        printf("%c%d ", col, lin);
        
        // Enfileira os vizinhos não visitados
        for (int i = 0; i < g->num_vertices; i++) {
            if (g->matriz[vertice][i] == 1 && !visitados[i]) {
                visitados[i] = 1;
                fila[fim++] = i;
            }
        }
    }
}

void buscarBFS(Grafo *g, char coluna, int linha) {
    int indice = coordenadaParaIndice(coluna, linha);
    if (indice < 0) {
        printf("Célula inválida: %c%d\n", coluna, linha);
        return;
    }
    
    printf("\n=== BUSCA EM LARGURA (BFS) - Dependências de %c%d ===\n", coluna, linha);
    printf("Células que %c%d depende (ordem BFS): ", coluna, linha);
    bfs(g, indice);
    printf("\n\n");
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
            
            // Inverter silenciosamente se intervalo for inválido
            if (col1 > col2) {
                char temp_col = col1;
                col1 = col2;
                col2 = temp_col;
            }
            if (lin1 > lin2) {
                int temp_lin = lin1;
                lin1 = lin2;
                lin2 = temp_lin;
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

// ============ QUESTAO 6: ANALISE DE DESEMPENHO ============
void executarQ6(Grafo *g) {
    const char *dados[20] = {
        "10.0", "20.0", "30.0", "40.0", "50.0",
        "60.0", "70.0", "80.0", "90.0", "100.0",
        "15.0", "25.0", "35.0", "45.0", "55.0",
        "65.0", "75.0", "85.0", "95.0", "105.0"
    };
    
    printf("\n=== QUESTAO 6: ANALISE DE DESEMPENHO (MATRIZ) ===\n");
    printf("Numero de repeticoes: %d\n\n", NUM_REPETICOES_Q6);
    
    // (a) Tempo de inserção de dados
    printf("(a) TEMPO DE INSERCAO DE DADOS\n");
    
    clock_t inicio_insercao = clock();
    
    for (int rep = 0; rep < NUM_REPETICOES_Q6; rep++) {
        Grafo g_teste;
        inicializarGrafo(&g_teste);
        
        // Inserir 20 dados
        for (int i = 0; i < 20; i++) {
            char col = 'A';
            int lin = i + 1;
            definirCelula(&g_teste, col, lin, dados[i]);
        }
    }
    
    clock_t fim_insercao = clock();
    double tempo_insercao_total = ((double)(fim_insercao - inicio_insercao)) / CLOCKS_PER_SEC;
    double tempo_insercao_medio = tempo_insercao_total / NUM_REPETICOES_Q6;
    double tempo_insercao_ns = (tempo_insercao_medio * 1000000000.0);
    
    printf("\nTempo total: %.6f segundos (%d repeticoes)\n", tempo_insercao_total, NUM_REPETICOES_Q6);
    printf("Tempo medio por ciclo: %.6f segundos (%.2f ns)\n", tempo_insercao_medio, tempo_insercao_ns);
    printf("Tempo medio por item: %.6f segundos (%.2f ns)\n\n", tempo_insercao_medio / 20, tempo_insercao_ns / 20);
    
    // (b) Tempo de BFS
    printf("(b) TEMPO DE BUSCA EM LARGURA (BFS)\n");
    
    Grafo g_teste2;
    inicializarGrafo(&g_teste2);
    for (int i = 0; i < 20; i++) {
        definirCelula(&g_teste2, 'A', i + 1, dados[i]);
    }
    
    clock_t inicio_bfs = clock();
    
    for (int rep = 0; rep < NUM_REPETICOES_Q6; rep++) {
        bfs(&g_teste2, coordenadaParaIndice('A', 1));
    }
    
    clock_t fim_bfs = clock();
    double tempo_bfs_total = ((double)(fim_bfs - inicio_bfs)) / CLOCKS_PER_SEC;
    double tempo_bfs_medio = tempo_bfs_total / NUM_REPETICOES_Q6;
    double tempo_bfs_ns = (tempo_bfs_medio * 1000000000.0);
    
    printf("\nTempo total: %.6f segundos (%d repeticoes)\n", tempo_bfs_total, NUM_REPETICOES_Q6);
    printf("Tempo medio: %.6f segundos (%.2f ns)\n\n", tempo_bfs_medio, tempo_bfs_ns);
    
    // (c) Tempo de DFS
    printf("(c) TEMPO DE BUSCA EM PROFUNDIDADE (DFS)\n");
    
    clock_t inicio_dfs = clock();
    
    for (int rep = 0; rep < NUM_REPETICOES_Q6; rep++) {
        int visitados[MAX_CELULAS] = {0};
        dfs(&g_teste2, coordenadaParaIndice('A', 1), visitados);
    }
    
    clock_t fim_dfs = clock();
    double tempo_dfs_total = ((double)(fim_dfs - inicio_dfs)) / CLOCKS_PER_SEC;
    double tempo_dfs_medio = tempo_dfs_total / NUM_REPETICOES_Q6;
    double tempo_dfs_ns = (tempo_dfs_medio * 1000000000.0);
    
    printf("\nTempo total: %.6f segundos (%d repeticoes)\n", tempo_dfs_total, NUM_REPETICOES_Q6);
    printf("Tempo medio: %.6f segundos (%.2f ns)\n\n", tempo_dfs_medio, tempo_dfs_ns);
    
    // Comparação e análise
    printf("COMPARACAO E ANALISE DOS RESULTADOS\n");
    printf("Numero de dados insertados: 20\n");
    printf("Numero de vertices no grafo: %d\n", MAX_CELULAS);
    printf("Repeticoes: %d\n\n", NUM_REPETICOES_Q6);
    
    printf("RESUMO DOS TEMPOS MEDIOS:\n");
    printf("  Insercao: %.10f seg\n", tempo_insercao_medio);
    printf("  BFS:      %.10f seg\n", tempo_bfs_medio);
    printf("  DFS:      %.10f seg\n\n", tempo_dfs_medio);
    
    printf("ANALISE:\n");
    printf("  1. Tamanho do Grafo:\n");
    printf("     - Matriz: %d x %d = %d elementos\n", 
           MAX_CELULAS, MAX_CELULAS, MAX_CELULAS * MAX_CELULAS);
    printf("     - Complexidade Espacial: O(V^2) = O(%d)\n", MAX_CELULAS * MAX_CELULAS);
    
    printf("\n  2. Complexidade Temporal:\n");
    printf("     - Insercao: O(n * m) onde n=20, m=160\n");
    printf("     - BFS: O(V^2) no pior caso (matriz densa)\n");
    printf("     - DFS: O(V^2) no pior caso (matriz densa)\n");
    
    printf("\n  3. Comparacao BFS vs DFS:\n");
    if (tempo_bfs_total > 0 && tempo_dfs_total > 0) {
        if (tempo_bfs_medio > tempo_dfs_medio) {
            double diff = ((tempo_bfs_medio - tempo_dfs_medio) / tempo_bfs_medio) * 100;
            printf("     - DFS eh %.2f%% mais rapido que BFS\n", diff);
        } else {
            double diff = ((tempo_dfs_medio - tempo_bfs_medio) / tempo_dfs_medio) * 100;
            printf("     - BFS eh %.2f%% mais rapido que DFS\n", diff);
        }
    }
    printf("     - Ambos tem mesma complexidade teorica: O(V^2)\n");
    printf("     - Diferenca pratica minima para este tamanho\n");
    
    printf("\n  4. Conclusoes:\n");
    printf("     - Operacoes sao extremamente rapidas nesta escala\n");
    printf("     - Matriz suporta O(1) para checar adjacencia\n");
    printf("     - Mas consome muita memoria: %d bytes\n", 
           MAX_CELULAS * MAX_CELULAS * sizeof(int));
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
    printf("  - Digite 'dfs COLUNA LINHA' para busca em profundidade (ex: dfs A5)\n");
    printf("  - Digite 'bfs COLUNA LINHA' para busca em largura (ex: bfs A5)\n");
    printf("  - Digite 'q6' para executar Questao 6 (Analise de Desempenho)\n");
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
        
        // Comando DFS
        if (entrada[0] == 'd' && entrada[1] == 'f' && entrada[2] == 's') {
            int i = 3;
            while (entrada[i] == ' ') i++;
            
            if (entrada[i] >= 'A' && entrada[i] <= 'H') {
                char colDFS = entrada[i];
                i++;
                int linDFS = 0;
                while (entrada[i] >= '0' && entrada[i] <= '9') {
                    linDFS = linDFS * 10 + (entrada[i] - '0');
                    i++;
                }
                buscarDFS(&g, colDFS, linDFS);
            } else {
                printf("Uso: dfs COLUNA LINHA (ex: dfs A5)\n");
            }
            continue;
        }
        
        // Comando Q6
        if (entrada[0] == 'q' && entrada[1] == '6') {
            executarQ6(&g);
            continue;
        }
        
        // Comando BFS
        if (entrada[0] == 'b' && entrada[1] == 'f' && entrada[2] == 's') {
            int i = 3;
            while (entrada[i] == ' ') i++;
            
            if (entrada[i] >= 'A' && entrada[i] <= 'H') {
                char colBFS = entrada[i];
                i++;
                int linBFS = 0;
                while (entrada[i] >= '0' && entrada[i] <= '9') {
                    linBFS = linBFS * 10 + (entrada[i] - '0');
                    i++;
                }
                buscarBFS(&g, colBFS, linBFS);
            } else {
                printf("Uso: bfs COLUNA LINHA (ex: bfs A5)\n");
            }
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
