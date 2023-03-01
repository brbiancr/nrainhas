#include <stdio.h>

// Defines
#define TAMANHOPOPULACAO 10
#define TAMANHOTABULEIRO 8

// Variáveis globais
int **populacaoAtual;   
int **proximaPopulacao;
int *fitnessDaPopulacao;

// Funções
void alocaMemoria();
void fitness();
void inicializaPopulacao();
void ordenaPopulacao();

int main(){

    alocaMemoria();
    srand(time(NULL));
    inicializaPopulacao();
    fitness();
    ordenaPopulacao();

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
    ---------
    fitness()
    ---------

    Avalia a aptidão de cada individuo da população.
*/
void fitness(){
    int i, j, k, l, m;
    int linhaDaRainha, fitnessRainha, colisao;
    int auxiliar1, auxiliar2;
    int tabuleiro[TAMANHOTABULEIRO][TAMANHOTABULEIRO];

    for (i=0; i<TAMANHOPOPULACAO; i++){
        fitnessDaPopulacao[i]= 0 ;
        printf ("--Individuo %d\n\n", i+1);
        // Posiciona e mostra na tela as rainhas no tabuleiro
        for (j=0; j<TAMANHOTABULEIRO; j++){
            for (k=0; k<TAMANHOTABULEIRO; k++){
                linhaDaRainha = populacaoAtual[i][k];
                if (j == (linhaDaRainha-1)){
                    tabuleiro[j][k] = 1;
                }else{
                    tabuleiro[j][k] = 0;
                }
                //printf ("%d ", tabuleiro[j][k]);
            }
            //printf ("\n");
        } 

        for (j=0; j<TAMANHOTABULEIRO; j++){
            for (k=0; k<TAMANHOTABULEIRO; k++){
                colisao = 0;
                if (tabuleiro[j][k]==1){
                    for (l=0; l<TAMANHOTABULEIRO; l++){
                        for (m=0; m<TAMANHOTABULEIRO; m++){
                            // Verifica se há colisao na linha
                            if (((j==l) && (m!=k)) && (tabuleiro[l][m]==1))   
                                colisao++;

                            // Verifica se a rainha esta na diagonal principal, caso esteja, verifica se há colisão na diagonal principal
                            if (((j==k) && (l==m)) && (k!=m) && (tabuleiro[l][m]==1))
                                colisao++;                            
                            
                            // Verifica se a rainha esta na diagonal secundária, caso esteja, verifica se há colisão na diagonal secundária
                            if (((j+k)==(TAMANHOTABULEIRO-1)) && ((l+m)==(TAMANHOTABULEIRO-1)) && (j!=l) && (tabuleiro[l][m]==1))
                                colisao++;

                            // Verifica colisão na paralela a diagonal secundária
                            if (((l+m)==(j+k)) && ((j!=l) && (k!=m)) && (j!=k) && ((l+m)!=(TAMANHOTABULEIRO-1)) && (tabuleiro[l][m]==1))
                                colisao++;
                        }
                    }

                    // Verifica colisao na paralela a diagonal principal
                    if (j!=k || (j+k != TAMANHOTABULEIRO-1)){
                    // Verificando abaixo da rainha
                        auxiliar1 = j+1;
                        auxiliar2 = k+1;
                        while (auxiliar1 < TAMANHOTABULEIRO && auxiliar2 < TAMANHOTABULEIRO){
                            if (tabuleiro[auxiliar1][auxiliar2] == 1)
                                colisao++;
                            auxiliar1++;
                            auxiliar2++;
                            
                        }
                        // Verificando acima da rainha
                        auxiliar1 = j-1;
                        auxiliar2 = k-1;
                        while (auxiliar1 >= 0 && auxiliar2 >= 0){
                            if (tabuleiro[auxiliar1][auxiliar2] == 1)
                                colisao++;
                            auxiliar1--;
                            auxiliar2--;
                        }
                    }

                    (colisao > 0) ? (fitnessRainha = 0) : (fitnessRainha = 1);
                
                    //printf ("Fitness da rainha: %d\n", fitnessRainha);

                    fitnessDaPopulacao[i] += fitnessRainha;
                }
            }

        }  
        printf("Fitness: %d\n\n", fitnessDaPopulacao[i]);
    }
    printf("\n");
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

/*
    -----------------
    ordenaPopulacao()
    -----------------

    Ordena os individuos da população de acordo com seu fitness.
    A ordenação é feita em ordem crescente.
*/
void ordenaPopulacao(){
    int i, j, k;
    int auxiliar;
    int auxiliarA[TAMANHOTABULEIRO];  

    for (i=0; i<TAMANHOPOPULACAO; i++){
        auxiliar = fitnessDaPopulacao[i];
        for (k=0; k<TAMANHOTABULEIRO; k++)
            auxiliarA[k] = populacaoAtual[i][k];
        
        for (j=i; (j>0) && auxiliar<fitnessDaPopulacao[j-1]; j--){
            fitnessDaPopulacao[j] = fitnessDaPopulacao[j-1];
            for (k=0; k<TAMANHOTABULEIRO; k++)
                populacaoAtual[j][k] = populacaoAtual[j-1][k];
        }
        fitnessDaPopulacao[j] = auxiliar;
        for (k=0; k<TAMANHOTABULEIRO; k++)
            populacaoAtual[j][k] = auxiliarA[k];
    }

    printf ("***POPULACAO ORDENADA PELO FITNESS***: \n");
    for (i=0; i<TAMANHOPOPULACAO; i++){
        for (j=0; j<TAMANHOTABULEIRO; j++)
            printf ("%d ", populacaoAtual[i][j]);
        printf ("Fitness: %d\n", fitnessDaPopulacao[i]);
    }
    printf("\n");
}

