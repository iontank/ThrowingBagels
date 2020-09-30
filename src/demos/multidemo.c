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

#define FPS 30
#define PI 3.14159
#define SCALE 1.0/8.0*PI
#define T_SCALE (double)45.0

typedef struct mode_config {
  uint32_t seconds_per_mode;
  uint32_t main_color;
  uint32_t accent_color;
  char * config_file;
  char * active_modes;
  uint8_t nmodes;
  double pulse_speed;
  double pulse_size;
  double sinusoid_scale;
  double sinusoid_speed;
  bool white_ramp;
  double ramp_speed;
  double hue_rotation_speed;
  double noise_scale;
  double noise_speed;
  uint8_t block_size;
  double block_speed;
} mode_config;

void print_usage() {
  printf("%s",
    "Run a sequence of demos.\n"
    "\tUsage: multidemo [OPTIONS]\n"
    "\n"
    "Options:\n"
    "\t-?/--help: print this message and exit\n"
    "\t-f [file]: the ledscape config file to use\n"
    "\t-d [seconds]: the duration (in seconds) of each animation we cycle through\n"
    "\t-q [modelist]: a list of the modes as a string, `hrsp`: (h)ue rotation, (r)amp, (s)inusoid, (p)ulse, (n)oise, color (b)lock\n"
    "\t-m [color]: main color as a hex triplet (no \"#\", \"0x\" is acceptable)\n"
    "\t-a [color]: accent color as a hex triplet (no \"#\", \"0x\" is acceptable)\n"
    "\t-s [SPEED,SCALE]: sinusoid settings, a pair of numbers in the form `SPEED,SCALE`\n"
    "\t-p [SPEED,SCALE]: pulse settings, a pair of numbers in the form `SPEED,SIZE`. Speed is duration is seconds, size is number of LEDs\n"
    "\t-r [seconds]: ramp speed (in seconds)\n"
    "\t-w: by default, ramp goes black-to-white, if this flag is present, it uses the main color and accent color\n"
    "\t-h [seconds]: hue rotation speed, in seconds\n"
    "\t-n [SPEED,SCALE]: noise settings, a pair of numbers in the form `SPEED,SCALE`\n"
    "\t-b [SPEED,BLOCK_SIZE]: speed factor and the pixel size of each block\n"
  );
}

void parse_block(mode_config * cfg, char * argstring) {
  int i = 0;
  char * dup = strdup(argstring);
  char * tok;
  while ((tok = strsep(&dup, ":-/,")) != NULL) {
    if (i == 0) cfg->block_speed = atof(tok);
    if (i == 1) cfg->block_size = atoi(tok);
    i++;
  }
  free(dup);
}

void parse_sinusoid(mode_config * cfg, char * argstring) {
  int i = 0;
  char * dup = strdup(argstring);
  char * tok;
  while ((tok = strsep(&dup, ":-/,")) != NULL) {
    if (i == 0) cfg->sinusoid_speed = atof(tok);
    if (i == 1) cfg->sinusoid_scale = atof(tok) * PI;
    i++;
  }
  free(dup);
}

void parse_pulse(mode_config * cfg, char * argstring) {
  int i = 0;
  char * dup = strdup(argstring);
  char * tok;
  while ((tok = strsep(&dup, ":-/,")) != NULL) {
    if (i == 0) cfg->pulse_speed = atof(tok)/FPS;
    if (i == 1) cfg->pulse_size = atof(tok);
    i++;
  }
  free(dup);
}

void parse_noise(mode_config * cfg, char * argstring) {
  int i = 0;
  char * dup = strdup(argstring);
  char * tok;
  while ((tok = strsep(&dup, ":-/,")) !=NULL) {
    if (i == 0) cfg->noise_speed = atof(tok);
    if (i == 1) cfg->noise_scale = atof(tok);
    i++;
  }
}

mode_config parse_args(int argc, char ** argv) {
  int option_index;
  mode_config res = {
    //seconds per mode, main color, accent color, configfile
    5, 0xFF00FF, 0x00FFFF, "leds.config", 
    //modes, nmodes
    "hsrpn", 4, 
    //pulse: speed/size
    1, 4, 
    //sinus: scale/speed
    SCALE, T_SCALE,
    //ramp: palette/speed
    true, 3.*FPS,
    //hue: speed
    3.*FPS,
    //noise: scale/speed
    0.08,100,
    //block: speed/size
    0.08,7
  };
  static struct option long_options[] = {
    {"modedur", required_argument, 0, 'd'},
    {"sinusoid", required_argument, 0, 's'},
    {"pulse", required_argument, 0, 'p'},
    {"maincolor", required_argument, 0, 'm'},
    {"accentcolor", required_argument, 0, 'a'},
    {"activemodes", required_argument, 0, 'q'},
    {"configfile", required_argument, 0, 'f'},
    {"ramp", required_argument, 0, 'r'},
    {"colorramp", no_argument, 0, 'w'},
    {"help", no_argument, 0, '?'},
    {"huerotationspeed", required_argument, 0, 'h'},
    {"noise", required_argument, 0, 'n'}
  };
  while(1) {
    const int c = getopt_long(
      argc, argv,
      "?f:d:q:m:a:s:p:r:wh:n:",
      long_options, &option_index
    );
    if (c == -1) return res;
    switch(c) {
      case '?':
        print_usage();
        exit(0);
      case 'f':
        res.config_file = optarg;
        break;
      case 'd':
        res.seconds_per_mode = atoi(optarg);
        break;
      case 's':
        parse_sinusoid(&res, optarg);
        break;
      case 'm':
        res.main_color = strtol(optarg, NULL, 16);
        break;
      case 'a':
        res.accent_color = strtol(optarg, NULL, 16);
        break;
      case 'q':
        res.active_modes = optarg;
        res.nmodes = strlen(optarg);
        break;
      case 'p':
        parse_pulse(&res, optarg);
        break;
      case 'w':
        res.white_ramp = false;
        break;
      case 'h':
        res.hue_rotation_speed = atof(optarg)*FPS;
        break;
      case 'n':
        parse_noise(&res, optarg);
        break;
      case 'b':
        parse_block(&res, optarg);
        break;
    }
  }
}

void wait_frame(int frames_per_second) {
  double delay = 1000000000./frames_per_second;
  struct timespec wait = {0,delay};
  nanosleep(&wait,NULL);
}

void apply_ramp(uint32_t * fb, uint32_t clock, strip_config * leds,
                mode_config config) {
  if (config.white_ramp) {
    ramp(fb, clock, leds, 0xFFFFFF, 0x000000, config.ramp_speed);
  } else {
    ramp(fb, clock, leds, config.main_color, config.accent_color, config.ramp_speed);
  }
}

void apply_mode(uint32_t * fb, char mode, mode_config config,
                strip_config * leds, double clock) {
  switch(mode) {
    case 'h':
      hue_rotation(fb, (uint32_t)clock, leds, config.hue_rotation_speed);
      break;
    case 's':
      sinusoidal(fb, clock, leds, config.main_color, 
        config.accent_color, config.sinusoid_scale, 
        config.sinusoid_speed);
      break;
    case 'r':
      apply_ramp(fb, (uint32_t)clock, leds, config);
      break;
    case 'p':
      pulse(fb, (uint32_t)clock, leds, config.main_color, config.accent_color,
                      config.pulse_size, config.pulse_speed);
      break;
    case 'n':
      noise_field(fb, (uint32_t)clock, leds, config.noise_scale, config.noise_speed);
      break;
    case 'b':
      color_segments(fb, (uint32_t)clock, leds, config.block_size, defaultPalette, defaultCount, config.block_speed);
      break;
  }
}

bool should_switch(double clock, double last_switch, int fps, int mode_dur) {
  double elapsed = (clock - last_switch) / fps;
  return elapsed >= mode_dur;
}

int running = 1;

void exit_handler(int signum) {
  running = 0;
}

int main(int argc, char ** argv) {
  signal(SIGINT, exit_handler);
  signal(SIGTERM, exit_handler);
  mode_config cfg = parse_args(argc, argv);
  strip_config * config = leds_config(cfg.config_file);
  int w = config->leds_width;
  int h = config->leds_height;
  if (leds_init(config) == 0) {
    fprintf(stderr, "Unable to open memory.");
    return 404;
  };
  int curmode = 0;
  double clock = 0;
  double last_switch = 0;
  printf("Entering main loop...\n");
  while(running) {
    uint32_t * fb = calloc(w*h,sizeof(uint32_t));
    if (should_switch(clock, last_switch, FPS, cfg.seconds_per_mode)) {
      last_switch = clock;
      curmode++;
      curmode %= cfg.nmodes;
      printf("Switching modes...\n");
    }
    apply_mode(fb, cfg.active_modes[curmode], cfg, config, clock);
    leds_draw(config, fb);
    wait_frame(FPS);
    free(fb);
    clock++;
  }
  uint32_t * fb = calloc(w*h,sizeof(uint32_t));
  fill_color(fb, 0, config);
  leds_draw(config, fb);
}
