/**
 * Simple status program, checks that the PRU firmware is running
 * in the correct state.
 **/
#include <stdio.h>
#include "ledstrips.h"
#include "gamma8.h"

int main() {
  strip_config cfg;
  leds_init(&cfg);
  led_command * cmd = cfg.base_addr;
  printf("dbg: %#08x, %#08x\n", cmd->debug0, cmd->debug1);
  switch(cmd->debug0) {
    case 1:
      printf("Starting...\n");
      break;
    case 2:
      printf("Running and waiting for command.\n");
      break;
    case 3:
      printf("Command received...\n");
      break;
    case 4:
      printf("In frame loop...\n");
      break;
    case 5:
      printf("In bit loop...\n");
      break;
    default:
      printf("Unknown PRU State\n");
      break;
  }
  leds_close(&cfg);
  return 0;
}