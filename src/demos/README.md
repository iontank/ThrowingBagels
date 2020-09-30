# Demo Programs
A collection of demo programs useful for testing/diagnosing/debugging LEDs

## Smoke Test
`smoke` is the simplest of all the programs. It sends white to the LEDs, then after a short delay, sends black. It does not use any drawing functions or complex logic, it is literally the simplest way to send data to the LEDs. 

To run from the ThrowingBagels install dir: `./launcher bin/smoke`

## Multidemo
`multidemo` is a much more complex demo program, suitable for making full hardware testbeds. It supports a huge number of configuration flags. Run `bin/multidemo --help` for more info.

To launch a full configuration, from the ThrowingBagels install dir: `./launcher bin/multidemo $ARGS`.