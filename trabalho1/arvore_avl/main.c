#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore_avl.h"

// -- MAIN ---
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
        printf("2. Mostrar Streams\n");
        printf("3. Buscar Stream\n");
        printf("4. Remover Stream\n");
        printf("5. Cadastrar Categoria em uma Stream\n");
        printf("6. Mostrar Categorias de uma Stream\n");
        printf("7. Cadastrar Programa em uma Categoria\n");
        printf("8. Mostrar Programas de uma Categoria\n");
        printf("9. Remover Programa\n");
        printf("10. Cadastrar Apresentador\n");
        printf("11. Mostrar Apresentadores\n");
        printf("12. Alterar Stream de Apresentador\n");
        printf("13. Mostrar Streams por Tipo de Categoria\n");
        printf("14. Mostrar Streams por Nome de Categoria\n"); 
        printf("15. Mostrar Programas por Dia e Horario\n");   
        printf("16. Mostrar Programas por Dia da Semana\n");     
        printf("17. Mostrar Apresentadores por Stream\n");     
        printf("18. Mostrar Apresentadores por Categoria\n");  
        printf("19. Mostrar Dados de um Programa\n");           
        printf("20. Remover Categoria\n");                       
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
                printf("Digite o nome da Stream para buscar: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                s = buscarStream(raiz, nome);
                if (s != NULL) {
                    printf("Encontrada: %s | Site: %s\n", s->nome, s->site);
                } else {
                    printf("Stream '%s' nao encontrada!\n", nome);
                }
                break;

            case 4:
                printf("Digite o nome da Stream para remover: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                removerStream(&raiz, nome);
                break;

            case 5:
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
                    printf("Stream nao encontrada!\n");
                }
                break;

            case 6:
                printf("Digite o nome da Stream: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                s = buscarStream(raiz, nome);
                if (s != NULL) {
                    printf("\nCategorias da Stream %s:\n", s->nome);
                    mostrarCategorias(s->categorias);
                } else {
                    printf("Stream nao encontrada!\n");
                }
                break;
            
            case 7:
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
                        printf("Digite a periodicidade: ");
                        fgets(periodicidade, 20, stdin);
                        periodicidade[strcspn(periodicidade, "\n")] = 0;
                        printf("Digite a duracao em minutos: ");
                        scanf("%d", &duracao);
                        getchar();
                        printf("Digite o horario de inicio: ");
                        fgets(horario, 20, stdin);
                        horario[strcspn(horario, "\n")] = 0;
                        printf("E ao vivo? (1-Sim, 0-Nao): ");
                        scanf("%d", &aoVivo);
                        getchar();
                        printf("Digite o nome do apresentador: ");
                        fgets(nomeApresentador, 50, stdin);
                        nomeApresentador[strcspn(nomeApresentador, "\n")] = 0;
                        
                        Apresentador *apresentador = buscarApresentador(listaApresentadores, nomeApresentador);
                        if (apresentador == NULL) {
                            printf("Apresentador nao cadastrado. Por favor, cadastre-o primeiro.\n");
                        } else if (strcmp(apresentador->categoria, nomeCategoria) != 0) {
                            printf("Apresentador trabalha em outra categoria ('%s'). Nao foi possivel cadastrar o programa.\n", apresentador->categoria);
                        } else if (strcmp(apresentador->streamAtual, nome) != 0) {
                            printf("Apresentador trabalha em outra stream ('%s'). Nao foi possivel cadastrar o programa.\n", apresentador->streamAtual);
                        } else {
                            cat->programas = cadastrarPrograma(cat->programas, nomePrograma, periodicidade, duracao, horario, aoVivo, nomeApresentador);
                        }
                    } else {
                        printf("Categoria nao encontrada!\n");
                    }
                } else {
                    printf("Stream nao encontrada!\n");
                }
                break;

            case 8:
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
                        printf("Categoria nao encontrada!\n");
                    }
                } else {
                    printf("Stream nao encontrada!\n");
                }
                break;
            
            case 9:
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
                        printf("Categoria nao encontrada!\n");
                    }
                } else {
                    printf("Stream nao encontrada!\n");
                }
                break;

            case 10:
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
                    printf("Stream '%s' nao cadastrada. Nao foi possivel cadastrar o apresentador.\n", nome);
                }
                break;

            case 11:
                printf("\n=== Apresentadores Cadastrados ===\n");
                mostrarApresentadores(listaApresentadores);
                break;

            case 12:
                printf("Digite o nome do apresentador: ");
                fgets(nomeApresentador, 50, stdin);
                nomeApresentador[strcspn(nomeApresentador, "\n")] = 0;
                printf("Digite o nome da nova stream que ele trabalha: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                if (buscarStream(raiz, nome) != NULL) {
                    alterarStreamApresentador(listaApresentadores, nomeApresentador, nome);
                } else {
                    printf("Nova stream '%s' nao encontrada. Nao foi possivel alterar.\n", nome);
                }
                break;

            case 13:   
                printf("Digite o tipo de categoria para buscar streams (ex: noticias, esporte): ");
                fgets(tipoCategoria, 30, stdin);
                tipoCategoria[strcspn(tipoCategoria, "\n")] = 0;
                printf("\nStreams com categoria do tipo '%s':\n", tipoCategoria);
                buscarStreamPorTipoCategoria(raiz, tipoCategoria);
                break;

            case 14:
                printf("Digite o nome da categoria para buscar streams: ");
                fgets(nomeCategoria, 30, stdin);
                nomeCategoria[strcspn(nomeCategoria, "\n")] = 0;
                printf("\nStreams que contem a categoria '%s':\n", nomeCategoria);
                buscarStreamPorNomeCategoria(raiz, nomeCategoria);
                break;

            case 15:
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
                        printf("Digite a periodicidade (ex: diario, semanal): ");
                        fgets(periodicidade, 20, stdin);
                        periodicidade[strcspn(periodicidade, "\n")] = 0;
                        printf("Digite o horario (ex: 10:00): ");
                        fgets(horario, 20, stdin);
                        horario[strcspn(horario, "\n")] = 0;
                        printf("\nProgramas da stream '%s' na categoria '%s' no dia/horario '%s'/'%s':\n", nome, nomeCategoria, periodicidade, horario);
                        buscarProgramasPorDiaHorario(cat->programas, periodicidade, horario);
                    } else {
                        printf("Categoria nao encontrada!\n");
                    }
                } else {
                    printf("Stream nao encontrada!\n");
                }
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
                        printf("Digite a periodicidade (ex: diario, semanal, mensal): ");
                        fgets(periodicidade, 20, stdin);
                        periodicidade[strcspn(periodicidade, "\n")] = 0;
                        printf("\nProgramas da categoria '%s' na stream '%s' com periodicidade '%s':\n", nomeCategoria, nome, periodicidade);
                        buscarProgramasPorDiaSemana(cat->programas, periodicidade);
                    } else {
                        printf("Categoria nao encontrada!\n");
                    }
                } else {
                    printf("Stream nao encontrada!\n");
                }
                break;
            
            case 17:
                printf("Digite o nome da Stream para listar os apresentadores: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                printf("\nApresentadores da stream '%s':\n", nome);
                mostrarApresentadoresPorStream(listaApresentadores, nome);
                break;

            case 18:
                printf("Digite o nome da Categoria para listar os apresentadores: ");
                fgets(nomeCategoria, 30, stdin);
                nomeCategoria[strcspn(nomeCategoria, "\n")] = 0;
                printf("\nApresentadores da categoria '%s':\n", nomeCategoria);
                mostrarApresentadoresPorCategoria(listaApresentadores, nomeCategoria);
                break;

            case 19:
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
                        mostrarDadosPrograma(cat->programas, nomePrograma);
                    } else {
                        printf("Categoria nao encontrada!\n");
                    }
                } else {
                    printf("Stream nao encontrada!\n");
                }
                break;

            case 20:
                printf("Digite o nome da Stream: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;
                s = buscarStream(raiz, nome);
                if (s != NULL) {
                    printf("Digite o nome da Categoria a ser removida: ");
                    fgets(nomeCategoria, 30, stdin);
                    nomeCategoria[strcspn(nomeCategoria, "\n")] = 0;
                    removerCategoria(s, nomeCategoria);
                } else {
                    printf("Stream nao encontrada!\n");
                }
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