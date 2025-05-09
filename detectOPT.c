//
// detectOPT.c - optimized function - apply mask to all interior points
//               to detect edges
//
#include "bmp.h"  // for MAX_ROW, MAX_COL, NUM_COLORS

extern byte a[MAX_ROW][MAX_COL][NUM_COLORS];
extern byte c[MAX_ROW][MAX_COL][NUM_COLORS];

void detect_optimized(int width, int height, int threshold)
{
  int x, y, z;
  int tmp;
  int outputColor; // set outputColor to 255 for white or 0 for black
  int mask[3][3] = {{-1, 0, -1},
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
        c[x][y][z] = outputColor;
    }
  return;

}
