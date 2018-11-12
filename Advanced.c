#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "Advanced.h"

/*adjust the brightness of image*/
void Brightness(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], int brightness)
{
        int r, c;
        int tmpR[WIDTH][HEIGHT];
        int tmpG[WIDTH][HEIGHT];
        int tmpB[WIDTH][HEIGHT];


        for (r = 0; r < WIDTH; r++) /*adds the inputted brightness value to the current image RBG values*/
        {
                for (c = 0; c < HEIGHT; c++)
                {
                        tmpR[r][c] = (R[r][c] + brightness);
                        if (tmpR[r][c] > 255)
                        {
                                tmpR[r][c] = 255;
                        }
                        else if (tmpR[r][c] < -255)
                        {
                                tmpR[r][c] = 0;
                        }

                        tmpG[r][c] = (G[r][c] + brightness);
                        if (tmpG[r][c] > 255)
                        {
                                tmpG[r][c] = 255;
                        }
                        else if (tmpG[r][c] < -255)
                        {
                                tmpG[r][c] = 0;
                        }

                        tmpB[r][c] = (B[r][c] + brightness);
                        if (tmpB[r][c] > 255)
                        {
                                tmpB[r][c] = 255;
                        }
                        else if (tmpB[r][c] < -255)
                        {
                                tmpB[r][c] = 0;
                        }
                }
        }
	for (r = 0; r < WIDTH; r++) /*moves the RGB values from temporary array back to original RGB array*/
        {
                for (c = 0; c < HEIGHT; c++)
                {
                        R[r][c] = tmpR[r][c];
                        G[r][c] = tmpG[r][c];
                        B[r][c] = tmpB[r][c];
                }
        }
}

/*reflects the left half of the image across to the right side*/
void HMirror(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
        unsigned char tmpR[WIDTH][HEIGHT];
        unsigned char tmpG[WIDTH][HEIGHT];
        unsigned char tmpB[WIDTH][HEIGHT];

        int r, c;

        for (r = 0; r < WIDTH/2; r++) /*copies the left half image into the left half of a temporary array*/
        {
                for (c = 0; c < HEIGHT; c++)
                {
                        tmpR[r][c] = R[r][c];
                        tmpG[r][c] = G[r][c];
                        tmpB[r][c] = B[r][c];
                }
        }

        for (r = 300; r > 0; r--) /*copies the left half image into the right half of temporary array in reverse order horizontally to produce mirrored image*/
        {
                for(c = 0; c < HEIGHT; c++)
                {
                        tmpR[WIDTH-r][c] = R[r][c];
                        tmpG[WIDTH-r][c] = G[r][c];
                        tmpB[WIDTH-r][c] = B[r][c];
                }
        }

	for (r = 0; r < WIDTH; r++) /*moves the modified mirrored image from temporary array back into the original RGB array*/
        {
                for (c = 0; c < HEIGHT; c++)
                {
                        R[r][c] = tmpR[r][c];
                        G[r][c] = tmpG[r][c];
                        B[r][c] = tmpB[r][c];
                }
        }
}

void HueRotate(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], float angle)
{
        #define pi 3.14159

        float d = cos(angle*pi); /*convert to radians for trig calculation*/
        float e = sin(angle*pi);

        float tmpR[WIDTH][HEIGHT]; /*temporary arrays of type double to store RGB value between 0 and 1*/
        float tmpG[WIDTH][HEIGHT];
        float tmpB[WIDTH][HEIGHT];

        float Y[WIDTH][HEIGHT]; /*polar arrays*/
        float I[WIDTH][HEIGHT];
        float Q[WIDTH][HEIGHT];

        float tmpY[WIDTH][HEIGHT]; /*adjusted polar arrays*/
        float tmpI[WIDTH][HEIGHT];
        float tmpQ[WIDTH][HEIGHT];

        int r, c;

        for (r = 0; r < WIDTH; r++) /*convert the RGB values to value between 0-1 by dividing 255 for each pixel*/
        {
                for (c = 0; c < HEIGHT; c++)
                {
                        tmpR[r][c] = (R[r][c]/255.0);
                        tmpG[r][c] = (G[r][c]/255.0);
                        tmpB[r][c] = (B[r][c]/255.0);
                }
        }

 	for (r = 0; r < WIDTH; r++) /*convert the 0-1 RGB array into YIQ array with new values*/
        {
                for (c = 0; c < HEIGHT; c++)
                {
                        Y[r][c] = (tmpR[r][c] * 0.299) + (tmpG[r][c] * 0.587) + (tmpB[r][c] * 0.114);
                        I[r][c] = (tmpR[r][c] * 0.596) - (tmpG[r][c] * 0.274) - (tmpB[r][c] * 0.321);
                        Q[r][c] = (tmpR[r][c] * 0.211) - (tmpG[r][c] * 0.523) + (tmpB[r][c] * 0.311);
                }
        }

        for (r =0; r < WIDTH; r++) /*add the inputted radian to a temp YIQ array to prevent contamination of Q*/
        {
                for (c = 0; c < HEIGHT; c++)
                {
                        tmpY[r][c] = Y[r][c];
                        tmpI[r][c] = (I[r][c] * d) - (Q[r][c] * e);
                        tmpQ[r][c] = (I[r][c] * e) + (Q[r][c] * d);
                }
        }

        for (r = 0; r < WIDTH; r++) /*change temporary YIQ with added radian back to RGB 0-1*/
        {
                for (c = 0; c < HEIGHT; c++)
                {
                        tmpR[r][c] = tmpY[r][c] + (tmpI[r][c] * 0.956) + (tmpQ[r][c] * 0.621);
                        tmpG[r][c] = tmpY[r][c] - (tmpI[r][c] * 0.272) - (tmpQ[r][c] * 0.647);
                        tmpB[r][c] = tmpY[r][c] - (tmpI[r][c] * 1.107) + (tmpQ[r][c] * 1.705);

                        tmpR[r][c] = tmpR[r][c] * 255; /*change temp RGB from 0-1 back to 0-255*/
                        tmpG[r][c] = tmpG[r][c] * 255;
                        tmpB[r][c] = tmpB[r][c] * 255;
                }
        }
	
 	for (r = 0; r < WIDTH; r++) /*check that the RGB values are within the range of 0 and 255*/
        {
                for (c = 0; c < HEIGHT; c++)
                {
                        while (tmpR[r][c] < 0 || tmpR[r][c] > 255)
                        {
                                if (tmpR[r][c] < 0)
                                {
                                        tmpR[r][c] = 0;
                                }
                                else if (tmpR[r][c] > 255)
                                {
                                        tmpR[r][c] = 255;
                                }
                        }
                        while (tmpG[r][c] < 0 || tmpG[r][c] > 255)
                        {
                                if (tmpG[r][c] < 0)
                                {
                                        tmpG[r][c] = 0;
                                }
                                else if (tmpG[r][c] > 255)
                                {
                                        tmpG[r][c] = 255;
                                }
                        }
                        while (tmpB[r][c] < 0 || tmpB[r][c] > 255)
                        {
                                if (tmpB[r][c] < 0)
                                {
                                        tmpB[r][c] = 0;
                                }
                                else if (tmpB[r][c] > 255)
                                {
                                        tmpB[r][c] = 255;
                                }
                        }
                }
        }
	
	for (r = 0; r < WIDTH; r++) /*move the 0-255 temp RGB back to the original RGB to produce hue image*/
        {
                for (c = 0; c < HEIGHT; c++)
                {
                        R[r][c] = tmpR[r][c];
                        G[r][c] = tmpG[r][c];
                        B[r][c] = tmpB[r][c];
                }
        }
}

