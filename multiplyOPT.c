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
    register int i,j,iN;
    register long *aptr, *bptr;
    iN = 0;
    for(i=0; i<N; i++) {
        aptr = &A[iN];
        bptr = &B[i];
        for(j=0; j<N; j++){
            *bptr = (*aptr++);
            bptr+=N;
        }
        iN += N;
    }
}

void multiply_matrix_opt(long *A, long *B, long *C, int N)
{
  int i, j, k;
  register long *B2;
  B2 = (long*)malloc(sizeof(long)*N*N);
  transpose(B,B2,N);

  for (i=0; i<N; i++)
    for (j=0; j<N; j++)
      C[i*N+j]=0;

  for (k=0; k<N; k++)
    for (j=0; j<N; j++)
      for (i=0; i<N; i++)
        C[i*N+j] += A[i*N+k]*B2[j*N+k];

  free(B2);

  return;
}
