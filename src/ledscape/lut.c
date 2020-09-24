#include "lut.h"

extern const uint8_t gamma8[];

#define MAX(a,b) (((a) < (b)) ? (b) : (a))
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define CLAMP(a) (a < 0 ? 0 : a > 255 ? 255 : (uint8_t)a)
#define LUT(a) (gamma8[(uint8_t)a])

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

//default gamma curve, based on Adafruit's neopixel gamma curve
const uint8_t gamma8[] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
  2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
  10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
  17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
  25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
  37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
  51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
  69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
  90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };
