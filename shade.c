//
// shade.c - shade input image
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "bmp.h"
#define CMIN 80000.0

extern void shade_optimized(int width, int height);

byte bmp_hdr[BMP_HDR_SIZE];
byte a[MAX_ROW][MAX_COL][3]; // input
byte b[MAX_ROW][MAX_COL][3]; // naive output
byte c[MAX_ROW][MAX_COL][3]; // optimized output
byte threshold[3][2];
byte shade[3];

//
// read shade file, shade.txt
//
int read_threshold(char *filename, byte threshold[3][2], byte shade[3])
{
  FILE *fp;
  byte *ptr; 
  int i, x;

  fp = fopen(filename,"r+");
  ptr = threshold[0]; // short for &threshold[0][0];
  for (i=0; i<6; i++)
    if (!feof(fp))
    {
      fscanf(fp, "%d", &x);
      *ptr = (byte) x;
      ptr++;
    }
  ptr = shade; // short for &shade[0]
  for (i=0; i<3; i++)
    if (!feof(fp))
    {
      fscanf(fp, "%d", &x);
      *ptr = (byte) x;
      ptr++;
    }
  fclose(fp);
  printf("Low Threshold: Blue = %d, Green = %d, Red = %d\n", threshold[0][0],threshold[1][0],threshold[2][0]);
  printf("High Threshold: Blue = %d, Green = %d, Red = %d\n", threshold[0][1],threshold[1][1],threshold[2][1]);
  printf("Shade Color: Blue = %d, Green = %d, Red = %d\n", shade[0],shade[1],shade[2]);
}

//
// naive version -- base version, leave alone
//
void shade_naive(int width, int height)
{
  int k, x, y, z;
  int inRange;

  for (z=0; z<3; z++)
    for (x=0; x<height; x++)
      for (y=0; y<width; y++)
      {
        inRange = 1;
        for (k=0; k<3; k++)
        {
          if(a[x][y][k]<threshold[k][0])
            inRange = 0;
          if(a[x][y][k]>threshold[k][1])
            inRange = 0;
        }
        if (inRange)
          b[x][y][z]=a[x][y][z];
        else
          b[x][y][z]=shade[z];
      }

   // set border pixels to 0,0,0 - black
   for (z=0; z<3; z++)
     for (x=0; x<height; x++)
       for (y=0; y<width; y++)
         if ((x==0)||(y==0)||(x==height-1)||(y==width-1))
           b[x][y][z] = 0;

  return;
}

int main(int argc, char *argv[])
{
  int i=0;
  int x=0, y=0, z=0;
  int width, height, size;
  double cycles, Mhz;
  int cnt = 1;
  double start_time, end_time, elapsed_time;
  double time_naive, time_optimized;
  double cycles_naive, cycles_optimized;

  Mhz = 2700.0; // mhz(1);

  if (argc<4)
  {
    fprintf(stderr, "Usage: shade <shade.txt> <input.bmp> <base output.bmp> [optimized output.bmp]\n");
    exit(1);
  }
  read_threshold(argv[1], threshold, shade);
  read_bmp(argv[2], bmp_hdr, a, &size, &width, &height);
  do {
    int c = cnt;
    printf("cnt = %d\n", cnt);
    // Warm up cache
    shade_naive(width, height);
    start_time = omp_get_wtime();
    while (c-- > 0)
    {
      // shade
      shade_naive(width, height);
    }
    end_time = omp_get_wtime();
    elapsed_time = end_time-start_time;
    cycles = elapsed_time*Mhz*1.0E6; // compute cycles
    cnt += cnt;
  } while (cycles < CMIN);  /* Make sure we have enough */
  cnt = cnt>>1;
  printf("Naive cnt = %d\n", cnt);
  time_naive = elapsed_time/cnt;
  printf("Naive elapsed time per image = %12.10lf seconds \n", time_naive);
  // Compute CPE - cycles per element (pixel) = cycles_naive/(width*height)
  cycles_naive = time_naive*2.7E9;

  printf("Naive CPE = %12.5lf cycles \n", cycles_naive/((double)width*height));
  write_bmp(argv[3], bmp_hdr, b, width, height);

  if (argc > 4)
  {
    cnt = 1;
    do {
      int c = cnt;
      printf("cnt = %d\n", cnt);
      // Warm up cache
      shade_optimized(width, height);
	  start_time = omp_get_wtime();
      while (c-- > 0)
      {
        // shade
        shade_optimized(width, height);
      }
      end_time = omp_get_wtime();
      elapsed_time = end_time-start_time;
      cycles = elapsed_time*Mhz*1.0E6; // compute cycles
      cnt += cnt;
    } while (cycles < CMIN);  /* Make sure we have enough */
    cnt = cnt>>1;
    printf("Optimized cnt = %d\n", cnt);
    time_optimized = elapsed_time/cnt;
    printf("Optimized elapsed time per image = %12.10lf seconds \n", time_optimized);
    // Compute CPE - cycles per element (pixel) = cycles_optimized/(width*height)
    cycles_optimized = time_optimized*2.7E9;
    write_bmp(argv[4], bmp_hdr, c, width, height);
    // Compute speedup
    printf("Speedup = %12.5lf\n", time_naive/time_optimized);
  }
  return 0;
}
