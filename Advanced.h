#ifndef Advanced_H
#define Advanced_H

#include "Constants.h"

/*make the image brightness different*/
void Brightness(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], int brightness);

/*reflect the image from left to right*/
void HMirror(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]);

/*adjust the hue of image*/
void HueRotate(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], float angle);

#endif /*Advanced_H*/
