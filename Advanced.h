#ifndef ADVANCED_H_INCLUDED_
#define ADVANCED_H_INCLUDED_

#include "Constants.h"
#include "Image.h"

/* Adjust the brightness of an image */
IMAGE *Brightness(IMAGE *image, int brightness);

/* Mirror an image horizontally */
IMAGE *HMirror(IMAGE *image);

void matrix_3_3multiplyVector_3(float vector[3], const float matrix[3][3]);

/* Change the hue of an image */
IMAGE *HueRotate(IMAGE *image, float angle);

/* Resize an image */
IMAGE *Resize(IMAGE *image, int percentage);

/* Rotate an image */
IMAGE *Rotate(IMAGE *image, int option);

/* Saturate an image*/
IMAGE *Saturate(IMAGE *image, float percent);

#endif /* ADVANCED_H_INCLUDED_ */

/* vim: set tabstop=8 softtabstop=8 shiftwidth=8 noexpandtab : */
