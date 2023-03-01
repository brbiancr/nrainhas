#include <stdio.h>

// Defines
#define TAMANHOPOPULACAO 10
#define TAMANHOTABULEIRO 8

// Variáveis globais
int **populacaoAtual;   
int **proximaPopulacao;

// Funções
void alocaMemoria();
void inicializaPopulacao();

int main(){

    alocaMemoria();
    srand(time(NULL));
    inicializaPopulacao();
    
    return 0;
}

/*
    ---------------
    alocaMemoria()
    ---------------

    Aloca memória de forma dinamica para:
    - populacaoAtual
    - proximaPopulacao
    - fitnessDaPopulacao
    - individuosTorneio
    - pais
    - fitnessTorneio
*/
void alocaMemoria(){
    int i;

    populacaoAtual = (int**) malloc(sizeof(int*) * TAMANHOPOPULACAO);
    proximaPopulacao = (int**) malloc(sizeof(int*) * TAMANHOPOPULACAO);

    for (i=0; i<TAMANHOPOPULACAO; i++){
        populacaoAtual[i] = (int*) malloc(sizeof(int) * TAMANHOTABULEIRO);
        proximaPopulacao[i] = (int*) malloc(sizeof(int) * TAMANHOTABULEIRO);
    }
}

/*
    ---------------------
    inicializaPopulacao()
    ---------------------

    Inicializa de forma aleatória a população inicial.
*/
void inicializaPopulacao(){
    int i, j;

    printf ("***POPULACAO***\n");
    for (i=0; i<TAMANHOPOPULACAO; i++){
        for (j=0; j<TAMANHOTABULEIRO; j++){
            populacaoAtual[i][j] = (rand()%TAMANHOTABULEIRO) + 1;
            printf("%d ", populacaoAtual[i][j]);     // Mostra a populacao na tela 
        }
        printf("\n");
    }
    printf("\n");
}