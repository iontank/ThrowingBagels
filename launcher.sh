#!/usr/bin/env sh
echo "Setting up GPIO"
sh pin_config.sh > /dev/null 2> /dev/null
PRU0=/sys/class/remoteproc/remoteproc1
TARGET=gen/ledscape.out
echo "Stopping any running PRU firmware"
echo stop | sudo tee ${PRU0}/state > /dev/null 2> /dev/null
echo "Deploying LEDScape firmware"
cp ${TARGET} /lib/firmware/am335x-pru0-fw
echo "Starting LEDScape firmware"
echo am335x-pru0-fw | sudo tee ${PRU0}/firmware > /dev/null 2> /dev/null
echo start | sudo tee ${PRU0}/state > /dev/null
