# Simple Frame Sender

This is a simple Python-based Frame Sender. It should be compatible with all Python versions 2.7+ (though we have only tested on 3.6+, it's a demo program, don't get greedy). It depends on NumPy and nothing else.

## Running
`python color.py color.json --speed 4`
The JSON configuration file contains information to describe the layout of your frames, overall. Speed is how many seconds it should take to do a full rotation.

### Configuration
The `color.json` file is an included example which demonstrates the layout. The key thing to note about this file you you specify an overall `frameSize`, which is the total render area, in LEDs, then each individual beaglebone is assigned a *slice* out of that range, a rectangle in the X/Y/W/H layout.

NB: this simple program assumes that all the strips run in the same direction, e.g. that X AXIS is always the individual GPIO channels on the Beaglebone, and Y AXIS is always the length down the strips. If your layout doesn't look like that, you may need to build your own custom mappings.

Each `target` needs a `hostname` (either the network name or the IP address), a `port`, then its `x`, `y`, `w`, `h` slice.