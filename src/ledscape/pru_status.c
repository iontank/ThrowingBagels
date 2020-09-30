#include <stdio.h>
#include "ledstrips.h"
#include "gamma8.h"

int main() {
  strip_config cfg;
  leds_init(&cfg);
  led_command * cmd = cfg.base_addr;
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
  return 0;
}