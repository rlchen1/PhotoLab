#include "Advanced.h"
#include "Image.h"

#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

/* Adjust the brightness of an image */
IMAGE *Brightness(IMAGE *image, int brightness) 
{
	assert(image);
	int x, y;
	int W = ImageWidth(image);
	int H = ImageHeight(image);
	int tmpR, tmpG, tmpB;

	for (x = 0; x < W; x++)
	{
		for (y = 0; y < H; y++)
		{
		tmpR = GetPixelR(image, x, y); /*extract the RGB values from image*/
		tmpG = GetPixelG(image, x, y);
		tmpB = GetPixelB(image, x, y);
			if (tmpR + brightness > 255) /*check the value of the image RGB added with brightness is in range 0-255*/
			{
				tmpR = 255;
			}
			else if(tmpR + brightness < 0)
			{
				tmpR = 0;
			}
			else
			{
				tmpR += brightness;
			}

			if (tmpG + brightness > 255)
			{
				tmpG = 255;
			}
			else if(tmpG + brightness < 0)
			{
				tmpG = 0;
			}
			else
			{
				tmpG += brightness;
			}

			if (tmpB + brightness > 255)
			{
				tmpB = 255;
			}
			else if(tmpB + brightness < 0)
			{
				tmpB = 0;
			}
			else
			{
				tmpB += brightness;
			}

		SetPixelR(image, x, y, tmpR); /*apply the brightness pixels to the image*/
		SetPixelG(image, x, y, tmpG);
		SetPixelB(image, x, y, tmpB);
		}
	}
	return image; /*return the modified brightness image*/
}

/* Mirror an image horizontally */
IMAGE *HMirror(IMAGE *image) 
{
	assert(image);
	int W = ImageWidth(image);
	int H = ImageHeight(image);
	int x, y;
        
	for (x = 0; x < W / 2; x++) 
	{
		for(y = 0; y < H; y++)
		{
			SetPixelR(image, x, y, GetPixelR(image, x, y)); /*copy the left half of image into left half of image*/
			SetPixelG(image, x, y, GetPixelG(image, x, y));
			SetPixelB(image, x, y, GetPixelB(image, x, y));
			
			SetPixelR(image, W-1-x, y, GetPixelR(image, x, y)); /*copy the left half of image into right half of image*/
			SetPixelG(image, W-1-x, y, GetPixelG(image, x, y));
			SetPixelB(image, W-1-x, y, GetPixelB(image, x, y));
		}
	} 
	return image; /*return the modified horizontally mirrored image*/
}

/*matrix function used in HueRotate function*/
void matrix_3_3multiplyVector_3(float vector[3], const float matrix[3][3])
{
	/* Memory for result */
	float result[3] = {0, 0, 0};

	/* Perform multiplication */
	result[0] = matrix[0][0]*vector[0] + matrix[0][1]*vector[1] + matrix[0][2]*vector[2];
	result[1] = matrix[1][0]*vector[0] + matrix[1][1]*vector[1] + matrix[1][2]*vector[2];
	result[2] = matrix[2][0]*vector[0] + matrix[2][1]*vector[1] + matrix[2][2]*vector[2];

	/* Copy result into other stack's memory */
	vector[0] = result[0];
	vector[1] = result[1];
	vector[2] = result[2];
}

/*Adjust the hue of image*/
IMAGE *HueRotate(IMAGE *image, float angle)
{
	/* Matrix to transform normalized RGB color vector from the RGB color space to the YIQ color space */
	static const float RGBtoYIQMatrix[3][3] = {{0.299,  0.587,  0.114},{0.596, -0.274, -0.321},{0.211, -0.523,  0.311}};

	/* Matrix to transform normalized YIQ color vector from the YIQ color space to the RGB color space */
	static const float YIQtoRGBMatrix[3][3] = {{1.000,  0.956,  0.621},{1.000, -0.272, -0.647},{1.000, -1.107,  1.705}};

	const float cs = cos(angle), ss = sin(angle);

	/* Matrix to perform a hue rotation on a normalized YIQ color vector*/
	const float XYPlaneVectorRotateMatrix[3][3] = {{1.000, 0.000, 0.000},{0.000, cs, -ss},{0.000, ss, cs}};

	/* Image variables */
	float temp[3];
	
	assert(image);
	int x, y;
	int W = ImageWidth(image);
	int H = ImageHeight(image);
//	int tmpR, tmpG, tmpB;

	for(x = 0; x < W; x++)
	{
		for(y = 0; y < H; y++)
		{
			/* Create a normalized RGB color vector from the current pixel in the image */
			temp[0] = ((float)GetPixelR(image, x, y))/255;
			temp[1] = ((float)GetPixelG(image, x, y))/255;
			temp[2] = ((float)GetPixelB(image, x, y))/255;

			/* Transform the RGB vector to a YIQ vector */
			matrix_3_3multiplyVector_3(temp, RGBtoYIQMatrix);

			/* Perform the DIP */
			matrix_3_3multiplyVector_3(temp, XYPlaneVectorRotateMatrix);

			/* Transform back to RGB */
			matrix_3_3multiplyVector_3(temp, YIQtoRGBMatrix);

			/* Denormalize and store back into the image */
			temp[0] = temp[0] * 255;
			temp[0] = temp[0] < 0 ? 0 : temp[0];
			temp[0] = temp[0] > 255 ? 255 : temp[0];
			temp[1] = temp[1] * 255;
			temp[1] = temp[1] < 0 ? 0 : temp[1];
			temp[1] = temp[1] > 255 ? 255 : temp[1];
			temp[2] = temp[2] * 255;
			temp[2] = temp[2] < 0 ? 0 : temp[2];
			temp[2] = temp[2] > 255 ? 255 : temp[2];
	
			/*apply the adjusted hue values to the image*/
			SetPixelR(image, x, y, (unsigned char)temp[0]);
		        SetPixelG(image, x, y, (unsigned char)temp[1]);
			SetPixelB(image, x, y, (unsigned char)temp[2]);
		}
	}
	return image; /*return the modified hue image*/
}

/*rotates or flips image*/ 
IMAGE *Rotate(IMAGE *image, int option)
{
	assert(image);
	int x, y;
	int W = ImageWidth(image);
	int H = ImageHeight(image);
	int W2 = H;
	int H2 = W; 	
	IMAGE *tmpImage = CreateImage(W2, H2);

	if (option == 1) /*flips image horizontally*/
	{
		int tmpR, tmpB, tmpG;
		for(x = 0; x < W/2; x++) /*apply pixels in image in reverse horizontal order*/
		{
			for(y = 0; y < H; y++)
			{			
				tmpR = GetPixelR(image, W/2+x, y); /*copy right half of image*/
				tmpG = GetPixelG(image, W/2+x, y);
				tmpB = GetPixelB(image, W/2+x, y);
				
				SetPixelR(image, W/2+x, y, GetPixelR(image, W/2-x, y)); /*apply left half image into right half*/
				SetPixelG(image, W/2+x, y, GetPixelG(image, W/2-x, y));
				SetPixelB(image, W/2+x, y, GetPixelB(image, W/2-x, y));
		
				SetPixelR(image, W/2-x, y, tmpR); /*apply copied right half into left half*/
				SetPixelG(image, W/2-x, y, tmpG);
				SetPixelB(image, W/2-x, y, tmpB);
			}
		}
	}
	
	else if (option == 2) /*flips image vertically*/
	{
		int tmpR, tmpG, tmpB;
		for(x = 0; x < W; x++)
		{
			for(y = 0; y < H/2; y++) /*apply pixels in image in reverse vertical order*/
			{
				tmpR = GetPixelR(image, x, H/2+y); /*copy bottom half of image*/
				tmpG = GetPixelG(image, x, H/2+y);
				tmpB = GetPixelB(image, x, H/2+y);

				SetPixelR(image, x, H/2+y, GetPixelR(image, x, H/2-y)); /*apply top half image into bottom half*/
				SetPixelG(image, x, H/2+y, GetPixelG(image, x, H/2-y));
				SetPixelB(image, x, H/2+y, GetPixelB(image, x, H/2-y));

				SetPixelR(image, x, H/2-y, tmpR); /*apply copied bottom half into top half*/
		                SetPixelG(image, x, H/2-y, tmpG);
				SetPixelB(image, x, H/2-y, tmpB);
			}
		}
	}

	else if (option == 3) /*rotate image right*/
        {
                for(y = H-1; y >= 0; y--) /*extract the image pixels left to right, in bottom to top order*/
                {
                        for(x = 0; x < W; x++) 
                        {
                                SetPixelR(tmpImage, H-1-y, x, GetPixelR(image, x, y)); /*apply the pixels into temporary image top to bottom, in left to right order*/
                                SetPixelG(tmpImage, H-1-y, x, GetPixelG(image, x, y));
                                SetPixelB(tmpImage, H-1-y, x, GetPixelB(image, x, y));
                        }
                }
        }

	else /*rotate image left*/
	{	
		 for(y = 0; y < H; y++) /*extract the image pixels right to left, in top to bottom order*/
                {
                        for(x = W-1; x >= 0; x--)
                        {
                                SetPixelR(tmpImage, y,W-1-x, GetPixelR(image, x, y)); /*apply the pixels into temporary image top to bottom, in left to right order*/
                                SetPixelG(tmpImage, y,W-1-x, GetPixelG(image, x, y));
                                SetPixelB(tmpImage, y,W-1-x, GetPixelB(image, x, y));
                        }
                }
	}

	if (option == 1 || option == 2) /*if flip option chosen, return image*/
	{	
		DeleteImage(tmpImage);
		tmpImage = NULL;
		return image;
	}
	else /*if rotate option chosen, return tmpImage with modified dimensions*/
	{
		DeleteImage(image);
		image = NULL;
		return tmpImage;
	}
}

/*resizes image as smaller or bigger*/
IMAGE *Resize(IMAGE *image, int percentage)
{
	assert(image);
	int W = ImageWidth(image);
	int H = ImageHeight(image);
	int x, y;
	int x2, y2;                         /*variables to hold new dimension*/
	int Wnew = W *(percentage/100.00);  /*temporary image width*/
	int Hnew = H *(percentage/100.00);  /*temporary image height*/
	unsigned char tmpR, tmpG, tmpB;

	IMAGE *tmpImage = CreateImage(Wnew, Hnew); /*create temporary image with different dimensions for resizing*/
	

	if (percentage == 100) /*if rescale value is 100, return original image*/
	{
		DeleteImage(tmpImage);
		return image;
	}

	else if (percentage > 100) /*if rescale value is over 100%, enlarge image*/
	{
		for (x = 0; x < W; x++)
		{
			for(y = 0; y < H; y++)
			{
                                x2 = x * (percentage/100.00);   /*get the new dimension for current pixel*/
                                y2 = y * (percentage/100.00);

				tmpR = GetPixelR(image, x, y); /*extract pixels values before deleting image*/
				tmpG = GetPixelG(image, x, y);
				tmpB = GetPixelB(image, x ,y);

                                SetPixelR(tmpImage, x2, y2, tmpR); /*apply extracted pixel to new dimension location in temporary image*/
                                SetPixelG(tmpImage, x2, y2, tmpG);
                                SetPixelB(tmpImage, x2, y2, tmpB);

			        SetPixelR(tmpImage, x2 + (percentage/100.00), y2, tmpR); /*interpolate for x in temporary image*/
			        SetPixelG(tmpImage, x2 + (percentage/100.00), y2, tmpG);
			        SetPixelB(tmpImage, x2 + (percentage/100.00), y2, tmpB);

			        SetPixelR(tmpImage, x2, y2 + (percentage/100.00), tmpR); /*interpolate for y with respect to x in temporary image*/
			        SetPixelG(tmpImage, x2, y2 + (percentage/100.00), tmpG);
			        SetPixelB(tmpImage, x2, y2 + (percentage/100.00), tmpB);

     		                SetPixelR(tmpImage, x2 + (percentage/100.00), y2 + (percentage/100.00), tmpR); /*interpolate for y with respect to shifted x in temporary image*/
                                SetPixelG(tmpImage, x2 + (percentage/100.00), y2 + (percentage/100.00), tmpG);
                                SetPixelB(tmpImage, x2 + (percentage/100.00), y2 + (percentage/100.00), tmpB);	
			}
		}
		DeleteImage(image);
		return tmpImage; /*return the modified enlarged image stored in tmpImage*/
	}

	else /*if rescale value is under 100, shrink image*/
	{	
		for (x = 1; x < W; x++) /*take the average RGB value for each x,y pixel in image, excluding edges*/
		{
			for (y = 1; y < H; y++)
			{
				x2 = x * (percentage/100.00); /*get the new dimension for current pixel*/
                                y2 = y * (percentage/100.00);

				tmpR = (GetPixelR(image, x, y) + GetPixelR(image, x-1, y-1) + GetPixelR(image, x, y-1) + GetPixelR(image, x-1, y))/4; /*average with top left 4 pixels*/
				tmpG = (GetPixelG(image, x, y) + GetPixelG(image, x-1, y-1) + GetPixelG(image, x, y-1) + GetPixelG(image, x-1, y))/4;
				tmpB = (GetPixelB(image, x, y) + GetPixelB(image, x-1, y-1) + GetPixelB(image, x, y-1) + GetPixelB(image, x-1, y))/4;
			
				SetPixelR(tmpImage, x2, y2, tmpR); /*apply the averaged pixels to image*/
				SetPixelG(tmpImage, x2, y2, tmpG);
				SetPixelB(tmpImage, x2, y2, tmpB);
		
				SetPixelR(tmpImage, x2, 0, GetPixelR(image, x, 0)); /*set the edges at 0 to the original pixel value*/
				SetPixelR(tmpImage, x2, 0, GetPixelR(image, x, 0));
				SetPixelR(tmpImage, x2, 0, GetPixelR(image, x, 0));

				SetPixelR(tmpImage, 0, y2, GetPixelR(image, 0, y));
				SetPixelG(tmpImage, 0, y2, GetPixelG(image, 0, y));
				SetPixelB(tmpImage, 0, y2, GetPixelB(image, 0, y));
			}
		}
		DeleteImage(image);
		return tmpImage; /*return the modified shrunken image stored in tmpImage*/
	}		
}

/*adds color saturation to image*/
IMAGE *Saturate(IMAGE *image, float percent)
{
	assert(image);
	int x, y;
	int W = ImageWidth(image);
	int H = ImageHeight(image);
	float GrayR, GrayG, GrayB;
	int tmpR, tmpG, tmpB;
	
	for(x = 0; x < W; x++)
	{
		for(y = 0; y < H; y++)
		{
			GrayR = (GetPixelR(image, x, y) + GetPixelG(image, x, y) + GetPixelB(image, x, y))/3; /*take the average RGB value (grayscale)*/
			GrayG = GrayB = GrayR;
			GrayR = GetPixelR(image, x, y) - GrayR; /*subtract the average RGB (grayscale) value from the pixel values*/
			GrayG = GetPixelG(image, x, y) - GrayG;
			GrayB = GetPixelB(image, x, y) - GrayB;

			tmpR = GetPixelR(image, x, y) + (GrayR * (percent/100.00)); /*multiply the grayscale value by the passed percentage value, add to pixel value*/
			tmpG = GetPixelG(image, x, y) + (GrayG * (percent/100.00));
			tmpB = GetPixelB(image, x, y) + (GrayB * (percent/100.00));			

			if (tmpR < 0) /*assert that the modified values of RGB are within the range of 0-255*/
			{
			    tmpR = 0;
			}
			else if (tmpR > 255)
			{
			    tmpR = 255;
			}
			if (tmpG < 0)
			{
			    tmpG = 0;
			}
			else if (tmpR > 255)
			{
			    tmpR = 255;
			}
			if (tmpB < 0)
			{
			    tmpB = 0;
			}
			else if (tmpB > 255)
			{
			    tmpB = 255;
			}

			SetPixelR(image, x, y, tmpR); /*apply the saturated pixel values to the image*/
			SetPixelG(image, x, y, tmpG);
			SetPixelB(image, x, y, tmpB);	
		}
	}
	return image; /*return the modified saturated image*/
}
/* vim: set tabstop=8 softtabstop=8 shiftwidth=8 noexpandtab : */
