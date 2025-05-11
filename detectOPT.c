//
// detectOPT.c - optimized function - apply mask to all interior points
//               to detect edges
//
#include "bmp.h"  // for MAX_ROW, MAX_COL, NUM_COLORS
#include <omp.h>

#define BLOCK_SIZE 64

extern byte a[MAX_ROW][MAX_COL][NUM_COLORS];
extern byte c[MAX_ROW][MAX_COL][NUM_COLORS];

void detect_optimized(int width, int height, int threshold)
{
  int x_block, y_block, x_end, y_end, x, y, r, g, b, offset;
  #pragma omp parallel for schedule(static)
  for (x_block = 1; x_block < height - 1; x_block += BLOCK_SIZE) {
    x_end = (x_block + BLOCK_SIZE < height-1) ? x_block + BLOCK_SIZE : height-1;
    for (int y_block = 1; y_block < width-1; y_block += BLOCK_SIZE) {
      y_end = (y_block + BLOCK_SIZE < width-1) ? y_block + BLOCK_SIZE : width-1;
      
      for (x = x_block; x < x_end; x++) {
        unsigned char *in_m1 = &a[x-1][0][0];
        unsigned char *in_0 = &a[x][0][0];
        unsigned char *in_p1 = &a[x+1][0][0];
        unsigned char *row_out = &c[x][1][0];
        for (y = y_block; y < y_end; y++) {
          unsigned char *out = row_out + (y-1)*3;
          offset = y * 3;

          r = -in_m1[offset-3] + 4*in_0[offset] - in_p1[offset-3]
              - in_m1[offset+3] - in_p1[offset+3];
          g = -in_m1[offset-2] + 4*in_0[offset+1] - in_p1[offset-2]
              - in_m1[offset+4] - in_p1[offset+4];
          b = -in_m1[offset-1] + 4*in_0[offset+2] - in_p1[offset-1]
              - in_m1[offset+5] - in_p1[offset+5];
      
          out[0] = out[1] = out[2] = (r > threshold || g > threshold || b > threshold) ? 0 : 255;
        }
      }
    }
  }
  return;
}
