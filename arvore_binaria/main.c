#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arv_binaria.h"

/// MENU ATUALIZADO
int main() {
    Stream *raiz = NULL;
    Apresentador *listaApresentadores = NULL;
    int opcao;
    char nome[50], site[100];
    char nomeCategoria[30], tipoCategoria[30];
    char nomePrograma[30], periodicidade[20], horario[20], nomeApresentador[50];
    int duracao, aoVivo;
    Stream *s;
    Categoria *cat;
    
    do {
        printf("\n===== MENU STREAMING =====\n");
        printf("1. Cadastrar Stream\n");
        printf("2. Mostrar todas as streams cadastradas\n");
        printf("3. Cadastrar Categoria para uma stream\n");
        printf("4. Mostrar categorias de uma stream\n");
        printf("5. Cadastrar Programa para uma categoria\n");
        printf("6. Mostrar programas de uma categoria\n");
        printf("7. Cadastrar Apresentador\n");
        printf("8. Mostrar todos os apresentadores\n");
        printf("9. Mostrar streams com uma categoria especifica\n");
        printf("10. Mostrar programas de uma stream por dia e horario\n");
        printf("11. Mostrar streams por tipo de categoria\n");
        printf("12. Mostrar programas de uma categoria por dia da semana e horario\n");
        printf("13. Mostrar apresentadores de uma stream\n");
        printf("14. Mostrar apresentadores de uma categoria\n");
        printf("15. Mostrar dados de um programa\n");
        printf("16. Remover um programa\n");
        printf("17. Remover uma categoria\n");
        printf("18. Alterar stream de um apresentador\n");
        printf("19. Remover Stream\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                printf("Digite o nome da Stream: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                printf("Digite o site da Stream: ");
                fgets(site, 100, stdin);
                site[strcspn(site, "\n")] = 0;
                cadastrarStream(&raiz, nome, site);
                
                break;

            case 2:
                printf("\n=== Streams cadastradas (ordem alfabetica) ===\n");
                mostrarStream(raiz);
                break;

            case 3:
                printf("Digite o nome da Stream: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                s = buscarStream(raiz, nome);
                if (s != NULL) {
                    printf("Digite o nome da categoria: ");
                    fgets(nomeCategoria, 30, stdin);
                    nomeCategoria[strcspn(nomeCategoria, "\n")] = 0;
                    printf("Digite o tipo da categoria: ");
                    fgets(tipoCategoria, 30, stdin);
                    tipoCategoria[strcspn(tipoCategoria, "\n")] = 0;
                    s->categorias = cadastrarCategoria(s->categorias, nomeCategoria, tipoCategoria);
                } else {
                    printf("Erro: Stream nao encontrada!\n");
                }
                break;

            case 4:
                printf("Digite o nome da Stream: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                s = buscarStream(raiz, nome);
                if (s != NULL) {
                    printf("\nCategorias da Stream %s:\n", s->nome);
                    mostrarCategorias(s->categorias);
                } else {
                    printf("Erro: Stream nao encontrada!\n");
                }
                break;
            
            case 5:
                printf("Digite o nome da Stream: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                s = buscarStream(raiz, nome);
                if (s != NULL) {
                    printf("Digite o nome da Categoria: ");
                    fgets(nomeCategoria, 30, stdin);
                    nomeCategoria[strcspn(nomeCategoria, "\n")] = 0;
                    cat = buscarCategoria(s->categorias, nomeCategoria);
                    if (cat != NULL) {
                        printf("Digite o nome do Programa: ");
                        fgets(nomePrograma, 30, stdin);
                        nomePrograma[strcspn(nomePrograma, "\n")] = 0;
                        printf("Digite a periodicidade (diario, semanal, mensal): ");
                        fgets(periodicidade, 20, stdin);
                        periodicidade[strcspn(periodicidade, "\n")] = 0;
                        printf("Digite a duracao em minutos: ");
                        scanf("%d", &duracao);
                        getchar();
                        printf("Digite o horario de inicio (HH:MM): ");
                        fgets(horario, 20, stdin);
                        horario[strcspn(horario, "\n")] = 0;
                        printf("E ao vivo? (1-Sim, 0-Nao): ");
                        scanf("%d", &aoVivo);
                        getchar();
                        printf("Digite o nome do apresentador: ");
                        fgets(nomeApresentador, 50, stdin);
                        nomeApresentador[strcspn(nomeApresentador, "\n")] = 0;
                        
                        if (buscarApresentador(listaApresentadores, nomeApresentador) == NULL) {
                            printf("Erro: Apresentador nao cadastrado. Por favor, cadastre-o primeiro.\n");
                        } else {
                            cat->programas = cadastrarPrograma(cat->programas, nomePrograma, periodicidade, duracao, horario, aoVivo, nomeApresentador, listaApresentadores, nome, nomeCategoria);
                        }
                    } else {
                        printf("Erro: Categoria nao encontrada!\n");
                    }
                } else {
                    printf("Erro: Stream nao encontrada!\n");
                }
                break;

            case 6:
                printf("Digite o nome da Stream: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                s = buscarStream(raiz, nome);
                if (s != NULL) {
                    printf("Digite o nome da Categoria: ");
                    fgets(nomeCategoria, 30, stdin);
                    nomeCategoria[strcspn(nomeCategoria, "\n")] = 0;
                    cat = buscarCategoria(s->categorias, nomeCategoria);
                    if (cat != NULL) {
                        printf("\nProgramas da categoria '%s' da stream '%s':\n", nomeCategoria, nome);
                        mostrarProgramas(cat->programas);
                    } else {
                        printf("Erro: Categoria nao encontrada!\n");
                    }
                } else {
                    printf("Erro: Stream nao encontrada!\n");
                }
                break;
            
            case 7:
                printf("Digite o nome do Apresentador: ");
                fgets(nomeApresentador, 50, stdin);
                nomeApresentador[strcspn(nomeApresentador, "\n")] = 0;
                printf("Digite a categoria que ele trabalha: ");
                fgets(nomeCategoria, 30, stdin);
                nomeCategoria[strcspn(nomeCategoria, "\n")] = 0;
                printf("Digite a stream que ele trabalha atualmente: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                if (buscarStream(raiz, nome) != NULL) {
                    listaApresentadores = cadastrarApresentador(listaApresentadores, nomeApresentador, nomeCategoria, nome);
                } else {
                    printf("Erro: Stream '%s' nao cadastrada. Nao foi possivel cadastrar o apresentador.\n", nome);
                }
                break;

            case 8:
                printf("\n=== Apresentadores Cadastrados ===\n");
                mostrarApresentadores(listaApresentadores);
                break;

            case 9:
                printf("Digite o nome da categoria: ");
                fgets(nomeCategoria, 30, stdin);
                nomeCategoria[strcspn(nomeCategoria, "\n")] = 0;
                printf("\nStreams que contem a categoria '%s':\n", nomeCategoria);
                buscarStreamsPorCategoria(raiz, nomeCategoria);
                break;

            case 10:
                printf("Digite o horario de inicio (ex: 20:00): ");
                fgets(horario, 20, stdin);
                horario[strcspn(horario, "\n")] = 0;
                printf("\nProgramas que ocorrem as %s:\n", horario);
                mostrarProgramasPorHorario(raiz, horario);
                break;
            
            case 11:
                printf("Digite o tipo de categoria (ex: noticias, esporte): ");
                fgets(tipoCategoria, 30, stdin);
                tipoCategoria[strcspn(tipoCategoria, "\n")] = 0;
                printf("\nStreams com categoria do tipo '%s':\n", tipoCategoria);
                buscarStreamPorTipoCategoria(raiz, tipoCategoria);
                break;

            case 12:
                printf("Digite o nome da categoria: ");
                fgets(nomeCategoria, 30, stdin);
                nomeCategoria[strcspn(nomeCategoria, "\n")] = 0;
                printf("Digite o horario de inicio (ex: 20:00): ");
                fgets(horario, 20, stdin);
                horario[strcspn(horario, "\n")] = 0;
                mostrarProgramasPorDiaSemanaCategoria(raiz, nomeCategoria, horario);
                break;
            
            case 13:
                printf("Digite o nome da Stream: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                printf("\nApresentadores que trabalham na stream '%s':\n", nome);
                mostrarApresentadoresPorStream(listaApresentadores, nome);
                break;

            case 14:
                printf("Digite o nome da Categoria: ");
                fgets(nomeCategoria, 30, stdin);
                nomeCategoria[strcspn(nomeCategoria, "\n")] = 0;
                printf("\nApresentadores que trabalham na categoria '%s':\n", nomeCategoria);
                mostrarApresentadoresPorCategoria(listaApresentadores, nomeCategoria);
                break;
            
            case 15:
                printf("Digite o nome da Stream: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                printf("Digite o nome da Categoria: ");
                fgets(nomeCategoria, 30, stdin);
                nomeCategoria[strcspn(nomeCategoria, "\n")] = 0;
                printf("Digite o nome do Programa: ");
                fgets(nomePrograma, 30, stdin);
                nomePrograma[strcspn(nomePrograma, "\n")] = 0;
                mostrarDadosPrograma(raiz, nome, nomeCategoria, nomePrograma);
                break;
            
            case 16:
                printf("Digite o nome da Stream: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                s = buscarStream(raiz, nome);
                if (s != NULL) {
                    printf("Digite o nome da Categoria: ");
                    fgets(nomeCategoria, 30, stdin);
                    nomeCategoria[strcspn(nomeCategoria, "\n")] = 0;
                    cat = buscarCategoria(s->categorias, nomeCategoria);
                    if (cat != NULL) {
                        printf("Digite o nome do Programa a ser removido: ");
                        fgets(nomePrograma, 30, stdin);
                        nomePrograma[strcspn(nomePrograma, "\n")] = 0;
                        cat->programas = removerPrograma(cat->programas, nomePrograma);
                    } else {
                        printf("Erro: Categoria nao encontrada!\n");
                    }
                } else {
                    printf("Erro: Stream nao encontrada!\n");
                }
                break;

            case 17:
                printf("Digite o nome da Stream: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                printf("Digite o nome da Categoria a ser removida: ");
                fgets(nomeCategoria, 30, stdin);
                nomeCategoria[strcspn(nomeCategoria, "\n")] = 0;
                removerCategoria(raiz, nome, nomeCategoria);
                break;

            case 18:
                printf("Digite o nome do apresentador: ");
                fgets(nomeApresentador, 50, stdin);
                nomeApresentador[strcspn(nomeApresentador, "\n")] = 0;
                printf("Digite o nome da nova stream que ele trabalha: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                if (buscarStream(raiz, nome) != NULL) {
                    alterarStreamApresentador(raiz, listaApresentadores, nomeApresentador, nome);
                } else {
                    printf("Erro: Nova stream '%s' nao encontrada. Nao foi possivel alterar.\n", nome);
                }
                break;

            case 19:
                printf("Digite o nome da Stream para remover: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                removerStream(&raiz, nome);
                break;
            
            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}