#include <stdio.h>
#include "grafos.h"

void menu_principal(){
       printf("--------------------------------------\n");
    printf("|           Torre de Hanoi           |\n");
    printf("--------------------------------------\n\n");
    printf("1. Criar Grafo\n"); 
    printf("2. Imprimir menor caminho (Dijkstra)\n"); 
    printf("3. Medir tempo de execucao \n");
    printf("0. Sair\n\n"); 
}

//msgens de informacao. 
void mensagensCriacaoGrafo(int situacao){
    if(situacao == 0){
        printf("A operacao falhou porque o grafo já tinha sido criado\n");
    }
    if(situacao == 1){
        printf("A operacaoo foi realizada com sucesso\n"); 
    }
}

void mensagemOperacao(int situacao){
    if(situacao == 0){
        printf("A operação falhou porque o grafo ainda não foi montado\n");
    }
    if(situacao == 1){
        printf("A operação foi realizada com sucesso\n"); 
    }
}