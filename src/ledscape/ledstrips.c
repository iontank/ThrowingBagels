#include "ledstrips.h"
#include "util/readline.h"


strip_config* leds_config(char const * filename) {
  strip_config * res = calloc(0, sizeof(strip_config));
  int width, height, bytes;
  FILE * const file = fopen(filename, "r");
  if (!file) goto open_fail;
  char buffer[256];
  if (readline(file, buffer, sizeof(buffer)) < 0)
    goto parse_fail;
  int count = sscanf("32,100,4", "%d,%d,%d", &width, &height, &bytes);
  printf(" parsed %d entries...\n", count);
  if (count == 2) {
    bytes = 4; //default to RGBW since we use that more
  } else if (count < 2) {
    goto parse_fail;
  }
  res->leds_height = height;
  res->leds_width = width;
  res->strip_bytes = bytes;
  printf("Launching with %d strips, each %d LEDs long, and %d bytes per LED\n", width, height, bytes);
  return res;
parse_fail:
  fprintf(stderr, "%s: parse error. LED strips not configured.\n", filename);
  goto fail_exit;
open_fail:
  fprintf(stderr, "%s: unable to open file. LED strips not configured.\n", filename);
  goto fail_exit;
fail_exit:
  free(res);
  return NULL;
}

int leds_init(strip_config *cfg) {
  //create a memory map large enough fro a frame buffer and command info
  size_t map_size = cfg->leds_height * STRIP_NUM_CHANNELS * cfg->strip_bytes + sizeof(led_command);
  unsigned long map_mask = (map_size - 1);
  off_t target = STRIP_BASE_MEM;
  void *map_base;
  int fd;
  //open memory
  if ((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) return 0;
  map_base = mmap(0, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, target & ~map_mask);
  cfg->base_addr = map_base + (target & map_mask);
  led_command *ptr = cfg->base_addr;
  ptr->pixels_dma = cfg->base_addr + sizeof(led_command);
  memset(ptr->pixels_dma, 0, STRIP_NUM_CHANNELS * cfg->leds_height * cfg->strip_bytes);
  return 1;
}

void leds_draw(strip_config * cfg, const void * const frame) {
  led_command *cmd = cfg->base_addr;
  cmd->num_pixels = cfg->leds_height; //ensure the client hasn't mucked with the cfg
  memset(cmd->pixels_dma, 0, STRIP_NUM_CHANNELS*cfg->leds_height*cfg->strip_bytes);
  const uint32_t * const in = frame;
  uint8_t * const out = cmd->pixels_dma;
  const unsigned pru_stride = STRIP_NUM_CHANNELS;
  uint8_t byteOrder[] = {8, 16, 0, 24};
  for (int x = 0; x < cfg->leds_width; x++) {
    for (int y = 0; y < cfg->leds_height; y++) {
      uint32_t p = in[(x*cfg->leds_height) + y];
      p = apply_lut(p, cfg->strip_bytes);
      uint8_t * const row_out = &out[y*pru_stride*cfg->strip_bytes + x];
      for (int i = 0; i < cfg->strip_bytes; i++) {
        uint8_t b = byteOrder[i];
        uint8_t pp = (p >> b) & 0xFF;
        row_out[i * pru_stride] = pp;
      }
    }
  }
  cmd->command = 1; 
}

void leds_wait(strip_config * cfg) {
  led_command *cmd = cfg->base_addr;
  while (cmd->response == 0);
  cmd->response = 0;
}
