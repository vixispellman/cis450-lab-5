//
// multiply.h - multiply header file
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <omp.h>

// Function prototypes
//
void multiply_matrix_semi_opt(long *, long *, long *, int);
void multiply_matrix_opt(long *, long *, long *, int);

