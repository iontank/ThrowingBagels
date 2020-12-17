#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>
#include <termios.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <getopt.h>
#include <math.h>
#include "gamma8.h"
#include "ledstrips.h"
#include "modes.h"

static struct option long_options[] = {
  {"hex color", required_argument, 0, 'x'},
  {"config", optional_argument, 0, 'c'}
};

int running = 1;

void exit_handler(__attribute__((unused)) int signum) {
  running = 0;
}

int main(int argc, char ** argv) {
  signal(SIGINT, exit_handler);
  signal(SIGTERM, exit_handler);
  uint32_t color = 0xFF0000;
  int option_index = 0;
  char * config = "leds.config";
  while (1) {
    const int c = getopt_long(
      argc, argv, "x:c",
      long_options, &option_index
    );
    if (c == -1) break;
    switch(c) {
      case 'x':
        color = strtol(optarg, 0, 16);
        break;
      case 'c':
        config = optarg;
        break;
    }
  }
  strip_config * cfg = leds_config(config);
  leds_init(cfg);
  uint32_t * fb = calloc(cfg->leds_width*cfg->leds_height, sizeof(uint32_t));
  fill_color(fb, color, cfg);
  leds_draw(cfg, fb);
  led_command *cmd = cfg->base_addr;
  while(running) {
    sleep(1);
    leds_draw(cfg, fb);
  }
  fill_color(fb, 0, cfg);
  leds_draw(cfg, fb);
  leds_close(cfg);
  return 0;
}