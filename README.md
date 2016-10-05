# Assignnment 1

## Name and ID:

* Name : ALEXANDER JONES
* ID : U5956709

## Summary of programs:

bmpedit is a simple program that applies simple edits to Windows BMP images.

bmpedit.c uses getopt to get command line arguments from user and apply the requested edits to the input image.

bmpdata.c contains a directive to bmpdata.h and the main function only.
bmpdata.h contains preprocessor information such as directives and macros as well as all the functions.

There are 8 functions in bmpedit.

1. threshold: takes an argument between 0.0 and 1.0 and changes all the colours less than threshold value to black greater than to white.

2. cropBottom: crops the bottom of the BMP image by percentage of height

3. cropTop: crops the top of the BMP image by percentage of height

4. cropRight: crops the right of the BMP image by percentage of width

5. cropLeft: crops the left of the BMP image by percentage of width

6. grayscale: uses the BT.601 formula to apply a grayscale effect to the bmp image

7. negate: applies a negative effect to the bmp image

8. usage: displays the usage message

## Compiling and running:

To compile bmpedit.c either use the MakeFile by typing `make` into terminal or
type `gcc -o bmpedit bmpedit.c` into terminal.

Once bmpedit compiles, you can run the program using command line arguments.
Here are some examples of command line arguments that should work:

* ./bmpedit -h

print usage message

* ./bmpedit -t 0.5 -o threshold.bmp cup.bmp

Apply a threshold of 0.5 to cup.bmp and put the output in threshold.bmp

* ./bmpedit -b 10 -p 10 -right 10 -left 10 -g output.bmp cup.bmp

Crop cup.bmp by 10% from all 4 sides and apply a grayscale to it and put the output in output.bmp

## Testing:

All testing was done on an Ubuntu 15.10 64-bit using gcc in terminal.

I added a 1 pixel border around cup.bmp using Microsoft Paint and called it cupad.bmp.
This meant that the width of cupad.bmp is 642 so cupad.bmp has 2 bytes of padding.
I tested my programs on cupad.bmp and they work in exactly the same way as cup.bmp.

I also created 2 simple bmp files for testing.
The first has dimension 64x64 and consists of 4 16x16 squares colored green, red, yellow and blue.
In this bmp file, the width is divisible by 4 so there is 0 bytes of padding.

The first has dimension 66x66 and consists of 4 16x16 squares colored green, red, yellow and blue.
The extra 2 pixels are created by drawing a 1 pixel border around the image.
In this bmp file, the width is not divisible by 4 so there is 2 bytes of padding.

I also downloaded sample bmp images and tested my programs using them.
Some of these images had padding and some didn't have padding. Some of them were not the correct format.
This meant that I could see my program's behaviour in different situations and improve error handling.

## Extensions:

I made 3 extensions:

* crop

To crop an image from 4 sides (top, bottom, right, left), I am using 4 functions.
These functions crop using percentage of height or width.

* grayscale

Applies a grayscale filter to the image

* negate

Applies a negative effect to the image

## Limitations:

There are many limitations to bmpedit.
bmpedit is a very simple program.

It only works on Windows Bitmap files (start with 0x4D42) and is limited to 24-bit colour.
bmpedit would not work on any other file format.
It also does not work on compressed Windows Bitmap files.

The user interface is not easy to use. Users have to use terminal and enter command line arguments to specify what edits they want.

getopt is used so only one argument can be specified per option.

Crop functions do not actually delete pixels but they only turn the cropped area white.
Removing pixels is much more complicated because it involves changing the header information, which introduces many challenges.
Header information must be perfect. Any error leads to a corrupt file.
