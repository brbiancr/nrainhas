#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Defines
#define TAMANHOPOPULACAO 5
#define TAMANHOTABULEIRO 8
#define QUANTIDADETORNEIOS 4
#define QUANTIDADEINDIVIDUOSPORTORNEIO 4
#define QUANTIDADEPAIS 2
#define TAXAELITISMO 2

// Variáveis globais
int **populacaoAtual;
int **proximaPopulacao;
int *fitnessDaPopulacao;
int **individuosTorneio;
int **pai;
int *fitnessTorneio;
int **tabuleiro;

// Funções
void alocaMemoria();
void cruzamentoDoisPontos();
void cruzamentoUmPonto();
void elitismo();
void fitness();
void inicializaPopulacao();
void mostraTabuleiro();
void ordenaPopulacao();
void ordenaTorneio();
void posicionaRainhas();
void selecaoAleatoria();
void selecaoRoleta();
void selecaoTorneio();

int main(){

    alocaMemoria();
    srand(time(NULL));
    inicializaPopulacao();
    fitness();
    ordenaPopulacao();
    //selecaoAleatoria();
    //selecaoTorneio();
    selecaoRoleta();
    elitismo();
    cruzamentoUmPonto();
    //cruzamentoDoisPontos();

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
    fitnessDaPopulacao = (int*) malloc(sizeof(int) * TAMANHOPOPULACAO);
    individuosTorneio = (int**) malloc(sizeof(int*) * QUANTIDADEINDIVIDUOSPORTORNEIO);
    pai = (int**) malloc(sizeof(int*) * QUANTIDADEPAIS);
    fitnessTorneio = (int*) malloc(sizeof(int) * QUANTIDADEINDIVIDUOSPORTORNEIO);
    tabuleiro = (int**) malloc(sizeof(int*) * TAMANHOTABULEIRO);

    for (i=0; i<TAMANHOPOPULACAO; i++){
        populacaoAtual[i] = (int*) malloc(sizeof(int) * TAMANHOTABULEIRO);
        proximaPopulacao[i] = (int*) malloc(sizeof(int) * TAMANHOTABULEIRO);
    }

    for (i=0; i<QUANTIDADEINDIVIDUOSPORTORNEIO; i++)
        individuosTorneio[i] = (int*) malloc(sizeof(int*) * TAMANHOTABULEIRO);

    for (i=0; i<QUANTIDADEPAIS; i++)
        pai[i] = (int*) malloc(sizeof(int) * TAMANHOTABULEIRO);

    for (i=0; i<TAMANHOTABULEIRO; i++)
        tabuleiro[i] = malloc(sizeof(int) * TAMANHOTABULEIRO);
}

/*
    ----------------------
    cruzamentoDoisPontos()
    ----------------------

    São selecionados dois pontos de corte aleatórios nos individuos pais.
    Recombina os dois pais gerando (um ou dois) novos filhos.
*/
void cruzamentoDoisPontos(){
    int i;
    int corte1, corte2;
    int filho[2][TAMANHOTABULEIRO];

    do{
        corte1 = rand() % TAMANHOTABULEIRO;
        corte2 = rand() % TAMANHOTABULEIRO;
    } while (corte1 == corte2 && (corte1 == TAMANHOTABULEIRO || corte2 == TAMANHOTABULEIRO) );
    
    printf ("Ponto de corte 1: %d\n", corte1);
    printf ("Ponto de corte 2: %d\n", corte2);
   
    if (corte1 > corte2){
        for (i=0; i<corte2; i++){
            filho[1][i] = pai[1][i];
            filho[2][i] = pai[2][i];
        }
        for (i=corte2; i<corte1; i++){
            filho[1][i] = pai[2][i];
            filho[2][i] = pai[1][i];
        }
        for (i=corte1; i<TAMANHOTABULEIRO; i++){
            filho[1][i] = pai[1][i];
            filho[2][i] = pai[2][i];
        }
    }
    else {
        for (i=0; i<corte1; i++){
            filho[1][i] = pai[1][i];
            filho[2][i] = pai[2][i];
        }
        for (i=corte1; i<corte2; i++){
            filho[1][i] = pai[2][i];
            filho[2][i] = pai[1][i];
        }
        for (i=corte2; i<TAMANHOTABULEIRO; i++){
            filho[1][i] = pai[1][i];
            filho[2][i] = pai[2][i];
        }
    }

    // Mostra filhos (Não ta mostrando isso)
    for (i=0; i<TAMANHOTABULEIRO; i++){
        printf ("%d ", filho[1][i]);
    }
    printf ("\n");

    for (i=0; i<TAMANHOTABULEIRO; i++){
        printf ("%d ", filho[2][i]);
    }
    printf ("\n");

}

/*
    -------------------
    cruzamentoUmPonto()
    -------------------

    Seleciona aleatóriamente um ponto de corte nos individuos pais.
    Recombina os dois pais gerando (um ou dois) novos filhos.
*/
void cruzamentoUmPonto(){
    int i;
    int corte;
    int filho[2][TAMANHOTABULEIRO];     
    
    do{
        corte = rand()% TAMANHOTABULEIRO;
    } while (corte == TAMANHOTABULEIRO);

    printf ("Ponto de corte: %d\n", corte);

    for (i=0; i<corte; i++){
        filho[1][i] = pai[1][i];
        filho[2][i] = pai[2][i];
    }

    for (i=corte; i<TAMANHOTABULEIRO; i++){
        filho[1][i] = pai[2][i];
        filho[2][i] = pai[1][i];
    }

    // Verificar se precisa da população intermediária
    for (i=0; i<TAMANHOTABULEIRO; i++){
        proximaPopulacao[TAXAELITISMO][i] = filho[1][i];    
        proximaPopulacao[TAXAELITISMO+1][i] = filho[2][i];
    }

    for (i=0; i<TAMANHOTABULEIRO; i++)
        printf ("%d ", filho[1][i]);
    printf ("\n");

    for (i=0; i<TAMANHOTABULEIRO; i++)
        printf ("%d ", filho[2][i]);
    printf ("\n");


}

/*
    ----------
    elitismo()
    ----------

    Copia para proximaPopulacao uma quantidade TAXAELITISMO dos individuos com maior fitness da populacaoAtual
*/
void elitismo(){
    int i;

    for (i=0; i<TAXAELITISMO; i++)
        proximaPopulacao[i] = populacaoAtual[TAMANHOPOPULACAO-i];
}

/*
    ---------
    fitness()
    ---------

    Avalia a aptidão de cada individuo da população.
*/
void fitness(){
    int i, j, k, l, m;
    int fitnessRainha, colisao;
    int auxiliar1, auxiliar2;

    for (i=0; i<TAMANHOPOPULACAO; i++){
        fitnessDaPopulacao[i]= 0 ;
        printf ("--Individuo %d\n\n", i+1);

        posicionaRainhas(i);

        mostraTabuleiro();

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
    A população inicial não deve ter individuos iguais.
*/
void inicializaPopulacao(){
    int i, j, k, l;
    int cont;

    printf ("***POPULACAO***\n");
    for (i=0; i<TAMANHOPOPULACAO; i++){
        for (j=0; j<TAMANHOTABULEIRO; j++){
            populacaoAtual[i][j] = rand()%TAMANHOTABULEIRO;

            do{
                cont = 0;
                if (i>0){
                    for (k=0; k<i; k++){
                        for (l=0; l<TAMANHOTABULEIRO; l++){
                            if (populacaoAtual[i][j] == populacaoAtual[k][l])
                                cont++;
                        }
                    }

                }
                for (k=0; k<TAMANHOTABULEIRO; k++)
                    populacaoAtual[i][k] = rand()%TAMANHOTABULEIRO;
            } while(cont == TAMANHOTABULEIRO);

            printf("%d ", populacaoAtual[i][j]);     // Mostra a populacao na tela
        }
        printf("\n");
    }
    printf("\n");
}

/*
    -----------------
    mostraTabuleiro()
    -----------------

    Mostra na tela o tabuleiro com as rainhas posicionadas
    0 indica posicao vazia
    1 indica rainha
*/
void mostraTabuleiro(){
    int i, j;

    for (i=0; i<TAMANHOTABULEIRO; i++){
        for (j=0; j<TAMANHOTABULEIRO; j++)
            printf("%d ", tabuleiro[i][j]);
        printf("\n");
    }
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
    ---------------
    ordenaTorneio()
    ---------------

    Ordena os individuos do torneio de acordo com seu fitness.
    A ordenação é feita em ordem crescente.
*/
void ordenaTorneio(){
    int i, j, k;
    int auxiliar;
    int auxiliarA[TAMANHOTABULEIRO];

    for (i=0; i<QUANTIDADEINDIVIDUOSPORTORNEIO; i++){
        auxiliar = fitnessTorneio[i];
        for (k=0; k<TAMANHOTABULEIRO; k++)
            auxiliarA[k] = individuosTorneio[i][k];

        for (j=i; (j>0) && auxiliar<fitnessTorneio[j-1]; j--){
            fitnessTorneio[j] = fitnessTorneio[j-1];
            for (k=0; k<TAMANHOTABULEIRO; k++)
                individuosTorneio[j][k] = individuosTorneio[j-1][k];
        }
        fitnessTorneio[j] = auxiliar;
        for (k=0; k<TAMANHOTABULEIRO; k++)
            individuosTorneio[j][k] = auxiliarA[k];
    }

    printf ("Torneio ordenado pelo fitness: \n");
    for (i=0; i<QUANTIDADEINDIVIDUOSPORTORNEIO; i++){
        for (j=0; j<TAMANHOTABULEIRO; j++)
            printf ("%d ", individuosTorneio[i][j]);
        printf ("\n");
    }
    printf("\n");
}

/*
    ------------------
    posicionaRainhas()
    ------------------

    Posiciona as rainhas no tabuleiro
    0 indica posição vazia
    1 indica Rainhas
*/
void posicionaRainhas(int individuo){
    int i, j;
    int linhaDaRainha;

    for (i=0; i<TAMANHOTABULEIRO; i++){
        for (j=0; j<TAMANHOTABULEIRO; j++){
            linhaDaRainha = populacaoAtual[individuo][j];
            if (i == (linhaDaRainha-1))
                tabuleiro[i][j] = 1;
            else
                tabuleiro[i][j] = 0;
        }
    }
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
                pai[i][j] = populacaoAtual[individuo][j];
                printf("%d ", pai[i][j]);
            }
            printf("\n");
        }
        printf("\n");

        if (i>1){
            for (i=0; i<TAMANHOTABULEIRO; i++){
                if (pai[0][i] == pai[1][i])
                    cont++;
            }
        }

        if (cont == TAMANHOTABULEIRO)
            printf ("PAIS IGUAIS, SELECIONE PAIS DIFERENTES!\n\n");
    } while (cont == TAMANHOTABULEIRO);
}

/*
    ---------------
    selecaoRoleta()
    ---------------

    Representa proporcionalmente os individuos na roleta de acordo com seu fitness.
    Gira a roleta e seleciona um pai.
    O precesso se repete até que exista dois pais diferentes entre si.
*/
void selecaoRoleta(){
    int i, j, k;
    int somaFitness = 0;
    int numeroSorteado, fitnessAcumulado, cont;

    // Soma os fitness da população
    for (i=0; i<TAMANHOPOPULACAO; i++)
        somaFitness += fitnessDaPopulacao[i];

    printf ("-> Soma dos fitness: %d\n\n", somaFitness);

    for (k=0; k<QUANTIDADEPAIS; k++){
        do{
            cont = 0;
            // Roda a roleta (sorteia um numero aleatorio)
            numeroSorteado = (rand()%somaFitness);
            printf("NUMERO SORTEADO: %d\n", numeroSorteado);

            // Seleciona um pai
            fitnessAcumulado = 0;
            for (i=0; (i<TAMANHOPOPULACAO) && (fitnessAcumulado <= numeroSorteado); i++){
                fitnessAcumulado += fitnessDaPopulacao[i];
                if (fitnessAcumulado > numeroSorteado){
                    for (j=0; j<TAMANHOTABULEIRO; j++)
                        pai[k][j] = populacaoAtual[i][j];
                }
            }

            // Verifica se os pais são iguais
            if (k>1){
                for (i=0; i<TAMANHOTABULEIRO; i++){
                    if (pai[0][i] == pai[1][i])
                        cont++;
                }
            }

            if (cont == TAMANHOTABULEIRO){
                printf ("PAIS IGUAIS, SELECIONE PAIS DIFERENTES!\n\n");
            } 
            // Mostra o pai sorteado
            else{
                printf ("Pai %d: ", k+1);
                for (i=0; i<TAMANHOTABULEIRO; i++)
                    printf ("%d", pai[k][i]);
            }
            printf ("\n");
        } while(cont == TAMANHOTABULEIRO);
    }
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

    cont1 = 0;
    for (k=0; k<QUANTIDADEPAIS; k++){
        // Seleciona individuos para o torneio
        for (i=0; i < QUANTIDADEINDIVIDUOSPORTORNEIO; i++){
            individuo[i] = (rand()%TAMANHOPOPULACAO);

            // Verifica se o individuo já foi selecionado (ta errado)
            for (j=0; j<i; j++){
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
            pai[k][i] = individuosTorneio[QUANTIDADEINDIVIDUOSPORTORNEIO-1][i];
            printf("%d ", pai[k][i]);
        }
        printf("\n\n");
    }

    // Verifica se os pais sao iguais
    if (k>1){
        for (i=0; i<TAMANHOTABULEIRO; i++){
            if (pai[0][i] == pai[1][i])
                cont1++;
        }
    }

    if (cont1 == TAMANHOTABULEIRO){
        printf ("PAIS IGUAIS, SELECIONE PAIS DIFERENTES!\n\n");
        printf ("Pai 2: ");
        for (i=0; i<TAMANHOTABULEIRO; i++){
            pai[1][i] = individuosTorneio[QUANTIDADEINDIVIDUOSPORTORNEIO-2][i];
            printf ("%d ", pai[1][i]);
        }
        printf ("\n");
    }

}

