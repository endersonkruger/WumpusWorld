#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "arsenal.h"


int main()
{
    int switchMenuPrincipal, switchInternoJogar, escolha, pontuacao = 0, tamanhoNome, retornoAgente, retornoWumpus, qtdFlecha, qtdOuro, qtdDica, continuador;
    char nome[64], verif, movimento, opt;
    caverna tabuleiro[TAM][TAM], resetTabuleiro[TAM][TAM];
    while (true) // Menu do Jogo
    {
        system("clear");
        printf("Seja Bem Vindo ao Mundo de Wumpus!\n\nMenu Principal\n1. Jogar\n2. Debug\n3. Lista de Jogadores\n4. Sair\n\n");
        scanf("%d", &switchMenuPrincipal);
        getchar();
        switch (switchMenuPrincipal)
        {
        case 1: // Jogar
            iniciaPartida(tabuleiro, &qtdFlecha, &qtdOuro, &qtdDica);
            salvaInfos(tabuleiro, resetTabuleiro);
            system("clear");
            printf("Digite seu nome\n");
            fgets(nome, 64, stdin);
            tamanhoNome = strlen(nome);
            system("clear");
            do
            {
                system("clear");
                jogarImprimeCaverna(tabuleiro, &qtdFlecha, &qtdOuro);
                printf("\nEscolha uma das opções:\n1. Controlar Agente\n2. Dica\n3. Encerrar a sessão e voltar ao menu principal\n\n");
                scanf("%d", &switchInternoJogar);
                getchar();
                switch (switchInternoJogar)
                {
                case 1: // Controla o agente (movimenta e atira flecha)
                    do
                    {
                        system("clear");
                        jogarImprimeCaverna(tabuleiro, &qtdFlecha, &qtdOuro);
                        printf("\n\nO que deseja fazer?\n1. Movimentar\n2. Disparar Flecha\n3. Voltar\n");
                        scanf("%d", &escolha);
                        getchar();
                        switch (escolha)
                        {
                        case 1: // Movimentar
                            do
                            {
                                do
                                {
                                    system("clear");
                                    jogarImprimeCaverna(tabuleiro, &qtdFlecha, &qtdOuro);
                                    printf("\nPara onde deseja ir?\nW - Cima\nA - Esquerda\nS - Baixo\nD - Direita\n\nR - Retornar às opções\n");
                                    scanf("%c", &movimento);
                                    getchar();
                                } while (movimento != 'W' && movimento != 'w' && movimento != 'A' && movimento != 'a' && movimento != 'S' && movimento != 's' && movimento != 'D' && movimento != 'd' && movimento != 'R' && movimento != 'r');

                                if (movimento != 'R' && movimento != 'r')
                                {
                                    continuador = 1;
                                    retornoAgente = moveAgente(tabuleiro, movimento, &qtdFlecha, &qtdOuro);
                                    retornoWumpus = moveWumpus(tabuleiro);
                                }
                                else
                                {
                                    continuador = 0;
                                }
                                if (retornoAgente == 3 || retornoWumpus == 3 || retornoAgente == 4)
                                {
                                    salvarOrganizar(nome, tamanhoNome, pontuacao);
                                    printf("Você Perdeu\nPontuação: %d\nPressione:\n0 para fechar o jogo.\n1 para reiniciar o jogo.\nQualquer outra tecla para voltar ao menu principal: ", pontuacao);
                                    scanf("%c", &opt);
                                    getchar();
                                    if (opt == '0')
                                    {
                                        return 0;
                                    }
                                    else if (opt == '1')
                                    {
                                        reiniciaPartida(tabuleiro, resetTabuleiro, &qtdFlecha, &qtdOuro, &qtdDica);
                                        continuador = 0;
                                        escolha = 3;
                                        pontuacao = -1;
                                    }
                                    else
                                    {
                                        continuador = 0;
                                        escolha = 3;
                                        switchInternoJogar = 3;
                                    }
                                }
                                if (tabuleiro[7][0].agente == true && qtdOuro == 1)
                                {
                                    system("clear");
                                    printf("\nMeus parabéns, você ganhou!\nPontuação: %d\nPressione:\n0 para fechar o jogo.\n1 para reiniciar o jogo.\nQualquer outra tecla para voltar ao menu principal: ", pontuacao);
                                    scanf("%c", &opt);
                                    getchar();
                                    salvarOrganizar(nome, tamanhoNome, pontuacao);
                                    if (opt == '0')
                                    {
                                        return 0;
                                    }
                                    else if (opt == '1')
                                    {
                                        reiniciaPartida(tabuleiro, resetTabuleiro, &qtdFlecha, &qtdOuro, &qtdDica);
                                        continuador = 0;
                                        escolha = 3;
                                        pontuacao = -1;
                                    }
                                    else
                                    {
                                        continuador = 0;
                                        escolha = 3;
                                        switchInternoJogar = 3;
                                    }
                                }
                                pontuacao++;
                            } while (continuador == 1);
                            break;

                        case 2: // Disparar Flecha
                            disparaFlecha(tabuleiro, &qtdFlecha);
                            break;

                        case 3: // Voltar
                            break;

                        default:
                            system("clear");
                            printf("Opção Invalida, tente novamente!\n");
                            printf("Pressione enter para continuar . . .");
                            scanf("%c", &verif);
                            break;
                        }
                    } while (escolha != 3);
                    break;

                case 2: // Dica --> só uma utilização por game, zerar ao encerrar o jogo
                    dica(tabuleiro, &qtdDica);
                    break;

                case 3: // Encerrar o jogo, salvar nome e pontuação no ranking
                    salvarOrganizar(nome, tamanhoNome, pontuacao);
                    system("clear");
                    break;

                default:
                    system("clear");
                    printf("Opção Invalida, tente novamente!\n");
                    printf("Pressione enter para continuar . . .");
                    scanf("%c", &verif);
                    break;
                }
            } while (switchInternoJogar != 3);
            break;

        case 2: // Debug --> igual à opção jogar, mas com o mapa aberto
            iniciaPartida(tabuleiro, &qtdFlecha, &qtdOuro, &qtdDica);
            salvaInfos(tabuleiro, resetTabuleiro);
            system("clear");
            printf("Digite seu nome\n");
            fgets(nome, 64, stdin);
            tamanhoNome = strlen(nome);
            system("clear");
            do
            {
                system("clear");
                debugImprimeCaverna(tabuleiro, &qtdFlecha, &qtdOuro);
                printf("\nEscolha uma das opções:\n1. Controlar Agente\n2. Dica\n3. Encerrar a sessão e voltar ao menu principal\n\n");
                scanf("%d", &switchInternoJogar);
                getchar();
                switch (switchInternoJogar)
                {
                case 1: // Controla o agente (movimenta e atira flecha)
                    do
                    {
                        system("clear");
                        debugImprimeCaverna(tabuleiro, &qtdFlecha, &qtdOuro);
                        printf("\n\nO que deseja fazer?\n1. Movimentar\n2. Disparar Flecha\n3. Voltar\n");
                        scanf("%d", &escolha);
                        getchar();
                        switch (escolha)
                        {
                        case 1: // Movimentar
                            do
                            {
                                do
                                {
                                    system("clear");
                                    debugImprimeCaverna(tabuleiro, &qtdFlecha, &qtdOuro);
                                    printf("\nPara onde deseja ir?\nW - Cima\nA - Esquerda\nS - Baixo\nD - Direita\n\nR - Retornar às opções\n");
                                    scanf("%c", &movimento);
                                    getchar();
                                } while (movimento != 'W' && movimento != 'w' && movimento != 'A' && movimento != 'a' && movimento != 'S' && movimento != 's' && movimento != 'D' && movimento != 'd' && movimento != 'R' && movimento != 'r');
                                if (movimento != 'R' && movimento != 'r')
                                {
                                    continuador = 1;
                                    retornoAgente = moveAgente(tabuleiro, movimento, &qtdFlecha, &qtdOuro);
                                    retornoWumpus = moveWumpus(tabuleiro);
                                }
                                else
                                {
                                    continuador = 0;
                                }
                                if (retornoAgente == 3 || retornoWumpus == 3 || retornoAgente == 4)
                                {
                                    salvarOrganizar(nome, tamanhoNome, pontuacao);
                                    printf("\nPontuação: %d\nPressione:\n0 para fechar o jogo.\n1 para reiniciar o jogo.\nQualquer outra tecla para voltar ao menu principal\n", pontuacao);
                                    scanf("%c", &opt);
                                    getchar();
                                    if (opt == '0')
                                    {
                                        return 0;
                                    }
                                    else if (opt == '1')
                                    {
                                        reiniciaPartida(tabuleiro, resetTabuleiro, &qtdFlecha, &qtdOuro, &qtdDica);
                                        continuador = 0;
                                        escolha = 3;
                                        pontuacao = -1;
                                    }
                                    else
                                    {
                                        continuador = 0;
                                        escolha = 3;
                                        switchInternoJogar = 3;
                                    }
                                }
                                if (tabuleiro[7][0].agente == true && qtdOuro == 1)
                                {
                                    system("clear");
                                    printf("\nMeus parabéns, você ganhou!\nPontuação: %d\nPressione:\n0 para fechar o jogo.\n1 para reiniciar o jogo.\nQualquer outra tecla para voltar ao menu principal\n", pontuacao);
                                    scanf("%c", &opt);
                                    getchar();
                                    salvarOrganizar(nome, tamanhoNome, pontuacao);
                                    if (opt == '0')
                                    {
                                        return 0;
                                    }
                                    else if (opt == '1')
                                    {
                                        reiniciaPartida(tabuleiro, resetTabuleiro, &qtdFlecha, &qtdOuro, &qtdDica);
                                        continuador = 0;
                                        escolha = 3;
                                        pontuacao = -1;
                                    }
                                    else
                                    {
                                        continuador = 0;
                                        escolha = 3;
                                        switchInternoJogar = 3;
                                    }
                                }
                                pontuacao++;
                            } while (continuador == 1);
                            break;

                        case 2: // Disparar Flecha
                            disparaFlecha(tabuleiro, &qtdFlecha);
                            break;

                        case 3: // Voltar
                            break;

                        default:
                            system("clear");
                            printf("Opção Invalida, tente novamente!\n");
                            printf("Pressione enter para continuar . . .");
                            scanf("%c", &verif);
                            break;
                        }
                    } while (escolha != 3);
                    break;

                case 2: // Dica --> só uma utilização por game, zerar ao encerrar o jogo
                    dica(tabuleiro, &qtdDica);
                    break;

                case 3: // Encerrar o jogo, salvar nome e pontuação no ranking
                    salvarOrganizar(nome, tamanhoNome, pontuacao);
                    system("clear");
                    break;

                default:
                    system("clear");
                    printf("Opção Invalida, tente novamente!\n");
                    printf("Pressione enter para continuar . . .");
                    scanf("%c", &verif);
                    break;
                }

            } while (switchInternoJogar != 3);
            break;

        case 3: // Lista de Jogadores
            system("clear");
            listar();
            printf("Pressione enter para continuar . . .");
            scanf("%c", &verif);
            system("clear");
            break;

        case 4: // Sair
            system("clear");
            printf("Obrigado por Jogar!\n");
            return 0;

        default: // Entrada Invalida
            system("clear");
            printf("\nOpção Invalida, tente novamente!\n");
            break;
        }
    }
}
