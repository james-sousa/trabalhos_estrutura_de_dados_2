#include <limits.h>
#ifndef _GRAFOS_H
#define _GRAFOS_H

#define MAX_VERTICES 81 // Para 4 discos, existem 3^4 = 81 configurações possíveis
#define INFINITO INT_MAX


//Prototipo dos Menus
void menu_principal(); 

//protótipo das funções de diagnóstico
void mensagensCriacaoGrafo(int situacao);
void mensagemOperacao(int situacao);


//Protótipo das Funções
int configuracaoParaIndice(int configuracao[], int n); //Função para converter uma configuração em um índice único no grafo
void indiceParaConfiguracao(int indice, int configuracao[], int n);// Função para converter um índice em uma configuração
int movimentoValido(int configuracao[], int n, int origem, int destino); //// Verifica se o movimento é válido (regra da Torre de Hanói)
void criarGrafo(int n, int grafo[MAX_VERTICES][MAX_VERTICES], int *grafo_criado);  //// Criação do grafo como matriz de adjacência
void dijkstra(int grafo[MAX_VERTICES][MAX_VERTICES], int inicio, int distancias[MAX_VERTICES], int anteriores[MAX_VERTICES]); //// Algoritmo de Dijkstra para encontrar o menor caminho(Ótimo)
long long tempoAtualNano(); //Função para medir tempo em nanossegundos
void imprimirMelhorCaminho(int grafo[MAX_VERTICES][MAX_VERTICES],int config_inicial[],int config_final[], int numero_discos,int distancias[MAX_VERTICES], int anteriores[MAX_VERTICES], int grafo_criado); 
void medirTempoDijkstra(int grafo[MAX_VERTICES][MAX_VERTICES], int config_inicial[], int numero_discos, int distancias[MAX_VERTICES], int anteriores[MAX_VERTICES], int grafo_criado); 


#endif