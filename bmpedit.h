/*
 * AUTHOR: ALEXANDER JONES U5956709
 * Completed on 30/March/2016
*/

// reference: https://en.wikipedia.org/wiki/BMP_file_format
#define HEADERSIZE 0x36 // size of header in bytes
#define WIDTHPOS 0x12   // position of the start of the 4 bytes that contain width information in header
#define HEIGHTPOS 0x16  // position of the start of the 4 bytes that contain height information in header
#define OFFSETPOS 0xa   // position of the start of the 4 bytes that contain offset information in header
#define SIZEPOS 0x02    // position of the start of the 4 bytes that contain file size information in header
#define IDPOS 0x00      // position of the start of the 2 bytes that contain ID information in header
#define DEFAULTOUTPUT "out.bmp" // default output file

int padding_generator (int width);

// I will mainly commend on the threshold function because remaining functions are similar

void threshold (int width, int height, int padding, char * bmp_array, float threshold_val, int tflag);

// For the top and bottomg crop functions, there is no need for a nested loop.

void crop_bottom (int width, int height, int offset, int padding, char * bmp_array, long percentBottom, int bflag);

void crop_top (int width, int height, int filesize, int padding, char * bmp_array, long percentTop, int pflag);

void crop_right (int width, int height, int padding, char * bmp_array, long percentRight, int rflag);

void crop_left (int width, int height, int padding, char * bmp_array, long percentLeft, int lflag);

void grayscale (int width, int height, int padding, char * bmp_array, int gflag);

void negate (int width, int height, int padding, char * bmp_array, int nflag);

// usage function is based on the sample usage message in the assignment specification by Eric McCreath

static void usage(void);