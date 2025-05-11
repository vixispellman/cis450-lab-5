//
// multiplyOPT.c - function to compute product of two NxN matrices
//
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

extern int numThreads;

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

void multiply_matrix_semi_opt(long *A, long *B, long *C, int N) {
  // Use aligned_alloc to be cache-friendly
  long *B2 = (long *)aligned_alloc(64, sizeof(long) * N * N);
  // Transpose to be cache-friendly
  transpose(B, B2, N);

  for (int iN = 0; iN < N * N; iN += N)
    for (int j = 0; j < N; j++) {
      // Common Subexpression
      const int jN = j * N;
      // Avoid Volatile
      long temp = 0;
      // Loop Unrolling
      int k;
      for (k = 0; k < N - 3; k += 4)
        temp += A[iN + k] * B2[jN + k]
                + A[iN + k + 1] * B2[jN + k + 1]
                + A[iN + k + 2] * B2[jN + k + 2]
                + A[iN + k + 3] * B2[jN + k + 3];
      for (; k < N; k++)
        temp += A[iN + k] * B2[jN + k];
      C[iN + j] = temp;
    }

  free(B2);

  return;
}

void multiply_matrix_opt(long *A, long *B, long *C, int N) {
  // Use aligned_alloc to be cache-friendly
  long *B2 = (long *)aligned_alloc(64, sizeof(long) * N * N);
  // Transpose to be cache-friendly
  transpose(B, B2, N);

  // Add Threads
  #pragma omp parallel for
  for (int iN = 0; iN < N * N; iN += N)
    for (int j = 0; j < N; j++) {
      // Common Subexpression
      const int jN = j * N;
      // Avoid Volatile
      long temp = 0;
      // Loop Unrolling
      int k;
      for (k = 0; k < N - 3; k += 4)
        temp += A[iN + k] * B2[jN + k]
                + A[iN + k + 1] * B2[jN + k + 1]
                + A[iN + k + 2] * B2[jN + k + 2]
                + A[iN + k + 3] * B2[jN + k + 3];
      for (; k < N; k++)
        temp += A[iN + k] * B2[jN + k];
      C[iN + j] = temp;
    }

  free(B2);

  return;
}
