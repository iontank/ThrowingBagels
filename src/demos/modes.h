/**
 * Demo Modes
 * These methods allow test programs to quickly and easily send data to LEDs.
 * Useful for disconnected idle modes, demonstrations, or hardware testing/profiling.
 **/
#include <stdio.h>
#include <stdlib.h>
#include "sdnoise.h"
#include "rgb_hsv.h"
#include "ledstrips.h"


#ifndef MODES_H
#define MODES_H

#define red(c) ((c & 0xFF0000) >> 16)
#define green(c) ((c & 0x00FF00) >> 8)
#define blue(c) ((c & 0x0000FF))
#define color(r,g,b) (((uint8_t)r << 16) | ((uint8_t)g << 8) | ((uint8_t)b))

/**
 * Blend: mix two colors using a weight (from 0-1)
 **/
uint32_t blend(uint32_t l, uint32_t q, double weight) {
	double w = fabs(weight);
	double r = red(l) * (1.0 - w) + red(q) * w;
	double g = green(l) * (1.0 - w) + green(q) * w;
	double b = blue(l) * (1.0 - w) + blue(q) * w;
	return color(r, g, b);
}

/**
 * fill_frame: fill the framebuffer using an `rgb_color` struct
 **/
void fill_frame(uint32_t * fb, 
                  rgb_color rgb, strip_config * config) {
  int w = config->leds_width;
  int h = config->leds_height;
  uint32_t color = (rgb.r << 16) | (rgb.g << 8) | (rgb.b);
  for (int i = 0; i < w * h; i++) {
    fb[i] = color;
  }
}

/**
 * fill_color: fill the frame using a 32-bit RGB value
 */
void fill_color(uint32_t * fb, uint32_t rgb, strip_config * config) {
  uint32_t color = rgb;
  int w = config->leds_width;
  int h = config->leds_height;
  for (int i = 0; i < w * h; i++) {
    fb[i] = color;
  }
}

/**
 * perform a hue rotation animation based on the `clock` time
 */
void hue_rotation(uint32_t * fb, 
                  uint32_t clock,
                  strip_config * config,
                  double speed) {

  hsv_color c = {0, 255, 128};
  double progress = fmod(clock, speed) / speed;
  c.h = (uint8_t)(255*progress);
  rgb_color rgb = hsv_to_rgb(c);
  fill_frame(fb, rgb, config);
}

/**
 * Ramp between main_color and accent_color, using the clock time and a speed multiplier
 */
void ramp(uint32_t * fb, uint32_t clock, strip_config * config,
          uint32_t main_color, uint32_t accent_color, double speed) {
  double progress = fmod(clock, speed) / speed;
  uint32_t rgb = blend(accent_color, main_color, progress);
  fill_color(fb, rgb, config);
}

/**
 * Perform a stripwise pulse animation, with a main color flying over
 * an accent color, with a pulse of a certain size and moving ata certain speed
 **/
void pulse(uint32_t * fb, uint32_t clock, strip_config * config,
          uint32_t main_color, uint32_t accent_color,
          double size, double speed) {
  int w = config->leds_width;
  int h = config->leds_height;
  double pulse_y = fmod((clock * speed), w);
  for (int x = 0; x < w; x++) {
    double d = fabs(pulse_y - x);
    if (d > size) d = size;
    double weight = d/size;
    uint32_t color = blend(main_color, accent_color, weight);
    for (int y = 0; y < h; y++) {
      fb[(y*w)+x] = color;
    }
  }
}

/**
 * Play sinusoidal noise, between two colors, with a size/speed scale factor
 **/ 
void sinusoidal(uint32_t * fb, uint32_t clock, strip_config * config,
                 uint32_t primary, uint32_t accent, 
                 double space_scale, double time_scale) {
  int w = config->leds_width;
  int h = config->leds_height;
  double s = ((double)clock)/time_scale;
  for (int x = 0; x < w; x++) {
    for (int y = 0; y < h; y++) {
      uint32_t p = blend(accent, primary, sin((y + x / 4)*space_scale+s));
      fb[(x*h)+y] = p;
    }
  }
}

/**
 * Play a perlin noise animation
 */
void noise_field(uint32_t * fb, uint32_t clock, strip_config * config, double space_scale, double time_scale) {
  int w = config->leds_width;
  int h = config->leds_height;
  double s = ((double)clock)/time_scale;
  hsv_color hsv = {0., 255., 128.};
  rgb_color rgb;
  for (int x = 0; x < w; x++) {
    float xp = x * space_scale;
    for (int y = 0; y <h; y++) {
      float yp = y * space_scale;
      hsv.h = (uint8_t)(255. * (1. + sdnoise3(xp, yp, s, NULL, NULL, NULL)) / 2);
      rgb = hsv_to_rgb(hsv);
      uint32_t c = color(rgb.r, rgb.g, rgb.b);
      fb[(x*h)+y] = c;
    }
  }
}

uint32_t defaultPalette[] = {
  0xFF00FF,
  0x00FFFF,
  0x888888,
  0x222222,
  0xFFA500
};
int defaultCount = 5;
//7 segments
void color_segments(uint32_t * fb, uint32_t clock, strip_config * config, int segmentSize, uint32_t * palette, int numColors, double timeScale) {
  int w = config->leds_width;
  int h = config->leds_height;
  for (int x = 0; x < w; x++) {
    for (int y = 0; y < h; y+=segmentSize) {
      double step = ((double)clock)/timeScale;
      int seg = (int)(step + y) % numColors;
      uint32_t color = palette[seg];
      for (int i = 0; i < segmentSize; i++) {
        fb[(x*h)+y+i] = color;
      }
    }
  }
}

#endif
