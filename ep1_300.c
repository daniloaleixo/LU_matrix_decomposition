#include "stringOps.h"
#include <math.h>


/* 
    FUNCOES AUXILIARES 
    */
void interpretaEntrada(FILE *entrada, double **matrixCoefs, double *vetorB, int n);
int extraiNumeroLinhas(FILE *entrada);
void printMatrixCoefs(double **matrixCoefs, int numeroDeLinhas);
void printVetorB(double *vetorB, int numeroDeLinhas);
double **multiplicaMatriz(double **A, double **B, int na, int nb);

int lucol(int n, double **A, int *p);
int sscol(int n, double **A, int *p, double *b);
int lurow(int n, double **A, int *p);
int ssrow(int n, double **A, int *p, double *b);



int main(int argc, char **argv)
{
    int i = 0;
    double **matrixCoefs;
    double *vetorB;
    int *p;

    int numeroDeLinhas = 0;

    /* lidamos com o arquivo de entrada */
    char *arquivoEntrada = argv[1];
    FILE *entrada = leEntrada(arquivoEntrada);


    /* pegamos o numero de linhas */
    numeroDeLinhas = extraiNumeroLinhas(entrada);

    /* inicializamos a matrix de coeficientes e os vetores */
    matrixCoefs = mallocSafe(numeroDeLinhas * sizeof(double *));
    for(i = 0; i < numeroDeLinhas; i++)
    {
            matrixCoefs[i] = mallocSafe(numeroDeLinhas * sizeof(double));
    }
    /*Vetor: */
    vetorB = mallocSafe(numeroDeLinhas * sizeof(double));
    /* vetor p*/
    p = mallocSafe(numeroDeLinhas * sizeof(int));


    /* vamos interpretar a entrada e colocar os valores nas respectivas matrizes */
    interpretaEntrada(entrada, matrixCoefs, vetorB, numeroDeLinhas);

    /*printMatrixCoefs(matrixCoefs, numeroDeLinhas);
    printVetorB(vetorB, numeroDeLinhas);*/

    printf("decomposiçao LU\n");
    lurow(numeroDeLinhas, matrixCoefs, p);

    printMatrixCoefs(matrixCoefs, numeroDeLinhas);

    printf("sai\n");

    ssrow( numeroDeLinhas, matrixCoefs, p, vetorB);

    /*printMatrixCoefs(matrixCoefs, numeroDeLinhas);*/

    return 0;
}



int extraiNumeroLinhas(FILE *entrada)
{
    int n;
    fscanf(entrada, "%d", &n);
    return n;
}

void interpretaEntrada(FILE *entrada, double **matrixCoefs, double *vetorB, int n)
{
    int i;
    int n2 = n * n;
    int x, y;
    double elemento;



    for(i = 0; i < n2; i++ )
    {
        fscanf(entrada, "%d %d %lf", &x, &y, &elemento);
        /* depuraçao *printf("elemento: %lf, posicao: %d, %d\n", elemento, x, y); */
        matrixCoefs[x][y] = elemento;
    }

    /* depuraçao *
    printf("matrixCoefs\n");
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n; j++)
            printf("[%d][%d]: %lf\n",i, j, matrixCoefs[i][j] );
    }
    * *888888888888888888*/

    for(i = 0; i < n; i++)
    {
        fscanf(entrada, "%d %lf", &x, &elemento);
        /* depuraçao *printf("elemento: %lf, posicao: %d\n", elemento, x);*/
        vetorB[x] = elemento;
    }
}

double **multiplicaMatriz(double **A, double **B, int na, int nb)
{
    int i, j, k;

    if (na != nb)
    {
        fprintf(stderr, "Matrizes nao podem ser multiplicadas\n");
        return NULL;
    }
        
    /* aloca matriz */    
    double **r =  mallocSafe(na * sizeof(double *));
    for(i = 0; i < na; i++)
    {
            r[i] = mallocSafe(na * sizeof(double));
    }

    /* resolve */
    for (i = 0; i < na; ++i) 
    {
        for (j = 0; j < nb; ++j) 
        {
            for (k = 0; k < na; ++k)
                r[i][j] += A[i][k] * B[k][j];
        }
    }
    return r;
}

void printMatrixCoefs(double **matrixCoefs, int n)
{
    int i, j;
    printf("matrix de Coefs:\n");
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n; j++)
            printf("[%d][%d]: %lf \n", i, j, matrixCoefs[i][j]);
    }
}


void printVetorB(double *vetorB, int n)
{
    int j;
    printf("vetor B:\n");
    for(j = 0; j < n; j++)
        printf("[%d]: %lf \n", j, vetorB[j]);
}

void matrixDuplicate(double **A, double **B, int n)
{
    int i, j;

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            A[i][j] = B[i][j];
        }
    }
}

void swap(double* A, double* B)
{
    double temp;
    /*double *temp = mallocSafe(n * sizeof(double));
    int i;
    for(i = 0; i < n; i++)
    {*/
        temp = *A;
        *A = *B;
        *B = temp;
}

int lurow(int n, double **A, int *p)
{
    int i, j, k;
    int imax;


    /*printf("&&&&&&&&&&&&&lurow:\n");
    printMatrixCoefs(A, n);
    printf("&&&&&&&&&&&&&&\n\n");*/



    for(k = 0; k < n; k++)
    {
        imax = k;
        for(i = k + 1; i < n; i++)
        {
            /*printf("ANTES --- *****k:%d e imax: %d\n A[i][k]: %lf e A[imax][k]: %lf \n\n", k, imax, A[i][k], A[imax][k]);
            */
            if( fabs(A[i][k]) > fabs(A[imax][k]))
            {
                imax = i;
                /*printf("entrei aquia\n");*/
            }
            /*printf("DEPOIS -- *****k:%d e imax: %d\n A[i][k]: %lf e A[imax][k]: %lf \n\n", k, imax, A[i][k], A[imax][k]);
            */

            if(fabs( A[imax][k] ) < 0.00000001)
            {
                printf("Parar pois A parece singular\n");
                return -1;
            }

            if(imax != k)
            {
                for(j = 0; j < n; j++)
                    swap(&A[imax][j], &A[k][j]);
            }

            p[k] = imax;

            for(i = k + 1; i < n; i++)
            {
                A[i][k] = A[i][k] / A[k][k];
                for(j = k + 1; j < n; j++)
                {
                    A[i][j] = A[i][j] - A[i][k] * A[k][j];
                }
            }

        }
    }

    return 1;

}


int ssrow(int n, double **A, int *p, double *b)
{
    int i, j, k;
    double *y = mallocSafe(n * sizeof(double));



    for(i = 0; i < n; i++)
    {
        swap(&b[i], &b[p[i]]);
    }


    /* 
    VAMOS RESOLVER Ly = b, onde y = Ux */

    for(i = 0; i < n; i++)
    {
        for(j = 0; j < i - 1; j++)
        {
            b[i] = b[i] - A[i][j] * b[j];
        }
        if(A[i][i] == 0)
        {
            printf("A e singular\n");
            return -1;
        }
        b[i] = b[i] / A[i][i];
    }

    /* vamos resolver Ux = b */

    for(i = n - 1; i >= 0; i--)
    {
        for(j = i + 1; j < n; j++)
        {
            b[i] = b[i] - A[i][j] * b[j];
        }
        b[i] = b[i] / A[i][i];

    }

    return 1;


    /*printVetorB(b, n);*/
}


int lucol(int n, double **A, int *p)
{
    int i, j, k;
    int imax;


    /*printf("&&&&&&&&&&&&&lurow:\n");
    printMatrixCoefs(A, n);
    printf("&&&&&&&&&&&&&&\n\n");*/



    for(k = 0; k < n; k++)
    {
        imax = k;
        for(i = k + 1; i < n; i++)
        {
            /*printf("ANTES --- *****k:%d e imax: %d\n A[i][k]: %lf e A[imax][k]: %lf \n\n", k, imax, A[i][k], A[imax][k]);
            */
            if( fabs(A[i][k]) > fabs(A[imax][k]))
            {
                imax = i;
                /*printf("entrei aquia\n");*/
            }
            /*printf("DEPOIS -- *****k:%d e imax: %d\n A[i][k]: %lf e A[imax][k]: %lf \n\n", k, imax, A[i][k], A[imax][k]);
            */

            if(fabs( A[imax][k] ) < 0.00000001)
            {
                printf("Parar pois A parece singular\n");
                return -1;
            }

            if(imax != k)
            {
                for(j = 0; j < n; j++)
                    swap(&A[imax][j], &A[k][j]);
            }

            p[k] = imax;

            for(j = k + 1; j < n; j++)
            {
                for(i = k + 1; i < n; i++)
                {
                    A[i][j] = A[i][j] - A[i][k] * A[k][j];
                }
                A[j][k] = A[j][k] / A[k][k];
            }

        }
    }

    return 1;

}


int sscol(int n, double **A, int *p, double *b)
{
    int i, j, k;
    double *y = mallocSafe(n * sizeof(double));



    for(i = 0; i < n; i++)
    {
        swap(&b[i], &b[p[i]]);
    }


    /* 
    VAMOS RESOLVER Ly = b, onde y = Ux */

    for(j = 0; j < n; j++)
    {
        if(A[j][j] == 0)
        {
            printf("A e singular\n");
            return -1;
        }

        b[j] = b[j] / A[j][j];

        for(i = j + 1; i < n; i++)
        {
            b[i] = b[i] - A[i][j] * b[j];
        }
    }

    /* vamos resolver Ux = b */

    for(i = 0; i < n; i++)
    {
        for(j = 0; j <= i; j++)
        {
            b[i] = b[i] - A[j][i] * b[j];
        }
        b[i] = b[i] / A[i][i];

    }

    return 1;


    /*printVetorB(b, n);*/
}