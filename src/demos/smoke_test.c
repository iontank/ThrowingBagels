/**
 * Literal smoke test: set the LEDs to a (dim) white, then back to black.
 * Does not use any drawing functions.
 **/
#include "ledstrips.h"
#include "gamma8.h"
int main() {
  strip_config cfg;
  cfg.leds_width = 32;
  cfg.leds_height = 100;
  cfg.strip_bytes = 4;
  printf("Starting smoke test.\n");
  leds_init(&cfg);
  led_command *cmd = cfg.base_addr;
  cmd->num_pixels = cfg.leds_height;
  cmd->response = 1;
  for (int i = 0; i < 100 * 32 * 4; i++) {
    cmd->pixels_dma[i] = 255;
  }
  printf("Sending white.\n");
  cmd->command = 1;
  leds_wait(&cfg);
  sleep(10);
  printf("Sending black.\n");
  for (int i = 0; i < 100 * 32 * 4; i++) {
    cmd->pixels_dma[i] = 0;
  }
  cmd->command = 1;
  leds_wait(&cfg);
  printf("Done\n");
}