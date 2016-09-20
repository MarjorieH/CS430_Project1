# CS430_Project1
### CS430: Computer Graphics
### Project 1: Images
### Marjorie Hahn
### 16 September 2016

This application takes in either a P3 or P6 formatted image and outputs the
image data to a file as a P3 or P6 image, depending on what the user selects.

This program is capable of converting:
* P3 -> P6
* P3 -> P3
* P6 -> P3
* P6 -> P6

Usage: ppmrw format inputfile.ppm outputfile.ppm

Where "format" is the desired format for the output image data (3 or 6).

In order to run the program, after you have downloaded the files off of Github,
make sure that you are sitting in the directory that holds all of the files and
run the command "make all". Then you will be able to run the program using the
usage command mentioned above. There are two test images included that you can
use to test the functionality of the program: testimageP3.ppm and testimageP6.ppm.
