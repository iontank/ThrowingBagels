#include "lut.h"

extern const uint8_t lut1d[];

#define MAX(a,b) (((a) < (b)) ? (b) : (a))
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define CLAMP(a) (a < 0 ? 0 : a > 255 ? 255 : (uint8_t)a)
#define LUT(a) (lut1d[(uint8_t)a])

/* Split a 24-bit color and apply the LUT to each byte */
uint32_t apply_gamma_lut(uint32_t c) 
{
  uint8_t r = ((0xFF0000 & c) >> 16);
  uint8_t g = ((0x00FF00 & c) >> 8);
  uint8_t b = ((0x0000FF & c));
  r = LUT(r); g = LUT(g); b = LUT(b);
  return (r << 16) | (g << 8) | (b);  
}

uint8_t saturation(uint8_t r, uint8_t g, uint8_t b) {
  float low = MIN(r, MIN(g, b));
  float high = MAX(r, MAX(g, b));
  return (uint8_t)roundf(100 * ((high - low) / high));
}

uint8_t white(uint8_t r, uint8_t g, uint8_t b) {
  return (0xFF - saturation(r,g,b)) / 0xFF * (r + g + b) / 3;
}

/* Convert RGB to RGBW data */
uint32_t rgb_to_rgbw(uint8_t in_r, uint8_t in_g, uint8_t in_b) 
{
  uint8_t r,g,b;
  r = LUT(in_r);
  g = LUT(in_g);
  b = LUT(in_b);
  uint8_t w = white(r,g,b);
  return (w << 24) | (r << 16) | (g << 8) | b;
}

uint32_t apply_rgbw_lut(uint32_t pixel) {
  uint8_t r = ((pixel & 0xFF0000) >> 16);
  uint8_t g = ((pixel & 0x00FF00) >> 8);
  uint8_t b = ((pixel & 0x0000FF) >> 0);
  return rgb_to_rgbw(r,g,b);
}

uint32_t apply_lut(uint32_t pixel, uint8_t bytes) {
  if (bytes == 3) {
    return apply_gamma_lut(pixel);
  } else if (bytes == 4) {
    return apply_rgbw_lut(pixel);
  }
  return -1;
}

