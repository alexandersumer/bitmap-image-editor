/*
 * AUTHOR: ALEXANDER JONES U5956709
 * Completed on 30/March/2016
*/

#include "bmpedit.h"

int padding_generator (int width) {
	int padding;
  if ((width * 3) % 4 != 0) { // check if byte row length is divisible by 4
		padding = 4 - ((width * 3) % 4); // if not, then find number of padding width
	} else {
		padding = 0; // 0 padding if byte width is divisible by 4
	}
	return padding;
}

// I will mainly commend on the threshold function because remaining functions are similar

void threshold (int width, int height, int padding, char * bmp_array, float threshold_val, int tflag) {
	if (tflag == 1) {

		int black = 0, white = 255;
		int row_byte_length = width * 3 + padding;

		// the nested loops make it possible to treat the 1-dimensional array of bytes as a 2-dimensional array
		for (int i = 0; i < height; i++) { // this loop goes through the BMP image starting at the bottom row and ending at the top row
			for (int j = 0; j < row_byte_length; j++) { // this loop goes through each row of the image from bottom to top
				unsigned char B = bmp_array[i*(row_byte_length)+j]; // blue
				unsigned char G = bmp_array[i*(row_byte_length)+j++]; // green
				unsigned char R = bmp_array[i*(row_byte_length)+j++]; // red

				float avg_pixel_col = (B + G + R) / (3.0 * 255.0);

				if (avg_pixel_col < threshold_val) { // all colors below threshold_val become black and above become white
					bmp_array[i*(row_byte_length)+j]   = black; // blue
					bmp_array[i*(row_byte_length)+j-1] = black; // green
					bmp_array[i*(row_byte_length)+j-2] = black; // red
				} else {
					bmp_array[i*(row_byte_length)+j]   = white; // blue
					bmp_array[i*(row_byte_length)+j-1] = white; // green
					bmp_array[i*(row_byte_length)+j-2] = white; // red
				}
			}
		}
	} else {
		return; // this is a void function so it doesn't return anything
	}
}

// For the top and bottomg crop functions, there is no need for a nested loop.

void crop_bottom (int width, int height, int offset, int padding, char * bmp_array, long percentBottom, int bflag) {
	if (bflag == 1) {

		int row_byte_length = width * 3 + padding;
    int delRows = (percentBottom*height)/100;

    for (int i = offset; i < (delRows*(row_byte_length)+offset); i++) {
    	bmp_array[i] = 255;
    }
	}
  else {
		return;
	}
}

void crop_top (int width, int height, int filesize, int padding, char * bmp_array, long percentTop, int pflag) {
	if (pflag == 1) {

		int row_byte_length = width * 3 + padding;
		int delRows = ((100-percentTop)*height)/100;

  	for (int i = filesize; i > delRows*(row_byte_length); i--) {
  		bmp_array[i] = 255;
  	}
	} else {
		return;
	}
}

void crop_right (int width, int height, int padding, char * bmp_array, long percentRight, int rflag) {
	if (rflag == 1) {

		int row_byte_length = width * 3 + padding;
    int delColumns = ((100-percentRight)*(row_byte_length))/100;

    for (int i = 0; i < height; i++) {
      for (int j = (row_byte_length); j > delColumns; j--) {
        bmp_array[i*(row_byte_length)+j] = 255;
      }
    }
	} else {
		return;
	}
}

void crop_left (int width, int height, int padding, char * bmp_array, long percentLeft, int lflag) {
	if (lflag == 1) {

		int row_byte_length = width * 3 + padding;
    int delColumns = (percentLeft*(row_byte_length))/100;

    for (int i = 0; i < height; i++) {
      for (int j = 0; j < delColumns; j++) {
        bmp_array[i*(row_byte_length)+j] = 255;
      }
    }
	} else {
		return;
	}
}

void grayscale (int width, int height, int padding, char * bmp_array, int gflag) {
	if (gflag == 1) {

		int row_byte_length = width * 3 + padding;

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < row_byte_length; j++) {
				unsigned char B = bmp_array[i*(row_byte_length)+j];
				unsigned char G = bmp_array[i*(row_byte_length)+j++];
				unsigned char R = bmp_array[i*(row_byte_length)+j++];

				// grayscale formula reference: http://www.tannerhelland.com/3643/grayscale-image-algorithm-vb6/
				// This formula is called the BT.601 recommendation

				bmp_array[i*(row_byte_length)+j]   = (B * 0.114 + G * 0.587 + R * 0.299); // blue
				bmp_array[i*(row_byte_length)+j-1] = (B * 0.114 + G * 0.587 + R * 0.299); // green
				bmp_array[i*(row_byte_length)+j-2] = (B * 0.114 + G * 0.587 + R * 0.299); // red
			}
		}
	} else {
		return;
	}
}

void negate (int width, int height, int padding, char * bmp_array, int nflag) {
	if (nflag == 1) {

		int row_byte_length = width * 3 + padding;

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < row_byte_length; j++) {
				bmp_array[i*(row_byte_length)+j]   = 255 - bmp_array[i*(row_byte_length)+j];   // blue
				bmp_array[i*(row_byte_length)+j-1] = 255 - bmp_array[i*(row_byte_length)+j-1]; // green
			  bmp_array[i*(row_byte_length)+j-2] = 255 - bmp_array[i*(row_byte_length)+j-2]; // red
			}
		}
	} else {
		return;
	}
}

// usage function is based on the sample usage message in the assignment specification by Eric McCreath

static void usage(void) {
  printf
	(
    "\n"
    "  Usage: bmpedit [OPTIONS...] [input.bmp]\n"
    "\n"
    "  DESCRIPTION:\n"
    "\n"
    "  This program does simple edits of BMP image files. When the program runs \n"
		"  it first prints out the width and the height of the input image within the BMP file.\n"
		"  Once this is done if there is a filter (or sequence of filters) then they are applied\n"
		"  to the image.  The resulting image is also stored using BMP format into an output file.\n"
		"  Without any filters only the width and height of the image is output.\n"
    "  It performs the following edits:\n"
    "\n"
    "  crop bottom, top, right and left by percentage\n"
    "  grayscale\n"
    "  negate\n"
    "  threshold\n"
    "\n"
    "  OPTIONS:\n"
		"\n"
		"  -o FILE             Sets the output file for modified images. Default output file is out.bmp\n"
		"\n"
		"  -t 0.0-1.0          Applies a threshold to the input image.\n"
		"\n"
		"  -h                  Displays this usage message\n"
    "\n"
    "  -b 0-100            Crops the bottom of the image by whitening rows of pixels by percentage of the height.\n"
		"\n"
		"  -p 0-100            Crops the top of the image by whitening rows of pixels by percentage of the height.\n"
		"\n"
		"  -r 0-100            Crops the right of the image by whitening columns of pixels by percentage of the width.\n"
		"\n"
		"  -b 0-100            Crops the left of the image by whitening columns of pixels by percentage of the width.\n"
    "\n"
		"    The above crop functions can be combined to crop the image from all 4 sides.\n"
		"    For example, to crop the image by 10%% from 4 sides, enter the following command: \n"
		"    ./bmpedit -b 10 -p 10 -r 10 -l 10 -o out.bmp cupad.bmp\n"
		"\n"
    "  -g:                 Applies grayscale to the input BMP.\n"
		"\n"
		"  -n                  Applies a negative effect to the output image\n"
		"\n"
	);
}
