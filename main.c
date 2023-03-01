#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Defines
#define TAMANHOPOPULACAO 10
#define TAMANHOTABULEIRO 8
#define QUANTIDADETORNEIOS 4
#define QUANTIDADEINDIVIDUOSPORTORNEIO 4
#define QUANTIDADEPAIS 2

// Variáveis globais
int **populacaoAtual;   
int **proximaPopulacao;
int *fitnessDaPopulacao;
int **individuosTorneio;
int **pais;
int *fitnessTorneio;

// Funções
void alocaMemoria();
void fitness();
void inicializaPopulacao();
void ordenaPopulacao();
void selecaoAleatoria();
void selecaoTorneio();

int main(){

    alocaMemoria();
    srand(time(NULL));
    inicializaPopulacao();
    fitness();
    ordenaPopulacao();
    //selecaoAleatoria();
    selecaoTorneio();

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

/*
    ------------------
    selecaoAleatoria()
    ------------------

    Seleciona de forma aleatória os dois individuos pais.
    O processo ocorre até que ambos os pais sejam diferentes entre si.
*/
void selecaoAleatoria(){
    int i, j;
    int individuo;
    int cont = 0;

    do{
        for (i=0; i<QUANTIDADEPAIS; i++){
            individuo = (rand()%TAMANHOPOPULACAO);
            printf ("Individuo escolhido para pai %d: individuo %d\n",i+1, individuo+1);
            for (j=0; j<TAMANHOTABULEIRO; j++){
                pais[i][j] = populacaoAtual[individuo][j];
                printf("%d ", pais[i][j]);
            }
            printf("\n");
        }
        printf("\n");

        if (i>1){
            for (i=0; i<TAMANHOTABULEIRO; i++){
                if (pais[0][i] == pais[1][i])
                    cont++;
            }
        }

        if (cont == TAMANHOTABULEIRO)
            printf ("PAIS IGUAIS, SELECIONE PAIS DIFERENTES!\n\n");
    } while (cont == TAMANHOTABULEIRO);
}

/*
    ----------------
    selecaoTorneio()
    ----------------

    São selecionados N individuos distintos entre si para participar do torneio.
    O individuo com maior aptidão entre os selecionados é escolhido para ser um pai.
    O precesso se repete até que exista dois pais diferentes entre si.
*/
void selecaoTorneio(){
    int i, j, k;
    int individuo[QUANTIDADEINDIVIDUOSPORTORNEIO];
    int cont1;

    do{
        cont1 = 0;
        for (k=0; k<QUANTIDADEPAIS; k++){
            // Seleciona individuos para o torneio
            for (i=0; i < QUANTIDADEINDIVIDUOSPORTORNEIO; i++){
                individuo[i] = (rand()%TAMANHOPOPULACAO);

                // Verifica se o individuo já foi selecionado (ta errado)
                for (j=0; j<=i; j++){
                    if (individuo[i] == individuo[j]){
                        individuo[i] = (rand()%TAMANHOPOPULACAO);
                        j = 0;
                    }
                }

                // opcao 2 da errado pq nao verifica se o novo numero sorteado é igual aos anteriores
                //for (j=0; j/,i; j++){
                    //while (individuo[i] == individuo[j])
                        //individuo[i] = (rand()%TAMANHOPOPULACAO);
                //} 

                fitnessTorneio[i] = fitnessDaPopulacao[individuo[i]];
                printf("%d: individuo %d selecionado: ", i+1, individuo[i]+1);
                for (j=0; j<TAMANHOTABULEIRO; j++){
                    individuosTorneio[i][j] = populacaoAtual[individuo[i]][j];
                    printf ("%d ", individuosTorneio[i][j]);
                }
                printf("\n");
                printf("Fitnes do individuo: %d\n\n", fitnessTorneio[i]);
            }
            printf("\n");

            // Ordenar os individuos do torneio
            ordenaTorneio();

            // Seleciona o individuo com maior fitness para ser o pai
            printf("Pai %d selecionado: ", k+1);
            for (i=0; i<TAMANHOTABULEIRO; i++){
                pais[k][i] = individuosTorneio[QUANTIDADEINDIVIDUOSPORTORNEIO-1][i];
                printf("%d ", pais[k][i]);
            }
            printf("\n\n");
        }

        // Verifica se os pais sao iguais
        if (k>1){
            for (i=0; i<TAMANHOTABULEIRO; i++){
                if (pais[0][i] == pais[1][i])
                    cont1++;
            }
        }

        if (cont1 == TAMANHOTABULEIRO)
            printf ("PAIS IGUAIS, SELECIONE PAIS DIFERENTES!\n\n");
    } while (cont1 == TAMANHOTABULEIRO);
}

