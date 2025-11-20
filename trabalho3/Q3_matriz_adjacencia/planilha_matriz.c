#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
// Ultima alteracao: 20/11 
#define COLUNAS 8
#define LINHAS 20
#define N_CELULAS (COLUNAS * LINHAS)
#define MAX_EXPR 128
#define INF 1000000000

typedef enum {TC_VAZIA, TC_NUMERO, TC_REF, TC_FUNC} TipoCelula;
typedef enum {TF_SOMA, TF_MAX, TF_MIN, TF_MEDIA, TF_DESCONHECIDA} TipoFuncao;

typedef struct {
    TipoCelula tipo;
    char expr[MAX_EXPR]; 
    double numero;         
    int refIndex;          
    TipoFuncao func;         
    int deps[N_CELULAS];     
    int depsCont;
} Cell;

int grafo[N_CELULAS][N_CELULAS]; 
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
    int linha = 0;
    int i = 1;
    while (s[i] && isdigit((unsigned char)s[i])) {
        linha = linha * 10 + (s[i] - '0');
        i++;
    }
    if (linha < 1 || linha > LINHAS) return -1;
    return col * LINHAS + (linha - 1);
}
void indiceParaCelulaNome(int idx, char *saida) {
    int col = idx / LINHAS;
    int linha = (idx % LINHAS) + 1;
    sprintf(saida, "%c%d", 'A' + col, linha);
}


void inicializaPlanilha() {
    for (int i = 0; i < N_CELULAS; i++) {
        planilha[i].tipo = TC_VAZIA;
        planilha[i].expr[0] = '\0';
        planilha[i].numero = 0.0;
        planilha[i].refIndex = -1;
        planilha[i].func = TF_DESCONHECIDA;
        planilha[i].depsCont = 0;
    }
    for (int i = 0; i < N_CELULAS; i++)
        for (int j = 0; j < N_CELULAS; j++)
            grafo[i][j] = 0;
}

void atualizaGrafoCelula(int to) {
    for (int u = 0; u < N_CELULAS; u++) grafo[u][to] = 0;
    for (int k = 0; k < planilha[to].depsCont; k++) {
        int u = planilha[to].deps[k];
        if (u >= 0 && u < N_CELULAS) grafo[u][to] = 1;
    }
}

void adicionaDependencia(int destino, int dep) {
    if (dep < 0 || dep >= N_CELULAS) return;
    for (int k = 0; k < planilha[destino].depsCont; k++)
        if (planilha[destino].deps[k] == dep) return;
    planilha[destino].deps[planilha[destino].depsCont++] = dep;
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

int adicionaDependenciaIntervalo(int destino, const char *nomeInicio, const char *nomeFim) {
    int idxInicio = obterCelulaNome(nomeInicio);
    int idxFim = obterCelulaNome(nomeFim);
    if (idxInicio < 0 || idxFim < 0) return 0;
    int colInicio = idxInicio / LINHAS, linhaInicio = idxInicio % LINHAS;
    int colFim = idxFim / LINHAS, linhaFim = idxFim % LINHAS;
    if (colInicio > colFim) { int t = colInicio; colInicio = colFim; colFim = t; }
    if (linhaInicio > linhaFim) { int t = linhaInicio; linhaInicio = linhaFim; linhaFim = t; }
    for (int c = colInicio; c <= colFim; c++) {
        for (int l = linhaInicio; l <= linhaFim; l++) {
            int dep = c * LINHAS + l;
            adicionaDependencia(destino, dep);
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

int processarLinhaEntrada(const char *linha) {
    char buf[MAX_EXPR];
    strncpy(buf, linha, MAX_EXPR - 1);
    buf[MAX_EXPR - 1] = '\0';
    removerEspacos(buf);
    if (strlen(buf) == 0) return 0;
    // comandos usados: print, exit, help
    if (strcasecmp(buf, "print") == 0) return -1; // sinal para imprimir
    if (strcasecmp(buf, "sair") == 0) return -2;  // sair
    if (strcasecmp(buf, "ajuda") == 0) return -3;  // ajuda

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
    int celulaIndice = obterCelulaNome(token1);
    if (celulaIndice < 0) {
        printf("Nome de célula inválido: %s\n", token1);
        return 0;
    }
    strncpy(planilha[celulaIndice].expr, rest, MAX_EXPR - 1);
    planilha[celulaIndice].expr[MAX_EXPR - 1] = '\0';
    LimparDependencias(celulaIndice);

    if (strlen(rest) == 0) {
        planilha[celulaIndice].tipo = TC_VAZIA;
        planilha[celulaIndice].numero = 0.0;
    } else if (rest[0] == '=') {
        const char *refNome = rest + 1;
        removerEspacos((char*)refNome); 
        int r = obterCelulaNome(refNome);
        if (r < 0) {
            printf("Referência inválida: %s\n", refNome);
            planilha[celulaIndice].tipo = TC_VAZIA;
            planilha[celulaIndice].numero = 0.0;
        } else {
            planilha[celulaIndice].tipo = TC_REF;
            planilha[celulaIndice].refIndex = r;
            adicionaDependencia(celulaIndice, r);
        }
    } else if (rest[0] == '@') {
        char tmp[MAX_EXPR];
        strncpy(tmp, rest + 1, MAX_EXPR - 2);
        tmp[MAX_EXPR - 2] = '\0';
        char *abre = strchr(tmp, '(');
        char *fecha = strchr(tmp, ')');
        if (!abre || !fecha || abre > fecha) {
            printf("Função inválida: sintaxe errada\n");
            planilha[celulaIndice].tipo = TC_VAZIA;
            planilha[celulaIndice].numero = 0.0;
        } else {
            *abre = '\0';
            char *nomeFuncao = tmp;
            char argstr[MAX_EXPR];
            strncpy(argstr, abre + 1, (fecha - (abre + 1)));
            argstr[fecha - (abre + 1)] = '\0';
            removerEspacos(nomeFuncao); removerEspacos(argstr);
            TipoFuncao ft = obterFuncaoNome(nomeFuncao);
            if (ft == TF_DESCONHECIDA) {
                printf("Função desconhecida: %s\n", nomeFuncao);
                planilha[celulaIndice].tipo = TC_VAZIA;
                planilha[celulaIndice].numero = 0.0;
            } else {
                char *pontos = strstr(argstr, "..");
                if (!pontos) {
                    printf("Função inválida: intervalo esperado A1..B2\n");
                    planilha[celulaIndice].tipo = TC_VAZIA;
                    planilha[celulaIndice].numero = 0.0;
                } else {
                    char nomeInicio[16], nomeFim[16];
                    int l1 = pontos - argstr;
                    strncpy(nomeInicio, argstr, l1);
                    nomeInicio[l1] = '\0';
                    strcpy(nomeFim, pontos + 2);
                    removerEspacos(nomeInicio); removerEspacos(nomeFim);
                    if (!adicionaDependenciaIntervalo(celulaIndice, nomeInicio, nomeFim)) {
                        printf("Intervalo inválido: %s..%s\n", nomeInicio, nomeFim);
                        planilha[celulaIndice].tipo = TC_VAZIA;
                        planilha[celulaIndice].numero = 0.0;
                    } else {
                        planilha[celulaIndice].tipo = TC_FUNC;
                        planilha[celulaIndice].func = ft;
                    }
                }
            }
        }
    } else {
        char *endptr = NULL;
        double v = strtod(rest, &endptr);
        if (endptr == rest || *endptr != '\0') {
            
            printf("Valor inválido: %s\n", rest);
            planilha[celulaIndice].tipo = TC_VAZIA;
            planilha[celulaIndice].numero = 0.0;
        } else {
            planilha[celulaIndice].tipo = TC_NUMERO;
            planilha[celulaIndice].numero = v;
        }
    }

    atualizaGrafoCelula(celulaIndice);
    return 1;
}

int estado[N_CELULAS]; 
double memo[N_CELULAS];
int memoValida[N_CELULAS];
int CicloDetectado = 0;

double avaliarCelula(int idx);

double avaliarFuncao(int idx) {
    // Se não há dependências, retorna 0
    if (planilha[idx].depsCont == 0) return 0.0;

    double result = 0.0;
    int contValidos = 0;  // contador de células não vazias

    // Inicializações conforme o tipo de função
    if (planilha[idx].func == TF_SOMA || planilha[idx].func == TF_MEDIA) result = 0.0;
    if (planilha[idx].func == TF_MAX) result = -INFINITY;
    if (planilha[idx].func == TF_MIN) result = INFINITY;

    // Percorrer dependências
    for (int k = 0; k < planilha[idx].depsCont; k++) {
        int dep = planilha[idx].deps[k];

        // Pular células vazias para MIN, MAX e AVG
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
                contValidos++;   // conta apenas valores válidos para média
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

    // Cálculo da média
    if (planilha[idx].func == TF_MEDIA) {
        if (contValidos > 0)
            result = result / (double)contValidos;
        else
            result = 0.0;
    }

    // Se MIN/MAX não tiveram valores válidos, retorna 0
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
            val = 0.0 ; 
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
        printf("   %2.2c    ", 'A' + c);  
    }
    printf("\n");
    for (int l = 0; l < LINHAS; l++) {
        printf("%2d |", l + 1);
        for (int c = 0; c < COLUNAS; c++) {
            int indice = c * LINHAS + l;
            double valor = memoValida[indice] ? memo[indice] : 0.0;
            printf(" ");
            char buf[32];
            {
                double parteInteira;
                if (modf(valor, &parteInteira) == 0.0) sprintf(buf, "%.0f", valor);
                else sprintf(buf, "%.2f", valor);
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
    printf("  ajuda                 -> mostra esta ajuda\n");
}


int main() {
    inicializaPlanilha();
    char linha[256];
    printf("Planilha simples (A..H x 1..20) com grafo de dependencias (matriz de adjacencia).\n");
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
            printf("Célula atualizada: %s\n", linha);
        } else {
        }
    }
    return 0;
}