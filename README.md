# PhotoLab
C program modules for pseudo-Photoshop program for manipulating an image.
The primary program file PhotoLab_v3.c utilizes a makefile to compile with the component modules consisting of header files (.h) and c files (.c) in order to produce a compilable, cohesive program. 

The final program presents users with a menu, where selecting different number options will result in different image "filters" being 
applied to a loaded image. Filters range from black and white, edge detection, shuffling, brightness, mirroring, hue, rotate/flip, resizing, and saturation. The filters utilize a data structure and pointer system, where the output image is saved to a website for viewing, accesible at http://bondi.eecs.uci.edu/~rlchen1/. 

------------------------------------------------------------------------------------------------------------------------------------------

The Advanced files contain C code for the brightness, mirror, hue, rotate/flip, resize, and saturation filters.

The DIPs files contain C code for the basic digital image processing functions: black and white, edge detection, and shuffle.

The FileIO files contain C code for managing input and output images.

The Image files contain C code for extracting the color pixels from a loaded image, and setting modified pixels into the output image. It also contains functions to create a new image structure, and free the allocated memory upon completion.

The Test files contain C code for running all of the filters at once. 
