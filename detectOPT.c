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
  #pragma omp parallel for
  for (int x = 1; x < height - 1; x++) {
    for (int y = 1; y < width - 1; y++) {

      int r = -a[x-1][y][-3] + 4*a[x][y][0] - a[x+1][y][-3]
          - a[x-1][y][3] - a[x+1][y][3];
      int g = -a[x-1][y][-2] + 4*a[x][y][1] - a[x+1][y][-2]
          - a[x-1][y][4] - a[x+1][y][4];
      int b = -a[x-1][y][-1] + 4*a[x][y][2] - a[x+1][y][-1]
          - a[x-1][y][5] - a[x+1][y][5];
  
      c[x][y][0] = c[x][y][1] = c[x][y][2] = (r > threshold || g > threshold || b > threshold) ? 0 : 255;
    }
  }
  return;
}
