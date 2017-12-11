#ifndef _OMP_H
#   include "omp.h"
#endif
#include <stdio.h>
#define iXmax 800
#define iYmax 800

int main() {
    FILE * fp;
    char *filename="new1.ppm";
    char *comment="# ";/* comment should start with # */
    static unsigned char color[iYmax][iXmax][3];
    static unsigned char MaxColorComponentValue = 255;
    int max = 200;

    double start_time = omp_get_wtime();
#pragma omp parallel for default(none) shared(color,max)
    /* Calculate Mandelbrot set copied from http://jonisalonen.com/2013/lets-draw-the-mandelbrot-set/ */
    for (int row = 0; row < iYmax; row++) {
        for (int col = 0; col < iXmax; col++) {
            double c_re = (col - iXmax/2.0)*4.0/iXmax;
            double c_im = (row - iYmax/2.0)*4.0/iXmax;
            double x = 0, y = 0;
            int iteration = 0;

            while (x*x+y*y <= 4 && iteration < max) {
                double x_new = x*x - y*y + c_re;
                y = 2*x*y + c_im;
                x = x_new;
                iteration++;
            }
            if (iteration < max) {
                color[col][row][0] = 255;
                color[col][row][1] = 255;
                color[col][row][2] = 255;
            }
            else {
                color[col][row][0] = 0;
                color[col][row][1] = 0;
                color[col][row][2] = 0;
            }
        }
    }
    double end_time = omp_get_wtime();
    printf("Performance: %f", end_time - start_time);
    /* Create ppm file copied from https://rosettacode.org/wiki/Mandelbrot_set#C */
    /*create new file,give it a name and open it in binary mode  */
    fp= fopen(filename,"wb"); /* b -  binary mode */
    /*write ASCII header to the file*/
    fprintf(fp,"P6\n %s\n %d\n %d\n %d\n",comment,iXmax,iYmax,MaxColorComponentValue);
    /*write image data bytes to the file*/
    for(int row=0;row<iYmax;row++) {
        for(int col=0;col<iXmax;col++) {
            /*write color to the file*/
            fwrite(color[col][row],1,3,fp);
        }
    }
    fclose(fp);
    return 0;
}