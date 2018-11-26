/*********************************************************************/
/* PhotoLab.c: homework assignment #3, for EECS 22,  Fall 2017       */
/* Modified: Mihnea Chirila for F18, 10/30/18                                                          */
/* Author: Mina Moghadam                                                */
/* Date:   11/8/2017                                                */
/*                                                                   */
/* Comments: */
/*                                                                   */
/* Please use this template to submit your hw3.                      */
/*********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "DIPs.h"
#include "Advanced.h"
#include "FileIO.h"
#include "Constants.h"
#include "Test.h"
#include "Image.h"

/*** function declarations ***/

/* print a menu */
void PrintMenu();

/* Test all functions */
int AutoTest(void);

int main()
{
#ifdef DEBUG
	AutoTest();

#else
	IMAGE *image = NULL;
	int option;			/* user input option */
        char fname[SLEN];		/* input file name */
	int brightness = -256;
	float angle, percent;
	int percentage = 0;

	PrintMenu();
        printf("Please make your choice: ");
        scanf("%d", &option);

	while (option != EXIT) {
		if (option == 1) {  /*if image existing, clear the image*/
		    if(image != NULL)
			{
			DeleteImage(image);
			}
            printf("Please input the file name to load: ");
            scanf("%s", fname);
            image = LoadImage(fname); 
		}

		/* menu item 2 - 11 requires image is loaded first */
        else if (option >= 2 && option <= 11) {
            if (image == NULL)	 {
                printf("No image is read.\n");
            }
            /* now image is loaded */
            else {
		switch (option) {
	  case 2:
	    printf("Please input the file name to save: ");
	    scanf("%s",fname);
	    SaveImage(fname,image);
	    break;
	  case 3:
            image = BlackNWhite(image);
            printf("\"Black & White\" operation is done!\n");
            break;
	  case 4:
            image = Edge(image);
            printf("\"Edge\" operation is done!\n");
            break;
          case 5:
            image = Shuffle(image);
            printf("\"Shuffle\" operation is done!\n");
            break;
	  case 6:
	    while(brightness > 255 || brightness < -255){
		printf("Enter brightness value (between -255 and 255):");
		scanf("%d", &brightness);}
            image = Brightness(image, brightness);
	    brightness = -256; /*reset the brightness value after running once*/
            printf("\"brightness\" operation is done!\n");
            break;
	  case 7:
            image = HMirror(image);
            printf("\"Horizontally Mirror\" operation is done!\n");
            break;
          case 8:
            printf("Enter hue rotation angle:");
            scanf("%f", &angle);
            image = HueRotate(image, angle);
            printf("\"HueRotate\" operation is done!\n");
            break;
	  case 9:
	    while(option > 4 || option < 1){
	        printf("Select option:\n1 - Horizontal flip\n2 - Vertical flip\n3 - Rotate right\n4 - Rotate left\n");
	        scanf("%d", &option);}	   
	    image = Rotate(image, option);
	    option = 0; /*reset the option value after running once*/
	    printf("\"Rotate\" operation is done!\n");
	    break;
	  case 10:
	    while(percentage > 199 || percentage < 1){
	        printf("Enter resize percentage (1 to 199):\n");
	        scanf("%d", &percentage);}
	    image = Resize(image, percentage);
	    percentage = 0; /*reset the percentage value after running once*/
	    printf("\"Resize\" operation is done!\n");
	    break;
	  case 11: 
	    printf("Enter saturation percentage:\n");
	    scanf("%f", &percent);
	    image = Saturate(image, percent);
	    printf("\"Saturate\" operation is done!\n");
	    break;
	  default:
	    break;}
			}
		}
	else if (option == 12) {
            AutoTest();
        }
        else {
            printf("Invalid selection!\n");
        }

		/* Process finished, waiting for another input */
        PrintMenu();
        printf("Please make your choice: ");
        scanf("%d", &option);
	}
	
	if (option == 13)
	{
		if (image != NULL)
		{
			DeleteImage(image); /*upon exiting the program, free memory allocations*/
		}
	}
    printf("You exit the program.\n");
#endif
    return 0;
}


/*******************************************/
/* Function implementations should go here */
/*******************************************/

/* Menu */
void PrintMenu() {
    printf("\n----------------------------\n");
    printf(" 1: Load a PPM image\n");
    printf(" 2: Save an image in PPM and JPEG format\n");
    printf(" 3: Change a color image to Black & White\n");
    printf(" 4: Sketch the edge of an image\n");
    printf(" 5: Shuffle an image\n");
    printf(" 6: Adjust the brightness of an image\n");
    printf(" 7: Mirror an image horizontally\n");
    printf(" 8: Adjust the hue of an image\n");
    printf(" 9: Rotate or flip the image\n");
    printf("10: Resize the image\n");
    printf("11: Saturate the image\n");
    printf("12: Test all functions\n");
    printf("13: Exit\n");
}
/* vim: set tabstop=8 softtabstop=8 shiftwidth=8 noexpandtab : */
