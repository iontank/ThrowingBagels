#include "ledstrips.h"
#include "gamma8.h"
int main() {
  strip_config cfg;
  cfg.leds_width = 32;
  cfg.leds_height = 100;
  cfg.strip_bytes = 4;
  leds_init(&cfg);
  led_command *cmd = cfg.base_addr;
  printf("Setting white\n");
  for (int i = 0; i < 100 * 32 * 4; i++) {
    cmd->pixels_dma[i] = 0x88;
  }
  printf("Sending\n");
  cmd->command = 1;
  printf("Waiting... ");
  leds_wait(&cfg);
  sleep(10);
  printf("setting black... ");
  for (int i = 0; i < 100 * 32 * 4; i++) {
    cmd->pixels_dma[i] = 0;
  }
  cmd->command = 1;
  leds_wait(&cfg);
}