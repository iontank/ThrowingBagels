#include <stdio.h>
#include "ledstrips.h"
#include "gamma8.h"

int main() {
  strip_config *cfg = leds_config("leds.config");
  if (leds_init(cfg) == 0) {
    sprintf(stderr, "Unable to initialize the LEDs, exiting.\n");
    return;
  }
  uint32_t *fb = calloc(cfg->leds_width*cfg->leds_height, sizeof(uint32_t));
  uint32_t colors[] = {0x00FF0000, 0x000000FF};
  int n = 2;
  int i = 0;
  while(1) {
    printf("Setting LEDs\n");
    for (int x = 0; x < cfg->leds_width; x++) {
      for (int y = 0; y < cfg->leds_height; y++) {
        fb[(x * cfg->leds_height) + y] = colors[i%n];
      }
    }
    getchar();
    i++;
  }
}