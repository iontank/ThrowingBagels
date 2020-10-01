# ThrowingBagels
**ThrowingBagels** is a modernization of the [LEDScape](https://github.com/osresearch/LEDscape) software package. ThrowingBagels a LED driver software package for the ~~bagelbone~~ BeagleBone Black, which drives LEDs from UDP packets you yeet at it over the network, or from your own custom software.

You can use ThrowingBagels to drive up to 32 channels of individual LED strips, of whatever length you can drive power to. This should work on any BeagleBone Black, running Debian Buster.

**TODO**: example PCB will be included

## Installing
Clone the repo onto a BeagleBone. You should be able to run `make` on a fresh BeagleBone Black, running Debian Buster, and it will build all the required software components.

**TODO**: example service files will be included, to run this as a service

## Included Application
### PRU Firmware
The main driver of everything is the PRU Firmware, `ledscape.asm`. This is what drives the GPIO channels to activate the LEDs.

### Demo Applications
For the purposes of testing, iterating, and diagnosing hardware issues, there are several included demo applications.

* `bin/smoke`(`src/demos/smoke_test.c`): a simple smoke test application which sets the LEDs to a "soft-white" for ten seconds, and then back to black. It is the "hello world" of ThrowingBagels applications.
* `bin/pru_status`(`src/ledscape/pru_status.c`): a simple tool which checks the status of the PRU firmware.
* `bin/multidemo`(`src/demos/multidemo.c`): a complex and highly configurable suite of animations which run right on the BeagleBone, useful for demonstrating lighting effects without network connectivity. Great "pitch tool" to prove hardware concepts.

### LED UDP RX
The primary application, and what drives the core use-case of this software stack, is the `bin/led-udp-rx` tool (`src/net/udp-rx.c`). This tool listens on a port (default: `9999`) for frame data. This allows a powerful "render machine" to run elsewhere on the network, and send frame data to an arbitrary number of ThrowingBagels instances.

## Running
Included is a `launcher` script. This script configures the GPIO, deploys the firmware, and then, optionally, launches a requested binary. E.g., `./launcher bin/led-udp-rx`, would launch the LED UDP RX tool on port 9999, using `leds.config` as the source for our led configuration.

### On Configuration
Many of the tools, like `led-udp-rx` and `multidemo`, expect an LED configuration to be supplied. By default, they check `leds.config`, but they both take a command line flag to change that. The file MUST be a single line, in this format:

`number of channels,length of strips,number of bytes`

e.g.:

`32,100,4`: this would drive 32 channels of output, for strips 100 pixels long, and expects each LED to need 4 bytes (an RGBW LED). For LED UDP RX, this also sets the expected packet size- each packet should contain 50 pixels for all 32 channels (see [the UDP RX Readme](src/net/README.md) for more details).


## Your own ThrowingBagles software
To build your own application running on the BeagleBone, you must include two files. First, you need to include `ledstrips.h` into your C code, and you must include a LUT: either `gamma8.h` (recommended), `linear.h`, or your own custom LUT.

