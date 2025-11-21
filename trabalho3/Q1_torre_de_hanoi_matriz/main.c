#include <stdio.h>
#include "grafos.h"



int main(){
    
    int op1, grafo_criado, n_discos;
    grafo_criado = 0; //não foi criado ainda
    n_discos = 4;
    // A matriz de adjacência
    static int grafo[MAX_VERTICES][MAX_VERTICES];

    // Vetores auxiliares para Dijkstra
    int distancias[MAX_VERTICES];
    int anteriores[MAX_VERTICES];

    // Exemplo de configurações (inicial e final)
    int config_inicial[4] = {1, 1, 1, 1}; 
    int config_final[4] = {3, 3, 3, 3};


    do{ 
      menu_principal(); 
      scanf("%d", &op1); 

      switch(op1){
        case 1: 
           //Montar o grafo (Dijkstra)
           criarGrafo(n_discos, grafo, &grafo_criado);
           break;
        case 2: 
           //imprimir o melhor caminho por configuração.
            imprimirMelhorCaminho(grafo, config_inicial, config_final, n_discos, distancias, anteriores, grafo_criado);
           break; 
        case 3:   
           //medir tempo 
           medirTempoDijkstra(grafo, config_inicial, n_discos, distancias, anteriores, grafo_criado);
           break; 
        case 0:  
           //Sair
           break;   
        default: 
           printf("Opcao invalida, tente novamente\n"); 
           break;            
      }
    }while(op1!=0);

    return 0; 
}