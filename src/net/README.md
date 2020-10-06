# LED UDP RX
This program is the LED UDP Receiver, which accepts RGB frame data via a UDP packet, and then dumps it to the LEDS.

## Running
Use either the provided service files, or `launcher bin/led-udp-rx`.

### Options
* `-c pathtofile`: by default, it looks for `leds.config` in the launch directory, but this lets you point to another config file.
* `-p port`: by default, runs on port 9999, but this lets you change the port
* `-t timeout`: by default 1000; if it doesn't receive a packet in `timeout` milliseconds, it fails back to a demo mode. 

### Config File
Many of the tools, like `led-udp-rx` and `multidemo`, expect an LED configuration to be supplied. By default, they check `leds.config`, but they both take a command line flag to change that. The file MUST be a single line, in this format:

`number of channels,length of strips,number of bytes`

e.g.:

`32,100,4`: this would drive 32 channels of output, for strips 100 pixels long, and expects each LED to need 4 bytes (an RGBW LED). For LED UDP RX, this also sets the expected packet size- each packet should contain 50 pixels for all 32 channels (see [the UDP RX Readme](src/net/README.md) for more details).

## Packet Format
LED UDP RX expects to receive two network packets per frame. The packet format is extremely simple:

The first byte needs to be the "index" of this packet- 0 means it contains the first half of the pixels for every strip, 1 means it contains the second half. LEDS refresh only AFTER a packet with an index of 1 is received.

The rest of the data is 24-bit raw RGB data.

## Sample Network Client
[A Python sample client](../../python/) demonstrates how to send RGB data from another application to one or more Beaglebones.

## Example Network Layout
<pre>
                                          +------------+    +--------------------------+
+---------+                   +---------->+  bagelbox  +--->+up to 32 channels of LEDs |
|         |     udp           |           +------------+    +--------------------------+
| Render  +-------------------+
| Machine |                   |           +------------+    +--------------------------+
|         |                   +---------->+  bagelbox  +--->+up to 32 channels of LEDs |
+---------+                   |           +------------+    +--------------------------+
                              |
                              |           +------------+    +--------------------------+
                              +---------->+  bagelbox  +--->+up to 32 channels of LEDs |
                              |           +------------+    +--------------------------+
                              |
                              |           +------------+    +--------------------------+
                              +---------->+  bagelbox  +--->+up to 32 channels of LEDs |
                                          +------------+    +--------------------------+
</pre>

The "Render Machine" may be pretty much *anything*. We've built frame generators in Unity, Python, C/C++, Processing. Anything that can construct an RGB frame and send it across the network. See the [Python sample](../../python) to get a sense of how this works.

The render machine must generate a frame using any technique you like, and then assign segments of that frame to Beaglebones, slice out the relevant segments, and then send them over the network via UDP.

Bagelboxes are Beaglebones running the ThrowingBagels LED-UDP-RX. This allows for synchronized drawing across an arbitrary number of Beaglebones, with an arbitrary number of LEDs. The maximum size of your LED installation is limited only by the performance of your renderer.