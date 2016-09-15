#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Structure to hold RGB pixel data
typedef struct RGBpixel {
  unsigned char R, G, B;
} RGBpixel;

// Global variables to hold image metadata
int w;
int h;
int maxColor;
int numPixels;
char format[2];

/*
 * Main C Function
 * Arguments: ppmrw, format (3 or 6), inputfile, outputfile
 */
int main(int argc, char *argv[]) {

  if (argc != 4) {
    fprintf(stderr, "Error: usage: ppmrw format input.ppm output.ppm\n");
    return(1);
  }

  FILE* fh = fopen(argv[2], "r");
  if (!fh) {
    fprintf(stderr, "Error: Unable to open file.\n");
    return(1);
  }

  // Get the image metadata
  fscanf(fh, "%s %i %i %i", format, &w, &h, &maxColor);
  numPixels = w * h;

  /*
  printf("Format: %s\n", format);
  printf("Width: %i\n", w);
  printf("Height: %i\n", h);
  printf("Max Color: %i\n", maxColor);
  printf("Total Pixels: %i\n", numPixels);
  */

  if (!strcmp(format, "P3")) {
    // READ IN P3 IMAGE DATA
  }
  else if (!strcmp(format, "P6")) {
    // READ IN P6 IMAGE DATA
  }
  else {
    fprintf(stderr, "Error: Unrecognized image format.\n");
    return(1);
  }

  RGBpixel pixmap[numPixels];

  int r, g, b;
  for (int i = 0; i < numPixels; i++) {
    if ((fscanf(fh, "%i", &r) != EOF) &&
        (fscanf(fh, "%i", &g) != EOF) &&
        (fscanf(fh, "%i", &b) != EOF)) {
          pixmap[i].R = r;
          pixmap[i].G = g;
          pixmap[i].B = b;
    }
    else {
      // Hit the end of the file before finished reading all of the pixels
      fprintf(stderr, "Error: Image data cannot fill 8 bits per channel.\n");
      return(1);
    }
  }

  int extra;
  if (fscanf(fh, "%i", &extra) != EOF) {
    // Finished reading the file, but there is still data left over
    fprintf(stderr, "Error: Image data exceeds 8 bits per channel.\n");
    return(1);
  }

  fclose(fh);
  return(0);
}
