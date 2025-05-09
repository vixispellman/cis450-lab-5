//
// shadeOPT.c - optimized shade function
//
#include "bmp.h"
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
          c[x][y][z]=a[x][y][z];
        else
          c[x][y][z]=shade[z];
      }

   // set border pixels to 0,0,0 - black
   for (z=0; z<3; z++)
     for (x=0; x<height; x++)
       for (y=0; y<width; y++)
         if ((x==0)||(y==0)||(x==height-1)||(y==width-1))
           c[x][y][z] = 0;

  return;
}
