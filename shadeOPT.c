//
// shadeOPT.c - optimized shade function
//
#include "bmp.h"
#include <omp.h>
#include <string.h>

extern byte a[MAX_ROW][MAX_COL][3]; // input
extern byte c[MAX_ROW][MAX_COL][3]; // optimized output
extern byte threshold[3][2];
extern byte shade[3];

//
// optimized version - modify this function
//
void shade_optimized(int width, int height)
{
  // set edge pixels to black
  memset(&c[0][0][0], 0, width * 3);
  memset(&c[height-1][0][0], 0, width * 3);
  for (int x = 1; x < height - 1; x++) {
    memset(&c[x][0][0], 0, 3);
    memset(&c[x][width-1][0], 0, 3);
  }

  // preloading thresholds and shades
  unsigned char t0_min = threshold[0][0];
  unsigned char t0_max = threshold[0][1];
  unsigned char t1_min = threshold[1][0];
  unsigned char t1_max = threshold[1][1];
  unsigned char t2_min = threshold[2][0];
  unsigned char t2_max = threshold[2][1];
  unsigned char s0 = shade[0];
  unsigned char s1 = shade[1];
  unsigned char s2 = shade[2];

  int x, y, inRange;

  #pragma omp parallel for private(y)
  for (x = 1; x < height - 1; x++) {
    unsigned char *row_in = &a[x][1][0];
    unsigned char *row_out = &c[x][1][0];
    for (y = 1; y < width - 1; y++) {
      unsigned char *in  = row_in + (y-1)*3;
      unsigned char *out = row_out + (y-1)*3;
      inRange = 
        (in[0] >= t0_min && in[0] <= t0_max) &
        (in[1] >= t1_min && in[1] <= t1_max) &
        (in[2] >= t2_min && in[2] <= t2_max);

      out[0] = inRange ? in[0] : s0;
      out[1] = inRange ? in[1] : s1;
      out[2] = inRange ? in[2] : s2;
    }
  }
  return;
}
