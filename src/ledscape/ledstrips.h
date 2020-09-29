/** \file
 * Drive up to 32 LED strips on a Beaglebone Black
**/

#ifndef _ledstrips_h_
#define _ledstrips_h_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>

#include "lut.h"

#define STRIP_NUM_CHANNELS 32
#define STRIP_BASE_MEM 0x80000000

typedef struct
{
	volatile uint32_t debug0;
	volatile uint32_t debug1;
	// in the DDR shared with the PRU
	uint8_t* pixels_dma;

	// Length in pixels of the longest LED strip.
	volatile uint16_t num_pixels;

	// write 1 to start, 0xFF to abort. will be cleared when started
	volatile uint8_t command;

	// will have a non-zero response written when done
	volatile uint8_t response;
	volatile uint8_t num_channels; //should be 3 or 4
} __attribute__((__packed__)) led_command;

typedef struct {
  int leds_width; //X axis, num channels, should be <=32
  int leds_height; //Y axis, LEDs per channel, must be the same on all channels
  uint8_t strip_bytes; //3 or 4
  void *base_addr; //memory location
} strip_config;

int leds_init(strip_config *cfg);
void leds_draw(strip_config * cfg, const void * const frame);
void leds_wait(strip_config * cfg);
strip_config* leds_config(char const * filename);
#endif