//
// multiplyOPT.c - function to compute product of two NxN matrices
//
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

extern int numThreads;

void multiply_matrix_semi_opt(long *A, long *B, long *C, int N)
{
  int i, j, k;

  for (i=0; i<N; i++)
    for (j=0; j<N; j++)
      C[i*N+j]=0;

  for (k=0; k<N; k++)
    for (j=0; j<N; j++)
      for (i=0; i<N; i++)
        C[i*N+j] += A[i*N+k]*B[k*N+j];

  return;

}

void transpose(long *A, long *B, int N) {
    int i,j;
    long *bptr;
    for(i=0; i<N; i++) {
        bptr = &B[i];
        for(j=0; j<N; j++){
            *bptr = (*A++);
            bptr+=N;
        }
    }
}

void multiply_matrix_opt(long *A, long *B, long *C, int N)
{
  long* B2 = (long*)malloc(sizeof(long)*N*N);
  transpose(B,B2,N);

  #pragma omp parallel for
  for (int iN=0; iN<N*N; iN+=N)
    for (int j=0; j<N; j++) {
      long temp = 0;
      for (int k=0; k<N; k++)
        temp += A[iN+k]*B2[j*N+k];
      C[iN+j] = temp;
    }

  free(B2);

  return;
}
