/*
 * AUTHOR: ALEXANDER JONES U5956709
 * Completed on 30/March/2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "bmpedit.h" // bmpedit.h contains preprocessor instructions and functions

int main (int argc, char **argv) {
	char header[HEADERSIZE]; // array header which will contain the header infor of the BMP file.
													 // Windows BMP headers are 0x36 bytes so we allocate that much
													 // using a macro constant.
  const char * outputBMP = NULL; // set pointer to NULL
	outputBMP = DEFAULTOUTPUT; // set default output file to "out.bmp"

	double threshold_val;
  long percentBottom, percentTop, percentRight, percentLeft; // must use long to use strtol() instead of atoi()

	int bflag = 0; int pflag = 0; int rflag = 0; int lflag = 0;

	int tflag = 0;
	int gflag = 0;
	int nflag = 0;

	char * endptr = NULL; //required for strtol and strtod
	int opt = 0;
	// reference: http://www.gnu.org/software/libc/manual/html_node/Getopt.html
  while ( (opt = getopt (argc, argv, "b:p:r:l:t:o:gnh")) != -1 ) {
    switch (opt) {
      case 'b':
        percentBottom = strtol (optarg, &endptr, 10);
        if (percentBottom < 0 || percentBottom > 100) {
          error(EXIT_FAILURE, 0, "Error: 0 <= percentage <= 100");
        }
        bflag = 1;
        break;
      case 'p':
        percentTop = strtol (optarg, &endptr, 10);
        if (percentTop < 0 || percentTop > 100) {
          error(EXIT_FAILURE, 0, "Error: 0 <= percentage <= 100");
        }
        pflag = 1;
        break;
      case 'r':
        percentRight = strtol (optarg, &endptr, 10);
        if (percentRight < 0 || percentRight > 100) {
          error(EXIT_FAILURE, 0, "Error: 0 <= percentage <= 100");
        }
        rflag = 1;
        break;
      case 'l':
        percentLeft = strtol (optarg, &endptr, 10);
        if (percentLeft < 0 || percentLeft > 100) {
          error(EXIT_FAILURE, 0, "Error: 0 <= percentage <= 100");
        }
        lflag = 1;
        break;
			case 't':
				threshold_val = strtod (optarg, &endptr);
				if (threshold_val < 0.0 || threshold_val > 1.0) {
					error(EXIT_FAILURE, 0, "Error: 0.0 <= threshold value <= 1.0");
				}
				tflag = 1;
				break;
			case 'g':
				gflag = 1;
				break;
			case 'n':
				nflag = 1;
				break;
      case 'o':
        outputBMP = optarg;
        break;
      case 'h':
        usage();
        return EXIT_SUCCESS;
      default:
        usage();
        return EXIT_FAILURE;
    }
		endptr = NULL;
  }

	if (argc - optind != 1) {
		error(0, 0, "Must specify an input file");
		usage();
		return EXIT_FAILURE;
  }

	const char * inputBMP = NULL; // set pointers to NULL to avoid memory problems
	inputBMP = * (argv + optind); // optind holds the index value of the next non-option command line argument.

	FILE * file1 = NULL; // create file pointer and set it to NULL
	file1 = fopen(inputBMP, "r");
	fread (header, sizeof(char), HEADERSIZE, file1); // read the input BMP file header and extract its bytes into an array

	if (!inputBMP) {
    error(EXIT_FAILURE, errno, "Error opening input file");
  }

	int width    = * (int *) &header[WIDTHPOS];  // There are two reason as to why this works
	int height   = * (int *) &header[HEIGHTPOS]; // 1. most computers are little endian
	int filesize = * (int *) &header[SIZEPOS];   // 2. type int has 4 bytes (32 bits) and width, height
	int offset   = * (int *) &header[OFFSETPOS]; //    filesize and offset are stored in 4 bytes in the header

	short bmpid  = * (short *) &header[IDPOS]; // short is used here because id is stored in the first 2 bytes

	int padding  = padding_generator(width); // calculate padding using the padding function

	if (bmpid != 0x4D42) { // input BMP must be a valid windows BMP, this program does not working for other formats
		fclose(file1);
		perror("Not a valid BMP");
	}

	printf("Image width: %d\nImage height: %d\n", width, height);

	int pixel_array_size = (height * (width * 3 + padding));

	unsigned char * pixel_array = NULL; // store pixel data in pixel_array
	pixel_array = (unsigned char*) malloc (pixel_array_size);
	fseek (file1, offset, SEEK_SET); // move file point to the begging of the input BMP file
	fread (pixel_array, sizeof(char), pixel_array_size, file1); // create image pixel array

	threshold   (width, height, padding, pixel_array, threshold_val, tflag);

  crop_bottom (width, height, offset, padding, pixel_array, percentBottom, bflag);
  crop_top    (width, height, filesize, padding, pixel_array, percentTop, pflag);
  crop_right  (width, height, padding, pixel_array, percentRight, rflag);
  crop_left   (width, height, padding, pixel_array, percentLeft, lflag);

  grayscale   (width, height, padding, pixel_array, gflag);
	negate      (width, height, padding, pixel_array, nflag);

	FILE * file2 = NULL;
	file2 = fopen(outputBMP, "w"); // create file pointer file2 for the output BMP
	fwrite (header, sizeof(char), HEADERSIZE, file2); // First, write the header of the output BMP
	fwrite (pixel_array, sizeof(char), pixel_array_size, file2); // Write the

	// I commented out both fclose and free becuse I get the error "double free or corruption (out)"
	// when I run my program on BMP files other than cup.bmp.
	// This problem is caused by one of two things:
	//  1. something is being deleted twice
  //  2. something that wasn't allocated is being deleted.
	// fclose (file2); // close file2
	// free (pixel_array); // releases the memory allocated to pixel_array back to the allocator, so the memory can be used again.
}
