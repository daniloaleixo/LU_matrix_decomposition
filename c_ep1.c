#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <math.h>

#define EPS 1e-20

/* 
    FUNCOES AUXILIARES 
    */
void interpretaEntrada(FILE *entrada, double **matrixCoefs, double *vetorB, int n);
void printMatrixCoefs(double **matrixCoefs, int numeroDeLinhas);
void printVetorB(double *vetorB, int numeroDeLinhas);
void * mallocSafe (size_t n);

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
    char *arquivoEntrada;
    FILE *entrada;

    /* le da entrada padrao se nao receber argumentos */
    if (argc != 2) {
      printf("Nao recebeu nenhum argumento. Lendo da entrada padrao...\n");
      entrada = stdin;
    } else {
      arquivoEntrada = argv[1];
      entrada = fopen(arquivoEntrada, "r");
      if (entrada == NULL) {
        printf("Nao consegui abrir %s\n", arquivoEntrada);
      }
    }

    /* pegamos o numero de linhas */
    if (fscanf(entrada, "%d", &numeroDeLinhas) == EOF) {
      printf("Erro ao ler o arquivo\n");
      return -1;
    }

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

    if (lucol(numeroDeLinhas, matrixCoefs, p) == -1) {
      return -1;
    }

    if (sscol( numeroDeLinhas, matrixCoefs, p, vetorB) == -1) {
      return -1;
    }

    /*printMatrixCoefs(matrixCoefs, numeroDeLinhas);
    printVetorB(vetorB, numeroDeLinhas);*/

    return 0;
}

/*
  mallocSafe: testa o ponteiro devolvido por malloc
  co'digo do professor Paulo Feofiloff
 */
void * mallocSafe (size_t n)
{
  void * pt;
  pt = malloc(n);
  if (pt == NULL) {
    printf("ERRO na alocacao de memoria.\n\n");
    exit(-1);
  }
  return pt;
}

void interpretaEntrada(FILE *entrada, double **matrixCoefs, double *vetorB, int n)
{
    int i;
    int n2 = n * n;
    int x, y;
    double elemento;

    for(i = 0; i < n2; i++ )
    {
      if (fscanf(entrada, " %d %d %lf", &x, &y, &elemento) == EOF) {
        printf("Erro ao ler o arquivo.\n");
        exit(-1);
      }
        matrixCoefs[x][y] = elemento;
    }

    for(i = 0; i < n; i++)
    {
      if (fscanf(entrada, " %d %lf", &x, &elemento) == EOF) {
        printf("Erro ao ler o arquivo.\n");
        exit(-1);
      }
        vetorB[x] = elemento;
    }
}

void printMatrixCoefs(double **matrixCoefs, int n)
{
    int i, j;
    printf("matrix de Coefs:\n");
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n; j++)
            printf("% .2f ", matrixCoefs[i][j]);
        printf("\n");
    }
}


void printVetorB(double *vetorB, int n)
{
    int j;
    printf("vetor B:\n");
    for(j = 0; j < n; j++)
        printf("[%d]: %.3f \n", j, vetorB[j]);
}

void swap(double* A, double* B)
{
    double temp;
    temp = *A;
    *A = *B;
    *B = temp;
}

/* orientadas a linha: */
int lurow(int n, double **A, int *p)
{
    int i, j, k;
    int imax;


    for(k = 0; k < n; k++)
    {
        imax = k;
        for(i = k + 1; i < n; i++)
        {
            if( fabs(A[i][k]) > fabs(A[imax][k]))
            {
                imax = i;
            }
        }

        if(fabs( A[imax][k] ) < EPS)
        {
          printf("A parece singular %d %d\n", imax, k);
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
          /* loop mais interno: */
          for(j = k + 1; j < n; j++)
          {
            A[i][j] = A[i][j] - A[i][k] * A[k][j];
          }
        }

    }

    return 0;

}

int ssrow(int n, double **A, int *p, double *b)
{
    int i, j;

    /* 'conserta' b */
    for(i = 0; i < n; i++)
    {
        swap(&b[i], &b[p[i]]);
    }

    /* 
    VAMOS RESOLVER Ly = b, onde y = Ux */

    for(i = 0; i < n; i++)
    {
        /* loop mais interno: */
        for(j = 0; j < i; j++)
        {
            b[i] = b[i] - A[i][j] * b[j];
        }
        /* L[i][i] = 1 sempre, entao nao precisamos dividir */
    }

    /* vamos resolver Ux = b */

    for(i = n - 1; i >= 0; i--)
    {
        /* loop mais interno: */
        for(j = i + 1; j < n; j++)
        {
            b[i] = b[i] - A[i][j] * b[j];
        }
        if(fabs(A[i][i]) < EPS)
        {
            printf("A e singular\n");
            return -1;
        }
        b[i] = b[i] / A[i][i];

    }

    return 0;
}

/* orientadas a coluna: */
int lucol(int n, double **A, int *p)
{
    int i, j, k;
    int imax;

    for(k = 0; k < n; k++)
    {
        imax = k;
        for(i = k + 1; i < n; i++)
        {
            if( fabs(A[i][k]) > fabs(A[imax][k]))
            {
                imax = i;
            }
        }

        if(fabs( A[imax][k] ) < EPS)
        {
          printf("A parece singular %d %d\n", imax, k);
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
        }
        for (j = k +1; j < n; j++)
        {
          /* loop mais interno: */
          for(i = k + 1; i < n; i++)
          {
            A[i][j] = A[i][j] - A[i][k] * A[k][j];
          }
        }

    }

    return 0;

}

int sscol(int n, double **A, int *p, double *b)
{
    int i, j;

    /* 'conserta' b */
    for(i = 0; i < n; i++)
    {
        swap(&b[i], &b[p[i]]);
    }

    /* 
    VAMOS RESOLVER Ly = b, onde y = Ux */

    /* L[i][i] = 1 sempre, entao nao precisamos dividir */
    for(j = 0; j < n; j++)
    {
        /* loop mais interno: */
        for(i = j + 1; i < n; i++)
        {
            b[i] = b[i] - A[i][j] * b[j];
        }
    }

    /* vamos resolver Ux = b */

    for(j = n - 1; j >= 0; j--)
    {
        if(fabs(A[j][j]) < EPS)
        {
            printf("A e singular\n");
            return -1;
        }
        b[j] = b[j] / A[j][j];
        /* loop mais interno: */
        for(i = 1; i < j; i++)
        {
            b[i] = b[i] - A[i][j] * b[j];
        }

    }

    return 0;
}
