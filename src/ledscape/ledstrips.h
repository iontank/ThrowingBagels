/** \file
 * Drive up to 32 LED strips on a Beaglebone Black
 * In addition to this file, you must also include a LUT file,
 * e.g. your program should start with:
 * 
 * #include "ledstrips.h"
 * #include "gamma8.h"
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

//The maximum number of supported channels
#define STRIP_NUM_CHANNELS 32
//the memory location of the PRUs memory
#define STRIP_BASE_MEM 0x80000000
//the amount of memory we want to reserve, which is more than we
//reasonably plan to use
#define STRIP_MEM_SIZE 132072

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
  void *base_addr; //memory location we share with the PRU
} strip_config;

/**
 * Initialize the PRU communication. Allocates memory, based on our configuration.
 **/
int leds_init(strip_config *cfg);
/**
 * Flush a frame (a byte array) to the LEDs. Assumes 24-bit RGB values in 32-bit spaces
 * i.e., 0x00FFFFFF is full white.
 * 
 * This assumes your LEDs are GRB or GRBW
 **/
void leds_draw(strip_config * cfg, const void * const frame);
/**
 * Wait on the PRU to flush the LED data.
 * This should not take very long at all, but will
 * hang indefinitely if the PRU fails for some reason.
 **/
void leds_wait(strip_config * cfg);
/**
 * Parse a config file.
 * The config file format is:
 * WIDTH(num channels),HEIGHT(length of LED strips),bytes (3 or 4, to match your LEDs)
 * i.e., 32,100,4
 * 
 * Strip bytes is optional, 4 is the default
 **/
strip_config* leds_config(char const * filename);

/**
 * Release the memory that we're using to share with the PRU
 **/
void leds_close(strip_config * cfg);
#endif