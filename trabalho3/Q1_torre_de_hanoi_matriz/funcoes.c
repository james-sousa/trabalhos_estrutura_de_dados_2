#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <windows.h>
#include "grafos.h"


// Função p/ converter uma configuração em um índice único no grafo. 
int configuracaoParaIndice(int configuracao[], int n) {
    int indice = 0;
    for (int i = 0; i < n; i++) {
        indice = indice * 3 + (configuracao[i] - 1); // Configurações variam de 1 a 3
    }
    return indice;
}

// Função para converter um índice em uma configuração
void indiceParaConfiguracao(int indice, int configuracao[], int n) {
    for (int i = n - 1; i >= 0; i--) {
        configuracao[i] = (indice % 3) + 1;
        indice /= 3;
    }
}

// Verifica se o movimento é válido (regra da Torre de Hanói)
int movimentoValido(int configuracao[], int n, int origem, int destino) {
    int topo_orig = -1, topo_dest = -1;

    // Encontrar o disco no topo do pino "origem" e "destino"
    for (int i = 0; i < n; i++) {
        if (configuracao[i] == origem) {
            topo_orig = i;
            break;
        }
    }
    for (int i = 0; i < n; i++) {
        if (configuracao[i] == destino) {
            topo_dest = i;
            break;
        }
    }

    // Movimento é válido se "origem" tem disco e "destino" está vazio ou disco de "destino" é maior
    return (topo_orig != -1 && (topo_dest == -1 || topo_orig < topo_dest));
}

// Criação do grafo como matriz de adjacência
void criarGrafo(int n, int grafo[MAX_VERTICES][MAX_VERTICES], int *grafo_criado) {
    int situacao; 
    if(*grafo_criado == 0){    
        int configuracao[n], nova_config[n];

        // Inicializar a matriz de adjacência com 0
        for (int i = 0; i < MAX_VERTICES; i++) {
            for (int j = 0; j < MAX_VERTICES; j++) {
                grafo[i][j] = 0;
            }
        }

        // Iterar por todas as configurações possíveis
        for (int i = 0; i < pow(3, n); i++) {
            indiceParaConfiguracao(i, configuracao, n);

            // Tentar mover cada disco de um pino para outro
            for (int origem = 1; origem <= 3; origem++) {
                for (int destino = 1; destino <= 3; destino++) {
                    if (origem != destino && movimentoValido(configuracao, n, origem, destino)) {
                        memcpy(nova_config, configuracao, sizeof(configuracao));

                        // Mover o disco do pino "origem" para "destino"
                        for (int d = 0; d < n; d++) {
                            if (nova_config[d] == origem) {
                                nova_config[d] = destino;
                                break;
                            }
                        }

                        int indice_origem = configuracaoParaIndice(configuracao, n);
                        int indice_destino = configuracaoParaIndice(nova_config, n);
                        grafo[indice_origem][indice_destino] = 1; // Conectar os vértices
                    }
                }
            }
        }
        (*grafo_criado) = 1; 
        situacao = 1; //sucesso
    }else{
        //O grafo já foi montado
        situacao = 0;
    }
    mensagensCriacaoGrafo(situacao);     
}

// Algoritmo de Dijkstra para encontrar o menor caminho
void dijkstra(int grafo[MAX_VERTICES][MAX_VERTICES], int inicio, int distancias[MAX_VERTICES], int anteriores[MAX_VERTICES]) {
    int visitados[MAX_VERTICES];

    // Inicializar distâncias e visitados
    for (int i = 0; i < MAX_VERTICES; i++) {
        distancias[i] = INFINITO;
        visitados[i] = 0;
        anteriores[i] = -1;
    }

    distancias[inicio] = 0;

    // Relaxamento
    for (int i = 0; i < MAX_VERTICES - 1; i++) {
        int menor_distancia = INFINITO, menor_indice = -1;

        // Encontrar o vértice com menor distância não visitado
        for (int j = 0; j < MAX_VERTICES; j++) {
            if (!visitados[j] && distancias[j] < menor_distancia) {
                menor_distancia = distancias[j];
                menor_indice = j;
            }
        }

        if (menor_indice == -1) {
            // Não há mais vértices alcançáveis
            break;
        }

        visitados[menor_indice] = 1;

        // Atualizar as distâncias dos vizinhos
        for (int j = 0; j < MAX_VERTICES; j++) {
            if (grafo[menor_indice][j] && !visitados[j]) {
                // Todas as arestas têm peso 1
                int nova_dist = distancias[menor_indice] + 1;
                if (nova_dist < distancias[j]) {
                    distancias[j] = nova_dist;
                    anteriores[j] = menor_indice;
                }
            }
        }
    }
}

// Função para medir tempo em nanossegundos. Conta os ticks.
long long tempoAtualNano() {
    LARGE_INTEGER frequencia, contador;
    QueryPerformanceFrequency(&frequencia);
    QueryPerformanceCounter(&contador);
    return (contador.QuadPart * 1000000000LL) / frequencia.QuadPart;
}

void imprimirMelhorCaminho(int grafo[MAX_VERTICES][MAX_VERTICES],int config_inicial[],int config_final[], int numero_discos,int distancias[MAX_VERTICES], int anteriores[MAX_VERTICES], int grafo_criado){
    int situacao;
    if(grafo_criado == 1){
    
        int inicio = configuracaoParaIndice(config_inicial, numero_discos);
        int fim = configuracaoParaIndice(config_final, numero_discos);

        // Roda o Dijkstra
        dijkstra(grafo, inicio, distancias, anteriores);

        // Exemplo simples: imprimir o caminho em índices (do fim para o início)
        printf("\nMenor caminho (índices de vértice): ");
        int atual = fim;
        while (atual != -1) {
            printf("%d ", atual);
            atual = anteriores[atual];
        }
        printf("\nDistancia minima: %d\n", distancias[fim]);

        // Agora imprimir em formato de configuração (do início para o fim)
        printf("\nCaminho em formato de configuracoes:\n");
        int caminho_temp[MAX_VERTICES];
        int count = 0;
        atual = fim;
        while (atual != -1) {
            caminho_temp[count++] = atual;
            atual = anteriores[atual];
        }
        // Imprime invertendo (para ficar do início ao fim)
        for (int i = count - 1; i >= 0; i--) {
            int config[4]; // se numero_discos=4, aqui poderia ser config[numero_discos]
            indiceParaConfiguracao(caminho_temp[i], config, numero_discos);
            printf("[");
            for (int d = 0; d < numero_discos; d++) {
                printf("%d", config[d]);
                if (d < numero_discos - 1) printf(",");
            }
            printf("] ");
        }
        printf("\n");
        situacao = 1; //sucesso
    }else{
        //Grafo não criado
        situacao = 0;
    }

    mensagemOperacao(situacao);     
}

void medirTempoDijkstra(int grafo[MAX_VERTICES][MAX_VERTICES], int config_inicial[], int numero_discos, int distancias[MAX_VERTICES], int anteriores[MAX_VERTICES], int grafo_criado){
    int situacao;
    if(grafo_criado == 1){
        long long t1, t2;

        int inicio = configuracaoParaIndice(config_inicial, numero_discos);

        // Marca tempo inicial
        t1 = tempoAtualNano();

        // Executa Dijkstra
        dijkstra(grafo, inicio, distancias, anteriores);

        // Marca tempo final
        t2 = tempoAtualNano();

        // Imprime o tempo em nanossegundos
        printf("\nTempo para executar Dijkstra: %lld ns\n", (t2 - t1));
        situacao = 1; //sucesso
    }else{
        //Grafo não criado
        situacao = 0;
    }   
    mensagemOperacao(situacao);  
}