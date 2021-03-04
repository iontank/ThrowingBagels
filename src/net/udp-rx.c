#include <stdio.h>
#include <stdlib.h>
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
#include "lut.h"
#include "ledstrips.h"
#include "../demos/modes.h"

#include "gamma8.h"

#define IDLE_MODE(buf,cfg,tick) fill_color(buf, 0x222222, cfg)

static int
udp_socket(
	const int port
)
{
	const int sock = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_port = htons(port),
		.sin_addr.s_addr = INADDR_ANY,
	};

	if (sock < 0)
		return -1;
	if (bind(sock, (const struct sockaddr*) &addr, sizeof(addr)) < 0)
		return -1;

	return sock;
}

static int
wait_socket(
	int fd,
	int msec_timeout
)
{
	struct timeval tv = { msec_timeout / 1000, (msec_timeout % 1000) * 100 };
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(fd, &fds);
	return select(fd+1, &fds, NULL, NULL, &tv);
}

static struct option long_options[] =
{
	{"port",    required_argument, 0, 'p'},
	{"config",  required_argument, 0, 'c'},
	{"timeout", required_argument, 0, 't'},
	{0, 0, 0, 0}
};

int running = 1;

void exit_handler(__attribute__((unused)) int signum) {
  running = 0;
}

int main_loop(strip_config * cfg, int sock, int timeout);

int main(int argc, char ** argv) {
  signal(SIGINT, exit_handler);
  signal(SIGTERM, exit_handler);
  int option_index = 0;
  int port = 9999;
  int timeout = 1000;
  const char * config_file = "leds.config";

  while (1) {
    const int c = getopt_long(
      argc, argv, "p:t:c", long_options,
      &option_index
    );
    if (c == -1) break;
    switch(c) {
      case 'p':
        port = atoi(optarg);
        break;
      case 'c':
        config_file = optarg;
        break;
      case 't':
        timeout = atoi(optarg);
        break;
    }
  }

  strip_config * cfg = leds_config(config_file);
  printf("Launching on port %d, with %d channels and %d pixels per channel.\n", 
    port, cfg->leds_width, cfg->leds_height);
  if (leds_init(cfg) == 0) {
    fprintf(stderr, "Failed to open memory. Did you forget to run as sudo?");
    return 404;
  }
  const int sock = udp_socket(port);
  if (sock < 0) {
    fprintf(stderr, "Failed to acquire socket. Is another copy running?");
    return 800;
  }
  

  return main_loop(cfg, sock, timeout);

}

int main_loop(strip_config * cfg, int sock, int timeout) {
  //Frame Buffer Size for RGB data
  const size_t image_size = cfg->leds_width * cfg->leds_height * 3;
  //Packet size- we expect packets to be half a frame
  const size_t frame_size = image_size / 2 + 1;

  double clock = 0;
  int t_val = timeout;

  uint8_t * packet_buf = calloc(frame_size, sizeof(uint8_t));
  uint32_t * frame_buf = calloc(cfg->leds_width * cfg->leds_height, sizeof(uint32_t));

  while(running) {
    int rc = wait_socket(sock, t_val);
    if (rc < 0) {
      fill_color(frame_buf, 0, cfg);
      leds_draw(cfg, frame_buf);
      return 900;
    }
    if (rc == 0) {
      //we've timed out waiting for a packet
      IDLE_MODE(frame_buf, cfg, clock);
      leds_draw(cfg, frame_buf);
      leds_wait(cfg);
      clock += 0.0250;
      t_val = 500;
      continue;
    }
    t_val = timeout;
    const ssize_t rlen = recv(sock, packet_buf, frame_size, 0);
    if (rlen < 0) {
      fprintf(stderr, "Failed to receive packet.\n");
      return 901;
    }

    const uint8_t frame_part = packet_buf[0];
    unsigned width = cfg->leds_width;
    unsigned height = cfg->leds_height;
    for (unsigned x = 0; x < width; x++) {
      for (unsigned y = 0; y < height; y++) {
        uint32_t * out = &frame_buf[((height*frame_part)/2 + y)*width + x];
        const uint8_t * in = &packet_buf[1 + 3*(y*width + x)];
        *out = (in[0] << 16) | (in[1] << 8) | (in[2] << 0);
      }
    }
    if (frame_part == 1) {
      leds_draw(cfg, frame_buf);
    }
  }
  fill_color(frame_buf, 0, cfg);
  leds_draw(cfg, frame_buf);
  leds_close(cfg);
  return 0;
}