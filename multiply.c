//
// multiply.c - compute product of two NxN matrices (and compute CPE)
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <omp.h>
#include "multiply.h"
 
void multiply_matrix_semi_opt(long *a, long *b, long *c, int N);
void multiply_matrix_opt(long *a, long *b, long *c, int N);

int numThreads = 1;

//
// naive version -- base version, leave alone
//
void multiply_matrix_base(long *a, long *b, long *c, int N)
{
  int i, j, k;

  for (i=0; i<N; i++)
    for (j=0; j<N; j++)
      c[i*N+j]=0;

  for (k=0; k<N; k++)
    for (j=0; j<N; j++)
      for (i=0; i<N; i++)
        c[i*N+j] += a[i*N+k]*b[k*N+j];

  return;
}

int main(int argc, char *argv[]) {
  int N, N2;
  int i;
  double start_time, naive_time, semi_optimized_time, optimized_time;

  if (argc>1)
      N=atoi(argv[1]);
  else
      N=700;

  if (argc>2)
      numThreads = atoi(argv[2]);
  else
      numThreads = 28;

  N2 = N*N;
  long *A = malloc(N2*sizeof(long));
  long *B = malloc(N2*sizeof(long));
  long *C = malloc(N2*sizeof(long));
  long *C1 = malloc(N2*sizeof(long));
  long *C2 = malloc(N2*sizeof(long));

  // load random data into A and B;
  srand(time(NULL));
  for(i=0; i<N2; i++) {
      A[i] = rand()/1000.0d;
      B[i] = rand()/1000.0d;
  }

  // Base run
  // Warm up cache
  multiply_matrix_base(A, B, C, N);
  // Measure execution time
  start_time = omp_get_wtime();
  multiply_matrix_base(A,B,C,N);
  naive_time = omp_get_wtime() - start_time;
  printf("Naive time: %f sec.\n", naive_time);

  // Semi-optimized run, no threads used
  // Warm up cache
  multiply_matrix_semi_opt(A, B, C1, N);
  // Measure execution time
  start_time = omp_get_wtime();
  multiply_matrix_semi_opt(A,B,C1,N);
  semi_optimized_time = omp_get_wtime() - start_time;
  printf("Semi-optimized time: %f\n", semi_optimized_time);
  printf("Speedup: %f\n", naive_time/semi_optimized_time);

  // Fully optimized, run with threads
  // Warm up cache
  multiply_matrix_opt(A, B, C2, N);
  // Measure execution time
  start_time = omp_get_wtime();
  multiply_matrix_opt(A,B,C2,N);
  optimized_time = omp_get_wtime() - start_time;
  printf("Threaded optimized time: %f\n", optimized_time);
  printf("Speedup: %f\n", naive_time/optimized_time);

  // Check computed values
  for (i=0; i<N2; i++)
  {
    if ((C[i]!=C1[i])||(C[i]!=C2[i]))
    {
      printf("Output differs: C[%d]=%ld, C1[%d]=%ld and C2[%d]=%ld\n\n",
          i, C[i], i, C1[i], i, C2[i]);
      return(1);
    }
  }
  return(0);
}
