/**
 * Apply LUTs to input data
 * This allows raw RGB to map nicely to the dynamic range of our LEDs
 **/
#ifndef _lut_h_
#define _lut_h_

#include <stdint.h>
#include <math.h>

/**
 * Given a 32-bit xRGB input, and how many output bytes we are using (3 or 4)
 * this will apply a LUT.
 * For 3-bytes, output order is RGB
 * For 4-bytes, output order is WRGB
 **/
uint32_t apply_lut(uint32_t pixel, uint8_t bytes);
#endif