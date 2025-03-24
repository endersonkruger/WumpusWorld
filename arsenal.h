#ifndef _ARSENAL_H
#define _ARSENAL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define TAM 8
typedef struct _Caverna
{
    bool abismo, ouro, flecha, agente, wumpus, brisa, fedor, brilho;
    char conteudo;
} caverna;

typedef struct _DadosJogador
{
    char nome[128];
    int pontuacao;
} dadosJogador;

void listar();
void salvarOrganizar(char nome[], int tamanhoNome, int pontuacao);
void iniciaPartida(caverna tabuleiro[TAM][TAM], int *qtdFlecha, int *qtdOuro, int *qtdDica);
void reiniciaPartida(caverna tabuleiro[TAM][TAM], caverna resetTabuleiro[TAM][TAM], int *qtdFlecha, int *qtdOuro, int *qtdDica);
void salvaInfos(caverna tabuleiro[TAM][TAM], caverna resetTabuleiro[TAM][TAM]);
void jogarImprimeCaverna(caverna tabuleiro[TAM][TAM], int *qtdFlecha, int *qtdOuro);
void debugImprimeCaverna(caverna tabuleiro[TAM][TAM], int *qtdFlecha, int *qtdOuro);
int disparaFlecha(caverna tabuleiro[TAM][TAM], int *qtdFLecha);
int moveWumpus(caverna tabuleiro[TAM][TAM]);
int moveAgente(caverna tabuleiro[TAM][TAM], char movimento, int *qtdFlecha, int *qtdOuro);
int dica(caverna tabuleiro[TAM][TAM], int *qtdDica);

#endif
