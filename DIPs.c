#include "DIPs.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

/* Example of DIP */
/* make the picture color black & white */
IMAGE *BlackNWhite(IMAGE *image)
{
   assert(image); 
   int x, y, tmp;
   int W = ImageWidth(image); /*extract the width and height of image loaded*/
   int H = ImageHeight(image);

    for (y = 0; y < H; y++) /*take the average of RGB values and apply to image*/
    {
        for (x = 0; x < W; x++)
        {
	    tmp = (GetPixelR(image, x, y) + GetPixelG(image, x, y) + GetPixelB(image, x, y))/3;
	    SetPixelR(image, x, y, tmp);
	    SetPixelG(image, x, y, tmp);
	    SetPixelB(image, x, y, tmp);
        }
    }
    return image; /*return the modified black and white image*/
}

/* Add edge to the image*/
IMAGE *Edge(IMAGE *image) 
{
    assert(image);
    int x, y, m, n, a, b;
    int W = ImageWidth(image);
    int H = ImageHeight(image);
    int tmpR, tmpG, tmpB;
    unsigned char tempR[W][H];
    unsigned char tempG[W][H];
    unsigned char tempB[W][H];

    for (y = 0; y < H; y++) /*extract the RGB values of image into separate arrays*/
    {
        for (x = 0; x < W; x++) 
	{
            tempR[x][y] = GetPixelR(image, x, y);
	    tempG[x][y] = GetPixelG(image, x, y);
	    tempB[x][y] = GetPixelB(image, x, y);
       }
    }
    int sumR = 0;   /* sum of the intensity differences with neighbors */
    int sumG = 0;
    int sumB = 0;
    for (y = 1; y < H - 1; y++){
        for (x = 1; x < W - 1; x++){
            for (n = -1; n <= 1; n++){
                for (m = -1; m <= 1; m++) {
                    a = (x + m >= W) ? W - 1 : (x + m < 0) ? 0 : x + m;
                    b = (y + n >= H) ? H - 1 : (y + n < 0) ? 0 : y + n;
                    sumR += (tempR[x][y] - tempR[a][b]);
                    sumG += (tempG[x][y] - tempG[a][b]);
                    sumB += (tempB[x][y] - tempB[a][b]);
                }
            }
            tmpR = (sumR > MAX_PIXEL) ? MAX_PIXEL: (sumR < 0) ? 0: sumR;
            tmpG = (sumG > MAX_PIXEL) ? MAX_PIXEL: (sumG < 0) ? 0: sumG;
            tmpB = (sumB > MAX_PIXEL) ? MAX_PIXEL: (sumB < 0) ? 0: sumB;

            sumR = sumG = sumB = 0; /*reset the computed difference for each for loop of n and m*/

	    SetPixelR(image, x, y, tmpR); /*apply the edge values to the image*/
	    SetPixelG(image, x, y, tmpG);
	    SetPixelB(image, x, y, tmpB);
       }
    }
    /* set all four borders to 0 */
    for (y = 0; y < H; y++) 
    {
        SetPixelR(image, 0, y, 0);
	SetPixelG(image, 0, y, 0);
	SetPixelB(image, 0, y, 0);
	SetPixelR(image, W-1, y, 0);
	SetPixelG(image, W-1, y, 0);
        SetPixelB(image, W-1, y, 0);
    }

    for (x = 0; x < W; x++) 
    {
        SetPixelR(image, x, 0, 0);
	SetPixelG(image, x, 0, 0);
	SetPixelB(image, x, 0, 0);
	SetPixelR(image, x, H-1, 0);
	SetPixelG(image, x, H-1, 0);
	SetPixelB(image, x, H-1, 0);
    }
 
    return image; /*return the modified edge detected image*/
}

/* Shuffle the image */
IMAGE *Shuffle(IMAGE *image)
{
    assert(image);
    int W = ImageWidth(image);
    int H = ImageHeight(image);

    int block_cnt = SHUFF_HEIGHT_DIV * SHUFF_WIDTH_DIV;
    int block_width = W/SHUFF_WIDTH_DIV;
    int block_height = H/SHUFF_HEIGHT_DIV;
    int block[SHUFF_WIDTH_DIV][SHUFF_HEIGHT_DIV];
    int i, j;


    srand(time(NULL));
    /* Initialize block markers to not done (-1) */
    for (i = 0; i < SHUFF_WIDTH_DIV; i++) {
        for (j = 0; j< SHUFF_HEIGHT_DIV; j++) {
            block[i][j] = -1;
        }
    }

    while (block_cnt > 0) {
        /* Generate a random pair of blocks */
        int dest_height = rand() % SHUFF_HEIGHT_DIV;
        int dest_width = rand() % SHUFF_WIDTH_DIV;
        int src_height = rand() % SHUFF_HEIGHT_DIV;
        int src_width = rand() % SHUFF_WIDTH_DIV;

        /* Check if these blocks are already swapped, if not swap blocks */
        if ((block[dest_width][dest_height] == -1) && (block[src_width][src_height] == -1)) {
            /* Swap blocks */
            for (i = 0; i < block_height; i++) {
                /* Init src and dest height offset */
                int h_dest = ((dest_height * block_height) + i) % H;
                int h_src  = ((src_height * block_height) + i) % H;
                for (j = 0; j < block_width; j++) {
                    int tmpR, tmpG, tmpB;
                    /* Init src and dest width offset */
                    int w_src  = ((src_width * block_width) + j) % W;
                    int w_dest = ((dest_width * block_width) + j) % W;

                   /*Swap the source and destination blocks for R, G, B*/
		    tmpR = GetPixelR(image, w_src, h_src);
		    SetPixelR(image, w_src, h_src, GetPixelR(image, w_dest, h_dest));
		    SetPixelR(image, w_dest, h_dest, tmpR);

                    tmpG = GetPixelG(image, w_src, h_src);
		    SetPixelG(image, w_src, h_src, GetPixelG(image, w_dest, h_dest));
	            SetPixelG(image, w_dest, h_dest, tmpG);

                    tmpB = GetPixelB(image, w_src, h_src);
		    SetPixelB(image, w_src, h_src, GetPixelB(image, w_dest, h_dest));
    		    SetPixelB(image, w_dest, h_dest, tmpB);
                }
            }
            /* Set marker as done */
            block[dest_width][dest_height] = 1;
            block[src_width][src_height] = 1;
            /* Decrease block count */
            block_cnt -= 2; /* Two blocks are swapped */
        }
    }
    return image; /*return the modified shuffled image*/
}

/* vim: set tabstop=8 softtabstop=8 shiftwidth=8 noexpandtab : */
