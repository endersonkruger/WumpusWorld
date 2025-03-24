#include "arsenal.h"


void listar()
{
    FILE *ranking;
    int i, linhas = 0, nRegistros;
    char c, primeiraLinha[16];

    ranking = fopen("ranking.txt", "r");
    while (fscanf(ranking, "%c", &c) != EOF)
    {
        if (c == '\n')
        {
            linhas++;
        }
    }
    fclose(ranking);

    nRegistros = linhas - 1;
    dadosJogador player[nRegistros];

    ranking = fopen("ranking.txt", "r");
    fgets(primeiraLinha, sizeof(primeiraLinha), ranking);

    for (i = 0; i < nRegistros; i++)
    {
        fscanf(ranking, "%d", &player[i].pontuacao);
        fgetc(ranking);
        fgets(player[i].nome, sizeof(player[i].nome), ranking);
    }
    fclose(ranking);

    for (i = 0; i < nRegistros; i++)
    {
        printf("%d %s", player[i].pontuacao, player[i].nome);
    }
}

void salvarOrganizar(char nome[], int tamanhoNome, int pontuacao)
{
    FILE *ranking;
    int i, j, linhas = 0, nRegistros, auxPontos;
    char c, primeiraLinha[16], auxNome[128];

    for (i = 0; i < tamanhoNome; i++) // Garante que o nome n tenha \n.
    {
        if (nome[i] == '\n')
        {
            nome[i] = '\0';
        }
    }

    ranking = fopen("ranking.txt", "a");
    fprintf(ranking, "%d %s\n", pontuacao, nome);
    fclose(ranking);

    ranking = fopen("ranking.txt", "r");
    while (fscanf(ranking, "%c", &c) != EOF)
    {
        if (c == '\n')
        {
            linhas++;
        }
    }
    fclose(ranking);

    nRegistros = linhas - 1; // -1 pois a primeira nunca será um player.
    dadosJogador player[nRegistros];

    ranking = fopen("ranking.txt", "r");                  // Ler conteudo dos arquivos.
    fgets(primeiraLinha, sizeof(primeiraLinha), ranking); // Primeira linha "ignorada".

    for (i = 0; i < nRegistros; i++)
    { // lê os dados
        fscanf(ranking, "%d", &player[i].pontuacao);
        fgetc(ranking);
        fgets(player[i].nome, sizeof(player[i].nome), ranking);
    }
    fclose(ranking);

    for (i = 0; i < nRegistros; i++) // Organiza a parafernalha.
    {
        for (j = i + 1; j < nRegistros; j++)
        {
            if (player[i].pontuacao > player[j].pontuacao)
            {
                auxPontos = player[i].pontuacao;
                player[i].pontuacao = player[j].pontuacao;
                player[j].pontuacao = auxPontos;
                strcpy(auxNome, player[i].nome);
                strcpy(player[i].nome, player[j].nome);
                strcpy(player[j].nome, auxNome);
            }
        }
    }

    ranking = fopen("ranking.txt", "w");
    fprintf(ranking, "%s", primeiraLinha);
    for (i = 0; i < nRegistros; i++)
    {
        fprintf(ranking, "%d %s", player[i].pontuacao, player[i].nome);
    }
    fclose(ranking);
}

void iniciaPartida(caverna tabuleiro[TAM][TAM], int *qtdFlecha, int *qtdOuro, int *qtdDica)
{
    srand(time(NULL));
    int x, y, qtdAbismos = 0;
    for (x = 0; x < TAM; x++) // Seta todas as posções inicias do tabuleiro.
    {
        for (y = 0; y < TAM; y++)
        {
            tabuleiro[x][y].abismo = false;
            tabuleiro[x][y].ouro = false;
            tabuleiro[x][y].flecha = false;
            tabuleiro[x][y].agente = false;
            tabuleiro[x][y].wumpus = false;
            tabuleiro[x][y].brilho = false;
            tabuleiro[x][y].fedor = false;
            tabuleiro[x][y].brisa = false;
            tabuleiro[x][y].conteudo = '?';
        }
    }
    tabuleiro[TAM - 1][0].agente = true; // Seta a posição inicial do Agente.
    *qtdFlecha = 1;
    *qtdOuro = 0;
    *qtdDica = 1;
    while (qtdAbismos != 5) // Posiciona os abismos na Caverna
    {
        do
        {
            x = rand() % TAM;
            y = rand() % TAM;
        } while (tabuleiro[x][y].agente == true || tabuleiro[x][y].abismo == true); // O abismo não pode estar junto ao agente.
        tabuleiro[x][y].abismo = true;                                              // Define 5 posições com o abismo.

        if (x > 0)
        {
            tabuleiro[x - 1][y].brisa = true; // Coloca a brisa da posição acima.
        }
        if (x < TAM - 1)
        {
            tabuleiro[x + 1][y].brisa = true; // Coloca a brisa da posição abaixo.
        }
        if (y > 0)
        {
            tabuleiro[x][y - 1].brisa = true; // Coloca a brisa da posição à esquerda.
        }
        if (y < TAM - 1)
        {
            tabuleiro[x][y + 1].brisa = true; // Coloca a brisa da posição à direita.
        }
        qtdAbismos++;
    }

    do
    {
        x = rand() % TAM;
        y = rand() % TAM;
    } while (tabuleiro[x][y].abismo == true || tabuleiro[x][y].agente == true); // O wumpus não pode estar junto ao agente ou ao abismo.
    tabuleiro[x][y].wumpus = true;                                              // Define uma posição com o wumpus.
    if (x > 0)
    {
        tabuleiro[x - 1][y].fedor = true; // Coloca o fedor da posição abaixo.
    }
    if (x < TAM - 1) // Esses Ifs garantem que o fedor nao ira ser atribuido a uma posição inexistente do tabuleiro.
    {
        tabuleiro[x + 1][y].fedor = true; // Coloca o fedor da posição acima.
    }
    if (y > 0)
    {
        tabuleiro[x][y - 1].fedor = true; // Coloca o fedor da à esquerda.
    }
    if (y < TAM - 1)
    {
        tabuleiro[x][y + 1].fedor = true; // Coloca o fedor da posição à direita.
    }

    do
    {
        x = rand() % TAM;
        y = rand() % TAM;
    } while (tabuleiro[x][y].abismo == true || tabuleiro[x][y].agente == true); // A flecha não pode estar junto ao abismo ou ao agente
    tabuleiro[x][y].flecha = true;                                              // Define uma posição com a flecha.

    do
    {
        x = rand() % TAM;
        y = rand() % TAM;
    } while (tabuleiro[x][y].abismo == true || tabuleiro[x][y].agente == true); // O ouro não pode estar junto ao abismo ou ao agente
    tabuleiro[x][y].ouro = true;                                                // Define uma posição com o ouro.
    tabuleiro[x][y].brilho = true;
}

void debugImprimeCaverna(caverna tabuleiro[TAM][TAM], int *qtdFlecha, int *qtdOuro)
{
    int x, y, agenteX, agenteY;
    printf("\n");
    for (x = 0; x < TAM; x++)
    {
        printf("|");
        for (y = 0; y < TAM; y++)
        {
            if (tabuleiro[x][y].abismo == true)
            {
                tabuleiro[x][y].conteudo = 'P';
            }

            if (tabuleiro[x][y].ouro == true)
            {
                tabuleiro[x][y].conteudo = 'O';
            }

            if (tabuleiro[x][y].flecha == true)
            {
                tabuleiro[x][y].conteudo = 'F';
            }

            if (tabuleiro[x][y].ouro == true && tabuleiro[x][y].flecha == true)
            {
                tabuleiro[x][y].conteudo = 'B';
            }

            if (tabuleiro[x][y].agente == true)
            {
                tabuleiro[x][y].conteudo = 'A';
                agenteX = x;
                agenteY = y;
            }

            if (tabuleiro[x][y].wumpus == true)
            {
                tabuleiro[x][y].conteudo = 'W';
            }

            if (tabuleiro[x][y].agente == false && tabuleiro[x][y].abismo == false && tabuleiro[x][y].wumpus == false && tabuleiro[x][y].ouro == false && tabuleiro[x][y].flecha == false)
            {
                tabuleiro[x][y].conteudo = ' ';
            }

            printf(" %c |", tabuleiro[x][y].conteudo);
        }
        printf("\n");
    }
    printf("\n\nLegenda:\nA\t--> Agente\nP\t--> Poço/Abismo\nW\t--> Wumpus\nO\t--> Ouro\nF\t--> Flecha\nB\t--> Bônus (ouro + flecha)\n");

    printf("\n\tPercepções do agente:\n");
    if (tabuleiro[agenteX][agenteY].brisa == true)
    {
        printf("BRISA  [*]\n");
    }
    else
    {
        printf("BRISA  [ ]\n");
    }
    if (tabuleiro[agenteX][agenteY].brilho == true)
    {
        printf("BRILHO [*]\n");
    }
    else
    {
        printf("BRILHO [ ]\n");
    }
    if (tabuleiro[agenteX][agenteY].fedor == true)
    {
        printf("FEDOR  [*]\n");
    }
    else
    {
        printf("FEDOR  [ ]\n");
    }
    if (*qtdFlecha != 0)
    {
        printf("\nMUNIÇÃO [%d] FLECHAS\n", *qtdFlecha);
    }
    else
    {
        printf("\nSEM FLECHAS\n");
    }
    if (*qtdOuro != 0)
    {
        printf("\nOURO NA MOCHILA!\n");
    }
}

void jogarImprimeCaverna(caverna tabuleiro[TAM][TAM], int *qtdFlecha, int *qtdOuro)
{
    int x, y, agenteX, agenteY;
    printf("\n");
    for (x = 0; x < 8; x++)
    {
        printf("|");
        for (y = 0; y < 8; y++)
        {
            if (tabuleiro[x][y].agente == true)
            {
                tabuleiro[x][y].conteudo = 'A';
                agenteX = x;
                agenteY = y;
            }
            else
            {
                tabuleiro[x][y].conteudo = '?';
            }
            printf(" %c |", tabuleiro[x][y].conteudo);
        }
        printf("\n");
    }

    printf("\n\tPercepções do agente:\n");
    if (tabuleiro[agenteX][agenteY].brisa == true)
    {
        printf("BRISA  [*]\n");
    }
    else
    {
        printf("BRISA  [ ]\n");
    }
    if (tabuleiro[agenteX][agenteY].brilho == true)
    {
        printf("BRILHO [*]\n");
    }
    else
    {
        printf("BRILHO [ ]\n");
    }
    if (tabuleiro[agenteX][agenteY].fedor == true)
    {
        printf("FEDOR  [*]\n");
    }
    else
    {
        printf("FEDOR  [ ]\n");
    }
    if (*qtdFlecha != 0)
    {
        printf("\nMUNIÇÃO [%d] FLECHAS\n", *qtdFlecha);
    }
    else
    {
        printf("\nSEM FLECHAS\n");
    }
    if (*qtdOuro != 0)
    {
        printf("\nOURO NA MOCHILA!\n");
    }
}

int moveWumpus(caverna tabuleiro[TAM][TAM])
{
    srand(time(NULL));
    bool wumpusVivo = false;
    int x, y, wumpusX, wumpusY, nWumpusX, nWumpusY, sorteio;
    char verif;
    for (x = 0; x < TAM; x++) // Percorre o tabuleiro procurando o Wumpus. Quando acha, sai do loop.
    {
        for (y = 0; y < TAM; y++)
        {
            if (tabuleiro[x][y].wumpus == true)
            {
                wumpusX = x;
                wumpusY = y;
                wumpusVivo = true;
                break;
            }
        }
    }
    if (wumpusVivo)
    {
        tabuleiro[wumpusX][wumpusY].wumpus = false; // Remove o Wumpus e seu fedor da posição atual, como preparação para sua nova posição.
        if (wumpusX > 0)
        {
            tabuleiro[wumpusX - 1][wumpusY].fedor = false; // Remove o fedor da posição acima.
        }
        if (wumpusX < TAM - 1)
        {
            tabuleiro[wumpusX + 1][wumpusY].fedor = false; // Remove o fedor da posição abaixo.
        }
        if (wumpusY > 0)
        {
            tabuleiro[wumpusX][wumpusY - 1].fedor = false; // Remove o fedor da posição à esquerda.
        }
        if (wumpusY < TAM - 1)
        {
            tabuleiro[wumpusX][wumpusY + 1].fedor = false; // Remove o fedor da posição à direita.
        }
        do // Acha novas coordenadas validas para o Wumpus.
        {
            sorteio = rand() % 2;
            if (sorteio == 0) // Sorteia se vai mudar a coordenada X ou Y.
            {
                do // Define um novo X e mantem o Y.
                {
                    nWumpusX = wumpusX + (rand() % 3 - 1);
                } while (nWumpusX < 0 || nWumpusX > 7);
                nWumpusY = wumpusY;
            }
            else
            {
                do // Define um novo Y e mantem o X.
                {
                    nWumpusY = wumpusY + (rand() % 3 - 1);
                } while (nWumpusY < 0 || nWumpusY > 7);
                nWumpusX = wumpusX;
            }
        } while (tabuleiro[nWumpusX][nWumpusY].abismo); // Garante que não vai andar para um abismo.
        tabuleiro[nWumpusX][nWumpusY].wumpus = true;    // Coloca o Wumpus em uma nova posição valida.

        if (nWumpusX > 0)
        {
            tabuleiro[nWumpusX - 1][nWumpusY].fedor = true; // Coloca o fedor da posição acima.
        }
        if (nWumpusX < TAM - 1)
        {
            tabuleiro[nWumpusX + 1][nWumpusY].fedor = true; // Coloca o fedor da posição abaixo.
        }
        if (nWumpusY > 0)
        {
            tabuleiro[nWumpusX][nWumpusY - 1].fedor = true; // Coloca o fedor da posição à esquerda.
        }
        if (nWumpusY < TAM - 1)
        {
            tabuleiro[nWumpusX][nWumpusY + 1].fedor = true; // Coloca o fedor da posição à direita.
        }
        if (tabuleiro[nWumpusX][nWumpusY].agente) // Se o Wumpus se move para a casa do Agente, devora o agente.
        {
            printf("O monstro te devorou!");
            printf("Pressione enter para continuar . . .");
            scanf("%c", &verif);
            tabuleiro[nWumpusX][nWumpusY].agente = false;
            return 3;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

int moveAgente(caverna tabuleiro[TAM][TAM], char movimento, int *qtdFlecha, int *qtdOuro)
{
    int x, y, agenteX, agenteY, nAgenteX, nAgenteY;
    char verif;
    for (x = 0; x < TAM; x++) // Percorre o tabuleiro procurando o Agente. Quando acha, sai do loop.
    {
        for (y = 0; y < TAM; y++)
        {
            if (tabuleiro[x][y].agente == true)
            {
                agenteX = x;
                agenteY = y;
                break;
            }
        }
    }
    if (((movimento == 'W' || movimento == 'w') && agenteX == 0) || ((movimento == 'A' || movimento == 'a') && agenteY == 0) || ((movimento == 'S' || movimento == 's') && agenteX == 7) || ((movimento == 'D' || movimento == 'd') && agenteY == 7)) // Verifica se o agente não deu de cara na parede.
    {
        printf("\nVocê deu de cara na parede!\nPressione enter para retornar à tentativa . . .\n");
        scanf("%c", &verif);
        return 0; // segue o baile
    }
    else
    {
        if (movimento == 'W' || movimento == 'w')
        {
            nAgenteX = agenteX - 1;
            nAgenteY = agenteY;
        }
        else if (movimento == 'A' || movimento == 'a')
        {

            nAgenteX = agenteX;
            nAgenteY = agenteY - 1;
        }
        else if (movimento == 'S' || movimento == 's')
        {

            nAgenteX = agenteX + 1;
            nAgenteY = agenteY;
        }
        else if (movimento == 'D' || movimento == 'd')
        {
            nAgenteX = agenteX;
            nAgenteY = agenteY + 1;
        }
        tabuleiro[agenteX][agenteY].agente = false;
        tabuleiro[nAgenteX][nAgenteY].agente = true; // Atualiza a situação do agente pós movimento.
        if (tabuleiro[nAgenteX][nAgenteY].abismo == true)
        {
            printf("\nVocê caiu no buraco!\nTente novamente.\n");
            printf("Pressione enter para continuar . . .");
            scanf("%c", &verif);
            return 4; // 4 --> cair no buraco
        }
        else if (tabuleiro[nAgenteX][nAgenteY].wumpus == true)
        {
            printf("\nQue pena, o Wumpus devorou você!\nTente novamente.\n");
            printf("Pressione enter para continuar . . .");
            scanf("%c", &verif);
            return 3; // 3 --> devorado pelo wumpus
        }
        else if (tabuleiro[nAgenteX][nAgenteY].ouro == true && tabuleiro[nAgenteX][nAgenteY].flecha == true)
        {
            printf("\nQue coincidência! Você encontrou o ouro e a flecha. Retorne à posição inicial com a sua fortuna ou arrisque-se numa luta contra o poderoso Wumpus!\nTome cuidado em seu caminho.\n");
            tabuleiro[nAgenteX][nAgenteY].ouro = false;
            tabuleiro[nAgenteX][nAgenteY].brilho = false;
            tabuleiro[nAgenteX][nAgenteY].flecha = false;
            (*qtdFlecha)++;
            (*qtdOuro)++;
            printf("Pressione enter para continuar . . .");
            scanf("%c", &verif);
            return 5; // 5 --> ouro e flecha
        }
        else if (tabuleiro[nAgenteX][nAgenteY].ouro == true)
        {
            printf("\nVocê encontrou o ouro! Agora, retorne à posição inicial com a sua fortuna. Mas tome cuidado com o poderoso Wumpus, ele pode chegar até você antes que chegue ao destino. Boa sorte!\n");
            tabuleiro[nAgenteX][nAgenteY].ouro = false;
            tabuleiro[nAgenteX][nAgenteY].brilho = false;
            (*qtdOuro)++;
            printf("Pressione enter para continuar . . .");
            scanf("%c", &verif);
            return 2; // 2 --> ouro
        }
        else if (tabuleiro[nAgenteX][nAgenteY].flecha == true)
        {
            printf("\nVocê encontrou mais uma flecha! Use com cuidado.\n");
            tabuleiro[nAgenteX][nAgenteY].flecha = false;
            (*qtdFlecha)++;
            printf("Pressione enter para continuar . . .");
            scanf("%c", &verif);
            return 1; // 1 --> flecha
        }
    }
    return 0;
}

int disparaFlecha(caverna tabuleiro[TAM][TAM], int *qtdFlecha)
{
    char direcao, verif;
    int x, y, agenteX, agenteY;
    if (*qtdFlecha == 0)
    {
        printf("Você esta sem flechas\n");
        return 0;
    }
    else
    {
        for (x = 0; x < TAM; x++) // Percorre o tabuleiro procurando o Agente. Quando acha, sai do loop.
        {
            for (y = 0; y < TAM; y++)
            {
                if (tabuleiro[x][y].agente == true)
                {
                    agenteX = x;
                    agenteY = y;
                    break;
                }
            }
        }
        do
        {

            printf("Para onde deseja atirar?\nW - Cima | A - Esquerda | S - Baixo | D - Direita\n");
            scanf("%c", &direcao);
            getchar();
            switch (direcao)
            {
            case 'w':
                if (agenteX > 0)
                {
                    (*qtdFlecha)--;
                    if (tabuleiro[agenteX - 1][agenteY].wumpus)
                    {
                        tabuleiro[agenteX - 1][agenteY].wumpus = false;
                        printf("WHAAAJAAAAAAAAAAAAAAAAAAAAAA\n -Você matou o Wumpus-\nPressione enter para continuar . . .\n");
                        scanf("%c", &verif);
                        return 0;
                    }
                    else
                    {
                        printf("Você errou!\nPressione enter para continuar . . .\n");
                        scanf("%c", &verif);
                        return 0;
                    }
                }
                else
                {
                    printf("Direção Invalida\nPressione enter para continuar . . .\n");
                    scanf("%c", &verif);
                }
                break;
            case 'a':
                if (agenteY > 0)
                {
                    (*qtdFlecha)--;
                    if (tabuleiro[agenteX][agenteY - 1].wumpus)
                    {
                        tabuleiro[agenteX][agenteY - 1].wumpus = false;
                        printf("WHAAAJAAAAAAAAAAAAAAAAAAAAAA\n -Você matou o Wumpus-\nPressione enter para continuar . . .\n");
                        scanf("%c", &verif);
                        return 0;
                    }
                    else
                    {
                        printf("Você errou!\nPressione enter para continuar . . .\n");
                        scanf("%c", &verif);
                        return 0;
                    }
                }
                else
                {
                    printf("Direção Invalida\nPressione enter para continuar . . .\n");
                    scanf("%c", &verif);
                }
                break;
            case 's':
                if (agenteX < TAM - 1)
                {
                    (*qtdFlecha)--;
                    if (tabuleiro[agenteX + 1][agenteY].wumpus)
                    {
                        tabuleiro[agenteX + 1][agenteY].wumpus = false;
                        printf("WHAAAJAAAAAAAAAAAAAAAAAAAAAA\n -Você matou o Wumpus-\nPressione enter para continuar . . .\n");
                        scanf("%c", &verif);
                        return 0;
                    }
                    else
                    {
                        printf("Você errou!\nPressione enter para continuar . . .\n");
                        scanf("%c", &verif);
                        return 0;
                    }
                }
                else
                {
                    printf("Direção Invalida\nPressione enter para continuar . . .\n");
                    scanf("%c", &verif);
                }
                break;
            case 'd':
                if (agenteY < TAM - 1)
                {
                    (*qtdFlecha)--;
                    if (tabuleiro[agenteX][agenteY + 1].wumpus)
                    {
                        tabuleiro[agenteX][agenteY + 1].wumpus = false;
                        printf("WHAAAJAAAAAAAAAAAAAAAAAAAAAA\n -Você matou o Wumpus-\nPressione enter para continuar . . .\n");
                        scanf("%c", &verif);
                        return 0;
                    }
                    else
                    {
                        printf("Você errou!\nPressione enter para continuar . . .\n");
                        scanf("%c", &verif);
                        return 0;
                    }
                }
                else
                {
                    printf("Direção Invalida\nPressione enter para continuar . . .\n");
                    scanf("%c", &verif);
                }
                break;
            case 'r':
                break;
            default:
                system("clear");
                printf("Opção Invalida, tente novamente!\n");
                printf("Pressione enter para continuar . . .");
                scanf("%c", &verif);
                break;
            }

        } while (direcao != 'r');
        return 0;
    }
}

int dica(caverna tabuleiro[TAM][TAM], int *qtdDica)
{
    int x, y, agenteX, agenteY, wumpusX, wumpusY;
    char verif;
    bool wumpusVivo = false;
    if (*qtdDica == 0)
    {
        printf("Você não tem mais dicas!\nPressione enter para continuar . . .\n");
        scanf("%c", &verif);
        return 0;
    }
    else
    {
        for (x = 0; x < TAM; x++) // Percorre o tabuleiro procurando o Wumpus. Quando acha, sai do loop.
        {
            for (y = 0; y < TAM; y++)
            {
                if (tabuleiro[x][y].wumpus == true)
                {
                    wumpusX = x;
                    wumpusY = y;
                    wumpusVivo = true;
                    break;
                }
            }
        }
        if (wumpusVivo)
        {
            for (x = 0; x < TAM; x++) // Percorre o tabuleiro procurando o Agente. Quando acha, sai do loop.
            {
                for (y = 0; y < TAM; y++)
                {
                    if (tabuleiro[x][y].agente == true)
                    {
                        agenteX = x;
                        agenteY = y;
                        break;
                    }
                }
            }
            if (agenteX > wumpusX && agenteY == wumpusY)
            {
                (*qtdDica)--;
                printf("O monstro esta acima de você\nPressione enter para continuar . . .\n");
                scanf("%c", &verif);
                return 0;
            }
            else if (agenteX < wumpusX && agenteY == wumpusY)
            {
                (*qtdDica)--;
                printf("O monstro esta abaixo de você\nPressione enter para continuar . . .\n");
                scanf("%c", &verif);
                return 0;
            }
            else if (agenteX == wumpusX && agenteY > wumpusY)
            {
                (*qtdDica)--;
                printf("O monstro esta a sua esquerda\nPressione enter para continuar . . .\n");
                scanf("%c", &verif);
                return 0;
            }
            else if (agenteX == wumpusX && agenteY < wumpusY)
            {
                (*qtdDica)--;
                printf("O monstro esta a sua direita\nPressione enter para continuar . . .\n");
                scanf("%c", &verif);
                return 0;
            }
            else if (agenteX > wumpusX && agenteY > wumpusY)
            {
                (*qtdDica)--;
                printf("O monstro esta acima de você e a sua esquerda\nPressione enter para continuar . . .\n");
                scanf("%c", &verif);
                return 0;
            }
            else if (agenteX > wumpusX && agenteY < wumpusY)
            {
                (*qtdDica)--;
                printf("O monstro esta acima de você e a sua direita\nPressione enter para continuar . . .\n");
                scanf("%c", &verif);
                return 0;
            }
            else if (agenteX < wumpusX && agenteY > wumpusY)
            {
                (*qtdDica)--;
                printf("O monstro esta abaixo de você e a sua esquerda\nPressione enter para continuar . . .\n");
                scanf("%c", &verif);
                return 0;
            }
            else if (agenteX < wumpusX && agenteY < wumpusY)
            {
                (*qtdDica)--;
                printf("O monstro esta abaixo de você e a sua direita\nPressione enter para continuar . . .\n");
                scanf("%c", &verif);
                return 0;
            }
        }
        else
        {
            printf("O monstro esta morto, você não precisa de dica!\nPressione enter para continuar . . .\n");
            scanf("%c", &verif);
            return 0;
        }
    }
    return 0;
}

void reiniciaPartida(caverna tabuleiro[TAM][TAM], caverna resetTabuleiro[TAM][TAM], int *qtdFlecha, int *qtdOuro, int *qtdDica)
{
    int x, y;
    for (x = 0; x < TAM; x++)
    {
        for (y = 0; y < TAM; y++)
        {
            tabuleiro[x][y].agente = resetTabuleiro[x][y].agente;
            tabuleiro[x][y].abismo = resetTabuleiro[x][y].abismo;
            tabuleiro[x][y].wumpus = resetTabuleiro[x][y].wumpus;
            tabuleiro[x][y].fedor = resetTabuleiro[x][y].fedor;
            tabuleiro[x][y].brilho = resetTabuleiro[x][y].brilho;
            tabuleiro[x][y].ouro = resetTabuleiro[x][y].ouro;
            tabuleiro[x][y].brisa = resetTabuleiro[x][y].brisa;
            tabuleiro[x][y].flecha = resetTabuleiro[x][y].flecha;
            tabuleiro[x][y].conteudo = resetTabuleiro[x][y].conteudo;
        }
    }
    *qtdFlecha = 1;
    *qtdOuro = 0;
    *qtdDica = 1;
}

void salvaInfos(caverna tabuleiro[TAM][TAM], caverna resetTabuleiro[TAM][TAM])
{
    int x, y;
    for (x = 0; x < TAM; x++)
    {
        for (y = 0; y < TAM; y++)
        {
            resetTabuleiro[x][y].agente = tabuleiro[x][y].agente;
            resetTabuleiro[x][y].abismo = tabuleiro[x][y].abismo;
            resetTabuleiro[x][y].wumpus = tabuleiro[x][y].wumpus;
            resetTabuleiro[x][y].fedor = tabuleiro[x][y].fedor;
            resetTabuleiro[x][y].brilho = tabuleiro[x][y].brilho;
            resetTabuleiro[x][y].ouro = tabuleiro[x][y].ouro;
            resetTabuleiro[x][y].brisa = tabuleiro[x][y].brisa;
            resetTabuleiro[x][y].flecha = tabuleiro[x][y].flecha;
            resetTabuleiro[x][y].conteudo = tabuleiro[x][y].conteudo;
        }
    }
}