#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Prototype declarations
int readP3(FILE* fh);
int readP6(FILE* fh);
int writeP3(FILE* fh);
int writeP6(FILE* fh);

// Structure to hold RGB pixel data
typedef struct RGBpixel {
  unsigned char R, G, B;
} RGBpixel;

// Global variables to hold image data
int w;
int h;
int maxColor;
int numPixels;
char format[2];
RGBpixel *pixmap;


int readP3 (FILE* fh) {
  // Fill the pixel map up with the image data
  int r, g, b;
  for (int i = 0; i < numPixels; i++) {
    if ((fscanf(fh, "%i", &r) != EOF) &&
        (fscanf(fh, "%i", &g) != EOF) &&
        (fscanf(fh, "%i", &b) != EOF)) {
      if (r > maxColor || g > maxColor || b > maxColor) {
        fprintf(stderr, "Error: Image data is not 8 bits per channel.\n");
        return(0);
      }
      pixmap[i].R = r;
      pixmap[i].G = g;
      pixmap[i].B = b;
    }
    else {
      // Hit the end of the file before finished reading all of the pixels
      fprintf(stderr, "Error: Image data cannot fill 8 bits per channel.\n");
      return(0);
    }
  }

  int extra;
  if (fscanf(fh, "%i", &extra) != EOF) {
    // Finished reading the file, but there is still data left over
    fprintf(stderr, "Error: Image data exceeds 8 bits per channel.\n");
    return(0);
  }
  return(1);
}


int readP6 (FILE* fh) {

  unsigned char pixel[3];

  for (int i = 0; i < numPixels; i++) {
    if (fread(pixel, sizeof(unsigned char), 3, fh) != 3) {
      fprintf(stderr, "Error: Image data is not 8 bits per channel.\n");
      return(0);
    }
    else {
      pixmap[i].R = pixel[0];
      pixmap[i].G = pixel[1];
      pixmap[i].B = pixel[2];
    }
  }

  return(1);
}


int writeP3(FILE* fh) {
  for (int i = 0; i < numPixels; i++) {
    fprintf(fh, "%i %i %i\n", pixmap[i].R, pixmap[i].G, pixmap[i].B);
  }
  return(1);
}


int writeP6(FILE* fh) {
  int pixelsWritten = fwrite(pixmap, sizeof(RGBpixel), numPixels, fh);
  if (pixelsWritten != numPixels) {
    fprintf(stderr, "Error: Could not write all of the image data.\n");
    return(0);
  }
  return(1);
}


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
  fscanf(fh, "%s %i %i %i\n", format, &w, &h, &maxColor);
  if (maxColor > 255) {
    fprintf(stderr, "Error: Image data is not 8 bits per channel.\n");
    return(1);
  }
  numPixels = w * h;

  // Allocate memory for the pixel map
  pixmap = malloc(sizeof(RGBpixel) * numPixels);

  // Read image data into memory based on format
  if (!strcmp(format, "P3")) {
    if (!readP3(fh)) {
      return(1);
    }
  }
  else if (!strcmp(format, "P6")) {
    if (!readP6(fh)) {
      return(1);
    }
  }
  else {
    fprintf(stderr, "Error: Unrecognized image format.\n");
    return(1);
  }

  // Finished reading image data, time to write it out
  FILE* outFile = fopen(argv[3], "w");
  char* writingFormat = argv[1];
  fprintf(outFile, "P%s\n%i %i\n%i\n", writingFormat, w, h, maxColor);
  if (!strcmp(writingFormat, "3")) {
    if (!writeP3(outFile)) {
      return(1);
    }
  }
  else if (!strcmp(writingFormat, "6")) {
    if (!writeP6(outFile)) {
      return(1);
    }
  }
  else {
    fprintf(stderr, "Error: Unrecognized image format.\n");
    return(1);
  }

  // Close the file and return success
  fclose(fh);
  return(0);
}
