#!/bin/bash
if [[ -e /tmp/pins_hot ]]
then
  echo "    GPIO Already configured";
else
echo "Setting up GPIO"
config-pin $(cat /sys/class/gpio/gpio2/label) gpio
echo out | sudo tee /sys/class/gpio/gpio2/direction
config-pin $(cat /sys/class/gpio/gpio3/label) gpio
echo out | sudo tee /sys/class/gpio/gpio3/direction
config-pin $(cat /sys/class/gpio/gpio4/label) gpio
echo out | sudo tee /sys/class/gpio/gpio4/direction
config-pin $(cat /sys/class/gpio/gpio5/label) gpio
echo out | sudo tee /sys/class/gpio/gpio5/direction
config-pin $(cat /sys/class/gpio/gpio7/label) gpio
echo out | sudo tee /sys/class/gpio/gpio7/direction
config-pin $(cat /sys/class/gpio/gpio8/label) gpio
echo out | sudo tee /sys/class/gpio/gpio8/direction
config-pin $(cat /sys/class/gpio/gpio9/label) gpio
echo out | sudo tee /sys/class/gpio/gpio9/direction
config-pin $(cat /sys/class/gpio/gpio10/label) gpio
echo out | sudo tee /sys/class/gpio/gpio10/direction
config-pin $(cat /sys/class/gpio/gpio11/label) gpio
echo out | sudo tee /sys/class/gpio/gpio11/direction
config-pin $(cat /sys/class/gpio/gpio14/label) gpio
echo out | sudo tee /sys/class/gpio/gpio14/direction
config-pin $(cat /sys/class/gpio/gpio15/label) gpio
echo out | sudo tee /sys/class/gpio/gpio15/direction
config-pin $(cat /sys/class/gpio/gpio20/label) gpio
echo out | sudo tee /sys/class/gpio/gpio20/direction
config-pin $(cat /sys/class/gpio/gpio22/label) gpio
echo out | sudo tee /sys/class/gpio/gpio22/direction
config-pin $(cat /sys/class/gpio/gpio23/label) gpio
echo out | sudo tee /sys/class/gpio/gpio23/direction
config-pin $(cat /sys/class/gpio/gpio26/label) gpio
echo out | sudo tee /sys/class/gpio/gpio26/direction
config-pin $(cat /sys/class/gpio/gpio27/label) gpio
echo out | sudo tee /sys/class/gpio/gpio27/direction
config-pin $(cat /sys/class/gpio/gpio30/label) gpio
echo out | sudo tee /sys/class/gpio/gpio30/direction
config-pin $(cat /sys/class/gpio/gpio31/label) gpio
echo out | sudo tee /sys/class/gpio/gpio31/direction
config-pin $(cat /sys/class/gpio/gpio48/label) gpio
echo out | sudo tee /sys/class/gpio/gpio48/direction
config-pin $(cat /sys/class/gpio/gpio49/label) gpio
echo out | sudo tee /sys/class/gpio/gpio49/direction
config-pin $(cat /sys/class/gpio/gpio50/label) gpio
echo out | sudo tee /sys/class/gpio/gpio50/direction
config-pin $(cat /sys/class/gpio/gpio65/label) gpio
echo out | sudo tee /sys/class/gpio/gpio65/direction
config-pin $(cat /sys/class/gpio/gpio66/label) gpio
echo out | sudo tee /sys/class/gpio/gpio66/direction
config-pin $(cat /sys/class/gpio/gpio67/label) gpio
echo out | sudo tee /sys/class/gpio/gpio67/direction
config-pin $(cat /sys/class/gpio/gpio68/label) gpio
echo out | sudo tee /sys/class/gpio/gpio68/direction
config-pin $(cat /sys/class/gpio/gpio69/label) gpio
echo out | sudo tee /sys/class/gpio/gpio69/direction
config-pin $(cat /sys/class/gpio/gpio70/label) gpio
echo out | sudo tee /sys/class/gpio/gpio70/direction
config-pin $(cat /sys/class/gpio/gpio71/label) gpio
echo out | sudo tee /sys/class/gpio/gpio71/direction
config-pin $(cat /sys/class/gpio/gpio72/label) gpio
echo out | sudo tee /sys/class/gpio/gpio72/direction
config-pin $(cat /sys/class/gpio/gpio73/label) gpio
echo out | sudo tee /sys/class/gpio/gpio73/direction
config-pin $(cat /sys/class/gpio/gpio74/label) gpio
echo out | sudo tee /sys/class/gpio/gpio74/direction
config-pin $(cat /sys/class/gpio/gpio75/label) gpio
echo out | sudo tee /sys/class/gpio/gpio75/direction
config-pin $(cat /sys/class/gpio/gpio76/label) gpio
echo out | sudo tee /sys/class/gpio/gpio76/direction
config-pin $(cat /sys/class/gpio/gpio77/label) gpio
echo out | sudo tee /sys/class/gpio/gpio77/direction
config-pin $(cat /sys/class/gpio/gpio78/label) gpio
echo out | sudo tee /sys/class/gpio/gpio78/direction
config-pin $(cat /sys/class/gpio/gpio79/label) gpio
echo out | sudo tee /sys/class/gpio/gpio79/direction
config-pin $(cat /sys/class/gpio/gpio80/label) gpio
echo out | sudo tee /sys/class/gpio/gpio80/direction
config-pin $(cat /sys/class/gpio/gpio81/label) gpio
echo out | sudo tee /sys/class/gpio/gpio81/direction
config-pin $(cat /sys/class/gpio/gpio86/label) gpio
echo out | sudo tee /sys/class/gpio/gpio86/direction
config-pin $(cat /sys/class/gpio/gpio87/label) gpio
echo out | sudo tee /sys/class/gpio/gpio87/direction
config-pin $(cat /sys/class/gpio/gpio88/label) gpio
echo out | sudo tee /sys/class/gpio/gpio88/direction
config-pin $(cat /sys/class/gpio/gpio89/label) gpio
echo out | sudo tee /sys/class/gpio/gpio89/direction
config-pin $(cat /sys/class/gpio/gpio110/label) gpio
echo out | sudo tee /sys/class/gpio/gpio110/direction
config-pin $(cat /sys/class/gpio/gpio111/label) gpio
echo out | sudo tee /sys/class/gpio/gpio111/direction
config-pin $(cat /sys/class/gpio/gpio112/label) gpio
echo out | sudo tee /sys/class/gpio/gpio112/direction
config-pin $(cat /sys/class/gpio/gpio113/label) gpio
echo out | sudo tee /sys/class/gpio/gpio113/direction
config-pin $(cat /sys/class/gpio/gpio115/label) gpio
echo out | sudo tee /sys/class/gpio/gpio115/direction
config-pin $(cat /sys/class/gpio/gpio117/label) gpio
echo out | sudo tee /sys/class/gpio/gpio117/direction

touch /tmp/pins_hot
fi