#include <stdlib.h>
#include <stdio.h>

/*
 * Main C Function
 * Arguments: ppmrw, format (3 or 6), inputfile, outputfile
 */
int main(int argc, char *argv[]) {
  if (argc != 4) {
    printf("usage: ppmrw format input.ppm output.ppm \n");
    return(1);
  }

  return 0;
}
