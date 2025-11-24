#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

#define NUM_DISCOS 4                      // Número de discos
#define NUM_PINOS 3                       // Número de pinos
#define CONFIGURACAO_MAXIMA 3 * 3 * 3 * 3 // NUM_PINOS ^ NUM_DISCOS (número total de configurações possíveis)
#define INFINITO INT_MAX

typedef struct
{
  int configuracao[NUM_DISCOS];
} Vertice;

// Estrutura para nó da lista de adjacência
typedef struct No
{
  int vertice_destino;
  struct No *proximo;
} No;

// Estrutura para lista de adjacência de um vértice
typedef struct
{
  No *inicio;
} ListaAdj;

bool movimentoValido(Vertice vertice1, Vertice vertice2)
{
  bool valido = true;

  int diferenca = 0, indiceDiferente = -1;

  for (int indiceConfiguracao = 0; indiceConfiguracao < NUM_DISCOS && diferenca <= 1; ++indiceConfiguracao)
  {
    if (vertice1.configuracao[indiceConfiguracao] != vertice2.configuracao[indiceConfiguracao])
    {
      diferenca++;
      indiceDiferente = indiceConfiguracao;
    }
  }

  if (diferenca > 1)
    valido = false;
  else
  {
    int invalido = 1;
    for (int indiceVerificacao = 0; indiceVerificacao < NUM_DISCOS && invalido; indiceVerificacao++)
      if (indiceVerificacao != indiceDiferente &&
          ((vertice1.configuracao[indiceVerificacao] == vertice1.configuracao[indiceDiferente] && indiceVerificacao < indiceDiferente) ||
           (vertice2.configuracao[indiceVerificacao] == vertice2.configuracao[indiceDiferente] && indiceVerificacao < indiceDiferente)))
        invalido = 0;

    if (!invalido)
      valido = false;
  }

  return (valido);
}

void gerarConfiguracoes(Vertice *grafo, ListaAdj *listaAdj)
{
  // Inicializar listas de adjacência
  for (int i = 0; i < CONFIGURACAO_MAXIMA; i++)
    listaAdj[i].inicio = NULL;

  int disco, numero_atribuido;
  // Gerar todas as configurações
  for (int indiceConfiguracao = 0; indiceConfiguracao < CONFIGURACAO_MAXIMA; indiceConfiguracao++)
  {
    numero_atribuido = indiceConfiguracao;
    for (disco = 0; disco < NUM_DISCOS; disco++)
    {
      grafo[indiceConfiguracao].configuracao[disco] = numero_atribuido % NUM_PINOS + 1;
      numero_atribuido /= NUM_PINOS;
    }
  }

  // Construir lista de adjacência
  for (int indiceAtual = 0; indiceAtual < CONFIGURACAO_MAXIMA; indiceAtual++)
  {
    for (int indiceVizinho = 0; indiceVizinho < CONFIGURACAO_MAXIMA; indiceVizinho++)
    {
      if (movimentoValido(grafo[indiceAtual], grafo[indiceVizinho]))
      {
        // Criar novo nó
        No *novoNo = (No *)malloc(sizeof(No));
        novoNo->vertice_destino = indiceVizinho;
        novoNo->proximo = listaAdj[indiceAtual].inicio;
        listaAdj[indiceAtual].inicio = novoNo;
      }
    }
  }
}

void dijkstra(int inicio, ListaAdj *listaAdj, int *distancias, int *predecessor)
{
  int visitados[CONFIGURACAO_MAXIMA];

  // Inicializa as distâncias, visitados e predecessores
  for (int configuracao_atual = 0; configuracao_atual < CONFIGURACAO_MAXIMA; configuracao_atual++)
  {
    distancias[configuracao_atual] = INFINITO;
    visitados[configuracao_atual] = 0;
    predecessor[configuracao_atual] = -1;
  }
  distancias[inicio] = 0;

  int vertice_menor_distancia;
  int configuracao_atual = 0;
  do
  {
    vertice_menor_distancia = -1;

    for (int i = 0; i < CONFIGURACAO_MAXIMA - 1; i++)
      if (!visitados[i] && (vertice_menor_distancia == -1 || distancias[i] < distancias[vertice_menor_distancia]))
        vertice_menor_distancia = i;

    if (!(vertice_menor_distancia == -1 || distancias[vertice_menor_distancia] == INFINITO))
    {
      visitados[vertice_menor_distancia] = 1;

      // Percorrer a lista de adjacência do vértice atual
      No *aresta = listaAdj[vertice_menor_distancia].inicio;
      while (aresta != NULL)
      {
        int v = aresta->vertice_destino;
        if (!visitados[v] && distancias[vertice_menor_distancia] != INFINITO &&
            distancias[vertice_menor_distancia] + 1 < distancias[v])
        {
          distancias[v] = distancias[vertice_menor_distancia] + 1;
          predecessor[v] = vertice_menor_distancia;
        }
        aresta = aresta->proximo;
      }
    }

    configuracao_atual++;
  } while (configuracao_atual < CONFIGURACAO_MAXIMA - 1 && vertice_menor_distancia != -1);
}

void exibir_caminho(int inicio, int fim, int *distancias, int *predecessor)
{
  if (distancias[fim] == INFINITO)
  {
    printf("Não há caminho acessível de %d para %d.\n", inicio, fim);
  }
  else
  {
    // Exibe a menor distância
    printf("Menor caminho de %d para %d: %d\n", inicio, fim, distancias[fim]);

    // Exibe o caminho
    printf("Caminho: ");
    int caminho[CONFIGURACAO_MAXIMA];
    int indice = 0;
    int atual = fim;

    // Reconstrói o caminho a partir do destino
    while (atual != -1)
    {
      caminho[indice++] = atual;  // Armazena o vértice no caminho
      atual = predecessor[atual]; // Vai para o predecessor
    }

    // Exibe o caminho na ordem correta
    for (int caminhoIndice = indice - 1; caminhoIndice >= 0; caminhoIndice--)
    {
      printf("%d", caminho[caminhoIndice]);
      if (caminhoIndice > 0)
        printf(" -> ");
    }
    printf("\n");
  }
}

void exibir_config(Vertice *vertice)
{
  for (int config_atual = 0; config_atual < CONFIGURACAO_MAXIMA; ++config_atual)
  {
    printf("vertex %d: ", config_atual);
    for (int disco_atual = 0; disco_atual < NUM_DISCOS; disco_atual++)
      printf("%d ", vertice[config_atual].configuracao[disco_atual]);
    printf("\n");
  }
}

void liberar_memoria(ListaAdj *listaAdj)
{
  for (int i = 0; i < CONFIGURACAO_MAXIMA; i++)
  {
    No *atual = listaAdj[i].inicio;
    while (atual != NULL)
    {
      No *temp = atual;
      atual = atual->proximo;
      free(temp);
    }
  }
}

int main()
{
  Vertice grafo[CONFIGURACAO_MAXIMA];
  ListaAdj listaAdj[CONFIGURACAO_MAXIMA];

  gerarConfiguracoes(grafo, listaAdj);
  exibir_config(grafo);

  int inicial, final;
  printf("Digite o índice da configuração inicial (0 a %d): ", CONFIGURACAO_MAXIMA - 1);
  scanf("%d", &inicial);
  printf("Digite o índice da configuração final (0 a %d): ", CONFIGURACAO_MAXIMA - 1);
  scanf("%d", &final);

  int distancias[CONFIGURACAO_MAXIMA];
  int predecessor[CONFIGURACAO_MAXIMA];

  clock_t inicio, fim;
  inicio = clock();
  dijkstra(inicial, listaAdj, distancias, predecessor);
  fim = clock();

  exibir_caminho(inicial, final, distancias, predecessor);

  double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
  printf("Tempo do algoritmo de dijkstra: %f segundos\n", tempo);

  // Liberar memória alocada
  liberar_memoria(listaAdj);

  return 0;
}
