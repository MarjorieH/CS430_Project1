#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Prototype declarations
void readP3(FILE* fh);
void readP6(FILE* fh);
void writeP3(FILE* fh);
void writeP6(FILE* fh);

// Structure to hold RGB pixel data
typedef struct RGBpixel {
  unsigned char R, G, B;
} RGBpixel;

// Global variables to hold image data
int w; // image width in pixels
int h; // image height in pixels
int maxColor; // maximum numeric color value for the image
int numPixels; // total number of pixels in image (w * h)
char format[64]; // format of the image, supports P3 or P6
RGBpixel *pixmap; // array of pixels to hold the image data


/* Reads in P3 formatted data into the pixmap
 * Takes in the file handler of the file to be read in, beginning at the image data */
void readP3 (FILE* fh) {
  // Fill the pixel map up with the image data
  int r, g, b;
  for (int i = 0; i < numPixels; i++) {
    if ((fscanf(fh, "%i", &r) != EOF) &&
        (fscanf(fh, "%i", &g) != EOF) &&
        (fscanf(fh, "%i", &b) != EOF)) {
      if (r > maxColor || g > maxColor || b > maxColor) {
        fprintf(stderr, "Error: Image data is not 8 bits per channel.\n");
        exit(1);
      }
      pixmap[i].R = r;
      pixmap[i].G = g;
      pixmap[i].B = b;
    }
    else {
      // Hit the end of the file before finished reading all of the pixels
      fprintf(stderr, "Error: Image data cannot fill 8 bits per channel.\n");
      exit(1);
    }
  }

  int extra;
  if (fscanf(fh, "%i", &extra) != EOF) {
    // Finished reading the file, but there is still data left over
    fprintf(stderr, "Error: Image data exceeds 8 bits per channel.\n");
    exit(1);
  }
}


/* Reads in P6 formatted data into the pixmap
 * Takes in the file handler of the file to be read in, beginning at the image data */
void readP6 (FILE* fh) {

  unsigned char pixel[3];

  for (int i = 0; i < numPixels; i++) {
    if (fread(pixel, sizeof(unsigned char), 3, fh) != 3) {
      fprintf(stderr, "Error: Image data is not 8 bits per channel.\n");
      exit(1);
    }
    else {
      pixmap[i].R = pixel[0];
      pixmap[i].G = pixel[1];
      pixmap[i].B = pixel[2];
    }
  }
  int extra;
  if (fscanf(fh, "%i", &extra) != EOF) {
    // Finished reading the file, but there is still data left over
    fprintf(stderr, "Error: Image data exceeds 8 bits per channel.\n");
    exit(1);
  }
}


/* Writes P3 formatted data to a file
 * Takes in the file handler of the file to be written to */
void writeP3(FILE* fh) {
  for (int i = 0; i < numPixels; i++) {
    fprintf(fh, "%i %i %i\n", pixmap[i].R, pixmap[i].G, pixmap[i].B);
  }
  fclose(fh);
}


/* Writes P6 formatted data to a file
 * Takes in the file handler of the file to be written to */
void writeP6(FILE* fh) {
  int pixelsWritten = fwrite(pixmap, sizeof(RGBpixel), numPixels, fh);
  if (pixelsWritten != numPixels) {
    fprintf(stderr, "Error: Could not write all of the image data.\n");
    exit(1);
  }
  fclose(fh);
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

  // Get the image metadata from the header, making sure to skip over comments
  int headerCount = 0;
  char buffer[64];
  while (headerCount < 4) {
    if (!fscanf(fh, "%s", buffer)) { // scan in a value
      fprintf(stderr, "Error: Insufficient image data.\n");
      return(1);
    }
    if (!strcmp("#", buffer)) { // check if we just scanned in a comment
      int c = fgetc(fh);
      while (c != 10) { // skip the line since it is a comment
        c = fgetc(fh);
      }
    }
    else { // otherwise, save the line based on where we are in the header
      if (headerCount == 0) { // save format
        strcpy(format, buffer);
      }
      else if (headerCount == 1) { // save width
        w = atoi(buffer);
      }
      else if (headerCount == 2) { // save height
        h = atoi(buffer);
      }
      else { // save maximum color value
        maxColor = atoi(buffer);
      }
      headerCount++;
    }
  }
  // make sure that the file handler is pointing to the
  // start of the image data before beginning to read it in
  fscanf(fh, "\n");

  if (maxColor > 255) {
    fprintf(stderr, "Error: Image data is not 8 bits per channel.\n");
    return(1);
  }
  numPixels = w * h;

  // Allocate memory for the pixel map
  pixmap = malloc(sizeof(RGBpixel) * numPixels);

  // Read image data into memory based on format
  if (!strcmp(format, "P3")) { // reading P3 image data
    readP3(fh);
  }
  else if (!strcmp(format, "P6")) { // reading P6 image data
    readP6(fh);
  }
  else {
    fprintf(stderr, "Error: Unrecognized image format.\n");
    return(1);
  }

  // Finished reading image data, time to write it out
  FILE* outFile = fopen(argv[3], "w");
  char* writingFormat = argv[1];
  fprintf(outFile, "P%s\n%i %i\n%i\n", writingFormat, w, h, maxColor);
  if (!strcmp(writingFormat, "3")) { // writing P3 image data
    writeP3(outFile);
  }
  else if (!strcmp(writingFormat, "6")) { // writing P6 image data
    writeP6(outFile);
  }
  else {
    fprintf(stderr, "Error: Unrecognized image format.\n");
    return(1);
  }

  // Close the file and return success
  fclose(fh);
  return(0);
}
