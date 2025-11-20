#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
// Q4 - Implementação com lista de adjacências. Alterado em 20/11
#define COLUNAS 8
#define LINHAS 20
#define N_CELULAS (COLUNAS * LINHAS)
#define MAX_EXPR 128
#define INF 1000000000

typedef enum {TC_VAZIA, TC_NUMERO, TC_REF, TC_FUNC} TipoCelula;
typedef enum {TF_SOMA, TF_MAX, TF_MIN, TF_MEDIA, TF_DESCONHECIDA} TipoFuncao;

// Estrutura para nó da lista de adjacências
typedef struct NoAdj {
    int vertice;
    struct NoAdj *prox;
} NoAdj;

typedef struct {
    TipoCelula tipo;
    char expr[MAX_EXPR]; 
    double numero;         
    int refIndex;          
    TipoFuncao func;         
    int deps[N_CELULAS];     
    int depsCont;
} Cell;

// Lista de adjacências (substituindo a matriz)
NoAdj *grafo[N_CELULAS]; 
Cell planilha[N_CELULAS];

int colCharParaIndice(char c) {
    c = toupper((unsigned char)c);
    if (c < 'A' || c > 'H') return -1;
    return c - 'A';
}

int obterCelulaNome(const char *s) {
    if (!s || !isalpha((unsigned char)s[0])) return -1;
    int col = colCharParaIndice(s[0]);
    if (col < 0) return -1;
    int row = 0;
    int i = 1;
    while (s[i] && isdigit((unsigned char)s[i])) {
        row = row * 10 + (s[i] - '0');
        i++;
    }
    if (row < 1 || row > LINHAS) return -1;
    return col * LINHAS + (row - 1);
}

void indiceParaCelulaNome(int idx, char *saida) {
    int col = idx / LINHAS;
    int linha = (idx % LINHAS) + 1;
    sprintf(saida, "%c%d", 'A' + col, linha);
}

// Função para adicionar aresta na lista de adjacências
void adicionarAresta(int origem, int destino) {
    // Verifica se a aresta já existe
    NoAdj *atual = grafo[origem];
    while (atual != NULL) {
        if (atual->vertice == destino) return;
        atual = atual->prox;
    }
    
    // Adiciona nova aresta no início da lista
    NoAdj *novo = (NoAdj*)malloc(sizeof(NoAdj));
    novo->vertice = destino;
    novo->prox = grafo[origem];
    grafo[origem] = novo;
}

// Função para remover todas as arestas de um vértice
void removerArestasDestino(int destino) {
    for (int i = 0; i < N_CELULAS; i++) {
        NoAdj *anterior = NULL;
        NoAdj *atual = grafo[i];
        
        while (atual != NULL) {
            if (atual->vertice == destino) {
                if (anterior == NULL) {
                    grafo[i] = atual->prox;
                } else {
                    anterior->prox = atual->prox;
                }
                NoAdj *temp = atual;
                atual = atual->prox;
                free(temp);
            } else {
                anterior = atual;
                atual = atual->prox;
            }
        }
    }
}

// Função para liberar lista de adjacências
void liberarGrafo() {
    for (int i = 0; i < N_CELULAS; i++) {
        NoAdj *atual = grafo[i];
        while (atual != NULL) {
            NoAdj *temp = atual;
            atual = atual->prox;
            free(temp);
        }
        grafo[i] = NULL;
    }
}

void inicializaPlanilha() {
    for (int i = 0; i < N_CELULAS; i++) {
        planilha[i].tipo = TC_VAZIA;
        planilha[i].expr[0] = '\0';
        planilha[i].numero = 0.0;
        planilha[i].refIndex = -1;
        planilha[i].func = TF_DESCONHECIDA;
        planilha[i].depsCont = 0;
        grafo[i] = NULL;  // Inicializa lista vazia
    }
}

void atualizaGrafoCelula(int to) {
    // Remove todas as arestas que chegam em 'to'
    removerArestasDestino(to);
    
    // Adiciona novas arestas baseadas nas dependências
    for (int k = 0; k < planilha[to].depsCont; k++) {
        int u = planilha[to].deps[k];
        if (u >= 0 && u < N_CELULAS) {
            adicionarAresta(u, to);
        }
    }
}

void adicionaDependencia(int to, int dep) {
    if (dep < 0 || dep >= N_CELULAS) return;
    for (int k = 0; k < planilha[to].depsCont; k++)
        if (planilha[to].deps[k] == dep) return;
    planilha[to].deps[planilha[to].depsCont++] = dep;
}

TipoFuncao obterFuncaoNome(const char *s) {
    if (strcasecmp(s, "soma") == 0) return TF_SOMA;
    if (strcasecmp(s, "max") == 0) return TF_MAX;
    if (strcasecmp(s, "min") == 0) return TF_MIN;
    if (strcasecmp(s, "media") == 0 || strcasecmp(s, "média") == 0) return TF_MEDIA;
    return TF_DESCONHECIDA;
}

void LimparDependencias(int idx) {
    planilha[idx].depsCont = 0;
    planilha[idx].refIndex = -1;
    planilha[idx].func = TF_DESCONHECIDA;
}

int adicionaDependenciaIntervalo(int to, const char *startName, const char *endName) {
    int sIdx = obterCelulaNome(startName);
    int eIdx = obterCelulaNome(endName);
    if (sIdx < 0 || eIdx < 0) return 0;
    int sCol = sIdx / LINHAS, sRow = sIdx % LINHAS;
    int eCol = eIdx / LINHAS, eRow = eIdx % LINHAS;
    if (sCol > eCol) { int t = sCol; sCol = eCol; eCol = t; }
    if (sRow > eRow) { int t = sRow; sRow = eRow; eRow = t; }
    for (int c = sCol; c <= eCol; c++) {
        for (int r = sRow; r <= eRow; r++) {
            int dep = c * LINHAS + r;
            adicionaDependencia(to, dep);
        }
    }
    return 1;
}

void removerEspacos(char *s) {
    int i = 0;
    while (s[i] && isspace((unsigned char)s[i])) i++;
    if (i) memmove(s, s + i, strlen(s + i) + 1);
    int len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1])) s[--len] = '\0';
}

int processarLinhaEntrada(const char *line) {
    char buf[MAX_EXPR];
    strncpy(buf, line, MAX_EXPR - 1);
    buf[MAX_EXPR - 1] = '\0';
    removerEspacos(buf);
    if (strlen(buf) == 0) return 0;
    
    if (strcasecmp(buf, "print") == 0) return -1;
    if (strcasecmp(buf, "sair") == 0) return -2;
    if (strcasecmp(buf, "ajuda") == 0) return -3;

    char token1[32] = {0};
    char rest[MAX_EXPR] = {0};
    int idx = 0;

    while (buf[idx] && !isspace((unsigned char)buf[idx]) && idx < (int)sizeof(token1)-1) {
        token1[idx] = buf[idx];
        idx++;
    }
    token1[idx] = '\0';
    while (buf[idx] && isspace((unsigned char)buf[idx])) idx++;
    strncpy(rest, buf + idx, MAX_EXPR - 1);
    rest[MAX_EXPR - 1] = '\0';
    removerEspacos(token1); removerEspacos(rest);
    
    if (strlen(token1) == 0) {
        printf("Entrada inválida. Formato: A1 10 | B2 =C3 | C1 @soma(A1..B2)\n");
        return 0;
    }
    
    int cellIdx = obterCelulaNome(token1);
    if (cellIdx < 0) {
        printf("Nome de célula inválido: %s\n", token1);
        return 0;
    }
    
    strncpy(planilha[cellIdx].expr, rest, MAX_EXPR - 1);
    planilha[cellIdx].expr[MAX_EXPR - 1] = '\0';
    LimparDependencias(cellIdx);

    if (strlen(rest) == 0) {
        planilha[cellIdx].tipo = TC_VAZIA;
        planilha[cellIdx].numero = 0.0;
    } else if (rest[0] == '=') {
        const char *refName = rest + 1;
        removerEspacos((char*)refName); 
        int r = obterCelulaNome(refName);
        if (r < 0) {
            printf("Referência inválida: %s\n", refName);
            planilha[cellIdx].tipo = TC_VAZIA;
            planilha[cellIdx].numero = 0.0;
        } else {
            planilha[cellIdx].tipo = TC_REF;
            planilha[cellIdx].refIndex = r;
            adicionaDependencia(cellIdx, r);
        }
    } else if (rest[0] == '@') {
        char tmp[MAX_EXPR];
        strncpy(tmp, rest + 1, MAX_EXPR - 2);
        tmp[MAX_EXPR - 2] = '\0';
        char *popen = strchr(tmp, '(');
        char *pclose = strchr(tmp, ')');
        if (!popen || !pclose || popen > pclose) {
            printf("Função inválida: sintaxe errada\n");
            planilha[cellIdx].tipo = TC_VAZIA;
            planilha[cellIdx].numero = 0.0;
        } else {
            *popen = '\0';
            char *funcName = tmp;
            char argstr[MAX_EXPR];
            strncpy(argstr, popen + 1, (pclose - (popen + 1)));
            argstr[pclose - (popen + 1)] = '\0';
            removerEspacos(funcName); removerEspacos(argstr);
            TipoFuncao ft = obterFuncaoNome(funcName);
            if (ft == TF_DESCONHECIDA) {
                printf("Função desconhecida: %s\n", funcName);
                planilha[cellIdx].tipo = TC_VAZIA;
                planilha[cellIdx].numero = 0.0;
            } else {
                char *dots = strstr(argstr, "..");
                if (!dots) {
                    printf("Função inválida: intervalo esperado A1..B2\n");
                    planilha[cellIdx].tipo = TC_VAZIA;
                    planilha[cellIdx].numero = 0.0;
                } else {
                    char startName[16], endName[16];
                    int l1 = dots - argstr;
                    strncpy(startName, argstr, l1);
                    startName[l1] = '\0';
                    strcpy(endName, dots + 2);
                    removerEspacos(startName); removerEspacos(endName);
                    if (!adicionaDependenciaIntervalo(cellIdx, startName, endName)) {
                        printf("Intervalo inválido: %s..%s\n", startName, endName);
                        planilha[cellIdx].tipo = TC_VAZIA;
                        planilha[cellIdx].numero = 0.0;
                    } else {
                        planilha[cellIdx].tipo = TC_FUNC;
                        planilha[cellIdx].func = ft;
                    }
                }
            }
        }
    } else {
        char *endptr = NULL;
        double v = strtod(rest, &endptr);
        if (endptr == rest || *endptr != '\0') {
            printf("Valor inválido: %s\n", rest);
            planilha[cellIdx].tipo = TC_VAZIA;
            planilha[cellIdx].numero = 0.0;
        } else {
            planilha[cellIdx].tipo = TC_NUMERO;
            planilha[cellIdx].numero = v;
        }
    }

    atualizaGrafoCelula(cellIdx);
    return 1;
}

int estado[N_CELULAS]; 
double memo[N_CELULAS];
int memoValida[N_CELULAS];
int CicloDetectado = 0;

double avaliarCelula(int idx);

double avaliarFuncao(int idx) {
    if (planilha[idx].depsCont == 0) return 0.0;

    double result = 0.0;
    int contValidos = 0;

    if (planilha[idx].func == TF_SOMA || planilha[idx].func == TF_MEDIA) result = 0.0;
    if (planilha[idx].func == TF_MAX) result = -INFINITY;
    if (planilha[idx].func == TF_MIN) result = INFINITY;

    for (int k = 0; k < planilha[idx].depsCont; k++) {
        int dep = planilha[idx].deps[k];

        if (planilha[dep].tipo == TC_VAZIA &&
            (planilha[idx].func == TF_MIN || 
             planilha[idx].func == TF_MAX || 
             planilha[idx].func == TF_MEDIA)) 
        {
            continue;
        }

        double v = avaliarCelula(dep);
        if (CicloDetectado) return 0.0;

        if (planilha[idx].func == TF_SOMA || planilha[idx].func == TF_MEDIA) {
            result += v;
            if (planilha[dep].tipo != TC_VAZIA) 
                contValidos++;
        }
        else if (planilha[idx].func == TF_MAX) {
            if (v > result) result = v;
            contValidos++;
        }
        else if (planilha[idx].func == TF_MIN) {
            if (v < result) result = v;
            contValidos++;
        }
    }

    if (planilha[idx].func == TF_MEDIA) {
        if (contValidos > 0)
            result = result / (double)contValidos;
        else
            result = 0.0;
    }

    if ((planilha[idx].func == TF_MIN || planilha[idx].func == TF_MAX) &&
        contValidos == 0) 
    {
        result = 0.0;
    }

    return result;
}

double avaliarCelula(int idx) {
    if (idx < 0 || idx >= N_CELULAS) return 0.0;
    if (memoValida[idx]) return memo[idx];
    if (estado[idx] == 1) {
        CicloDetectado = 1;
        return 0.0;
    }
    if (estado[idx] == 2) {
        return memo[idx];
    }
    estado[idx] = 1; 
    double val = 0.0;
    switch (planilha[idx].tipo) {
        case TC_VAZIA:
            val = 0.0; 
            break;
        case TC_NUMERO:
            val = planilha[idx].numero;
            break;
        case TC_REF:
            if (planilha[idx].refIndex >= 0 && planilha[idx].refIndex < N_CELULAS) {
                val = avaliarCelula(planilha[idx].refIndex);
            } else val = 0.0;
            break;
        case TC_FUNC:
            val = avaliarFuncao(idx);
            break;
        default:
            val = 0.0;
    }
    estado[idx] = 2; 
    memo[idx] = val;
    memoValida[idx] = 1;
    return val;
}

void avaliarTudo() {
    for (int i = 0; i < N_CELULAS; i++) {
        estado[i] = 0;
        memoValida[i] = 0;
        memo[i] = 0.0;
    }
    CicloDetectado = 0;
    for (int i = 0; i < N_CELULAS; i++) {
        if (!memoValida[i]) {
            avaliarCelula(i);
            if (CicloDetectado) break;
        }
    }
}

void imprimirValor(double valor) {
    double parteInteira;
    if (modf(valor, &parteInteira) == 0.0) {
        printf("%.0f", valor);
    } else {
        printf("%.2f", valor);
    }
}

void imprimirPlanilha() {
    avaliarTudo();
    if (CicloDetectado) {
        printf("ERRO: dependência circular detectada! Avaliação interrompida.\n");
    }
    printf("\n     ");
    for (int c = 0; c < COLUNAS; c++) {
        printf("   %c    ", 'A' + c);
    }
    printf("\n");
    for (int r = 0; r < LINHAS; r++) {
        printf("%2d |", r + 1);
        for (int c = 0; c < COLUNAS; c++) {
            int idx = c * LINHAS + r;
            double v = memoValida[idx] ? memo[idx] : 0.0;
            printf(" ");
            char buf[32];
            {
                double iv;
                if (modf(v, &iv) == 0.0) sprintf(buf, "%.0f", v);
                else sprintf(buf, "%.2f", v);
            }
            printf("%6s", buf);
            printf(" ");
            printf("|");
        }
        printf("\n");
    }
    printf("\n");
}

void imprimeAjuda() {
    printf("Comandos/entradas válidas:\n");
    printf("  A1 10                -> escreve número 10 na célula A1\n");
    printf("  D1 =A1               -> D1 referencia diretamente A1\n");
    printf("  C1 @soma(A1..B2)     -> função soma do intervalo A1..B2\n");
    printf("  Funções suportadas: @soma, @max, @min, @media\n");
    printf("  print                -> exibe a planilha atual\n");
    printf("  sair                 -> sai\n");
    printf("  ajuda                -> mostra esta ajuda\n");
    printf("\n*** IMPLEMENTAÇÃO COM LISTA DE ADJACÊNCIAS ***\n");
}

int main() {
    inicializaPlanilha();
    char linha[256];
    printf("Planilha simples (A..H x 1..20) com grafo de dependencias (LISTA DE ADJACÊNCIAS).\n");
    printf("Digite 'ajuda' para instrucoes, 'print' para mostrar planilha, 'sair' para encerrar.\n");
    while (1) {
        printf("\n> ");
        if (!fgets(linha, sizeof(linha), stdin)) break;
        linha[strcspn(linha, "\n")] = '\0';
        removerEspacos(linha);
        if (strlen(linha) == 0) continue;
        int res = processarLinhaEntrada(linha);
        if (res == -2) { 
            printf("Saindo...\n");
            break;
        } else if (res == -1) { 
            imprimirPlanilha();
        } else if (res == -3) {
            imprimeAjuda();
        } else if (res == 1) {
            avaliarTudo();
            if (CicloDetectado) {
                printf("ALERTA: dependência circular detectada. Corrija as células envolvidas.\n");
            }
            printf("Célula atualizada.\n");
        }
    }
    liberarGrafo();
    return 0;
}