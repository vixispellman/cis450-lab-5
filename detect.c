//
// detect.c - apply mask to all interior points to detect edges, allow a threshold to
//          - be specfied as well to filter out small values
//          - set edge output to black color = (0,0,0) 
//
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "bmp.h"
#define CMIN 80000

extern void detect_optimized(int width, int height, int threshold);

byte bmp_hdr[BMP_HDR_SIZE];
byte a[MAX_ROW][MAX_COL][NUM_COLORS];
byte b[MAX_ROW][MAX_COL][NUM_COLORS];
byte c[MAX_ROW][MAX_COL][NUM_COLORS];

//
// detect_naive - the naive baseline version
//
void detect_naive(int width, int height, int threshold)
{
  volatile int x, y, z;
  volatile int tmp;
  volatile int outputColor; // set outputColor to 255 for white or 0 for black
  volatile int mask[3][3] = {{-1, 0, -1},
                             { 0, 4,  0},
                             {-1, 0, -1}};

  for (y = 1; y < width-1; y++)
    for (x = 1; x < height-1; x++)
    {
      outputColor = 255; // in any computed color > threshold, set color to black (an edge)
      for (z = 0; z < 3; z++)
      {
        tmp = mask[0][0]*a[x-1][y-1][z]+
              mask[1][0]*a[x][y-1][z]+
              mask[2][0]*a[x+1][y-1][z]+
              mask[0][1]*a[x-1][y][z]+
              mask[1][1]*a[x][y][z]+
              mask[2][1]*a[x+1][y][z]+
              mask[0][2]*a[x-1][y+1][z]+
              mask[1][2]*a[x][y+1][z]+
              mask[2][2]*a[x+1][y+1][z];
        if (tmp>threshold)
              outputColor = 0;
      }
      for (z = 0; z < 3; z++)
        b[x][y][z] = outputColor;
    }
  return;
}

int main(int argc, char *argv[])
{
  int i=0;
  int x=0, y=0, z=0;
  int cnt=1;
  int width, height, size;
  double start_time, end_time, elapsed_time;
  double min_time = CMIN/2700000000.0; // min_time to sec.
  double time_naive, time_optimized;
  double cycles;
  int threshold = 0;

  if (argc<4)
  {
    fprintf(stderr, "Usage: detect <threshold> <input> <base output> [optimized output]\n");
    return(1);
  }
  threshold = atoi(argv[1]);
  if (threshold < 0)
    threshold = 0;

  read_bmp(argv[2], bmp_hdr, a, &size, &width, &height);
  do {
    int n = cnt;
    printf("cnt = %d\n", cnt);
    // Warm up cache
    detect_naive(width, height, threshold);
    start_time = omp_get_wtime();
    while (n-- > 0)
    {
      detect_naive(width, height, threshold);
    }
    end_time = omp_get_wtime();
    elapsed_time = end_time - start_time;
    cnt += cnt;
  } while (elapsed_time < min_time); /* Take enough time for accurate measurement */
  cnt = (cnt>>1); /* The cnt value used for the measurement */
  printf("Naive cnt = %d\n", cnt);
  time_naive = elapsed_time/cnt;
  printf("Naive elapsed time per image = %12.10lf seconds \n",time_naive);
  // Compute CPE - cycles per element (pixel) = cycles/(width*height)
  cycles = time_naive * 2.7E9;
  printf("Naive CPE = %f cycles \n", cycles/((double)width*height));
  write_bmp(argv[3], bmp_hdr, b, width, height);

  if (argc > 4)
  {
    cnt = 1;
    do {
      int n = cnt;
      printf("cnt = %d\n", cnt);
      // Warm up cache
      detect_optimized(width, height, threshold);
      start_time = omp_get_wtime();
      while (n-- > 0)
      {
        detect_optimized(width, height, threshold);
      }
      end_time = omp_get_wtime();
      elapsed_time = end_time - start_time;
      cnt += cnt;
    } while (elapsed_time < min_time); /* Take enough time for accurate measurement */
    cnt = (cnt>>1); /* The cnt value used for the measurement */
    printf("Optimized cnt = %d\n", cnt);
    time_optimized = elapsed_time/cnt;
    printf("Optimized elapsed time per image = %12.10lf seconds \n",time_optimized);
    // Compute CPE - cycles per element (pixel) = cycles/(width*height)
    cycles = time_optimized * 2.7E9;
    printf("Optimized CPE = %f cycles \n", cycles/((double)width*height));
    write_bmp(argv[4], bmp_hdr, c, width, height);
    // Compute speedup
    printf("Speedup = %f\n", time_naive/time_optimized);
  }
  return 0;
}

