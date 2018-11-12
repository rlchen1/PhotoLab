#include <stdio.h>
#include <stdlib.h>

#include "Constants.h"
#include "FileIO.h"
#include "DIPs.h"
#include "Advanced.h"

void PrintMenu();
void AutoTest(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]);

int main() {
    unsigned char   R[WIDTH][HEIGHT];   /* use three 2-D arrays to store R, G, B components */
    unsigned char   G[WIDTH][HEIGHT];
    unsigned char   B[WIDTH][HEIGHT];

#ifdef DEBUG
    AutoTest(R, G, B);/*if debug mode is on, program will only run the AutoTest, does not print menu*/
    printf("End program.\n");
    return(0);
#endif

#ifdef NDEBUG /*if debug mode is off, program will execute normally, but messages in AutoTest will not print*/
    int option;                 /* user input option */
    char fname[SLEN];           /* input file name */
    int brightness;
    float angle;

    PrintMenu();
    printf("Please make your choice: ");
    scanf("%d", &option);

    int rc = -1;                        /* return code of LoadImage() */

    while (option != EXIT) {
        if (option == 1) {
            printf("Please input the file name to load: ");
            scanf("%s", fname);
            rc = LoadImage(fname, R, G, B);
        }
        /* menu item 2 - 10 requires image is loaded first */
        else if (option >= 2 && option <= 8) {
            if (rc != SUCCESS)   {
                printf("No image is read.\n");
            }
            /* now image is loaded */
            else {
                switch(option) {
                    case 2:
                        printf("Please input the file name to save: ");
                        scanf("%s", fname);
                        SaveImage(fname, R, G, B);
                        break;
                    case 3:
                        BlackNWhite(R, G, B);
                        printf("\"Black & White\" operation is done!\n");
                        break;
                    case 4:
                        Edge(R, G, B);
                        printf("\"Edge\" operation is done!\n");
                        break;
                    case 5:
                        Shuffle(R, G, B);
                        printf("\"Shuffle\" operation is done!\n");
                        break;
		    case 6:
                        printf("Enter a brightness value (between -255 and 255):\n"); /*ask for user input for brightness*/
                        scanf("%d", &brightness);
                        if (brightness > -255 && brightness < 255)
                        {
                                Brightness(R, G, B, brightness);
                                printf("\"Brightness\" operation is done!\n");
                                break;
                        }
                        else
                        {
                                printf("Invalid brightness value.\n");
                                break;
                        }
                    case 7:
                        HMirror(R, G, B);
                        printf("\"Mirror\" operation is done!\n");
                        break;
          	    case 8:
                        printf("Enter radian of hue rotation (between -2 and 2):\n"); /*ask for user input for radians of rotation*/
                        scanf("%f", &angle);
                        if (angle > -2 && angle < 2)
			{
				HueRotate(R, G, B, angle);
                        	printf("\"Hue Rotation\" operation is done!\n");
                       		break;
			}
			else 
			{
				printf("Invalid radian value.\n");
				break;
			}
	            default:
                        break;}
            	  }
	}
        else if (option == 9) {
            AutoTest(R, G, B);
            rc = SUCCESS;       /* set returned code SUCCESS, since image is loaded */
        }
        else {
            printf("Invalid selection!\n");
        }

        /* Process finished, waiting for another input */
        PrintMenu();
        printf("Please make your choice: ");
        scanf("%d", &option);
    }
    printf("You exit the program.\n");
    return 0;
#endif
}

/*print the menu*/
void PrintMenu() {
    printf("\n----------------------------\n");
    printf(" 1: Load a PPM image\n");
    printf(" 2: Save an image in PPM and JPEG format\n");
    printf(" 3: Change a color image to Black & White\n");
    printf(" 4: Sketch the edge of an image\n");
    printf(" 5: Shuffle an image\n");
    printf(" 6: Change brightness\n");
    printf(" 7: Reflect image\n");
    printf(" 8: Adjust hue\n");
    printf(" 9: Test all functions\n");
    printf(" 10: Exit\n");
} 

/* Test all functions */
void AutoTest(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]) {
    char fname[SLEN] = "HSSOE";
    char sname[SLEN];

    int brightness = 200;
    float angle = -1.5;

    LoadImage(fname, R, G, B);
    BlackNWhite(R, G, B);
    strcpy(sname, "bw");
    SaveImage(sname, R, G, B);
#ifdef DEBUG /*only print message in the DEBUG mode*/
    printf("Black & White tested!\n\n");
#endif

    LoadImage(fname, R, G, B);
    Edge(R, G, B);
    strcpy(sname, "edge");
    SaveImage(sname, R, G, B);
#ifdef DEBUG
    printf("Edge Detection tested!\n\n");
#endif

    LoadImage(fname, R, G, B);
    Shuffle(R, G, B);
    strcpy(sname, "shuffle");
    SaveImage(sname, R, G, B);
#ifdef DEBUG
    printf("Shuffle tested!\n\n");
#endif

    LoadImage(fname, R, G, B);
    Brightness(R, G, B, brightness);
    strcpy(sname, "brightness");
    SaveImage(sname, R, G, B);
#ifdef DEBUG
    printf("Brightness tested!\n\n");
#endif

    LoadImage(fname, R, G, B);
    HMirror(R, G, B);
    strcpy(sname, "hmirror");
    SaveImage(sname, R, G, B);
#ifdef DEBUG
    printf("Mirror tested!\n\n");
#endif

    LoadImage(fname, R, G, B);
    HueRotate(R, G, B, angle);
    strcpy(sname, "hue");
    SaveImage(sname, R, G, B);
#ifdef DEBUG
    printf("Hue Rotation tested!\n\n");
#endif
}


