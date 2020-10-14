#!/usr/bin/env bash
read -p "Press [Enter] to provision this Bagelbox for network rendering."
echo "What hostname would you like this Bagelbox to use? "
read -p "[HOST] > "  hostval
echo "What is the LED config as:"
read -p "[WIDTH] > " w
read -p "[HEIGHT] > " h
read -p "[BYTES] > " b

if [[ $w -gt 32 ]]
then
  echo "Too many channels- 32 is the max. No changes have been made."
  exit
fi

case $b in
  3)
    ;;
  4)
    ;;
  *)
    echo "Num bytes must be 3 or 4 (RGB or RGBW). No changes have been made."
    exit
    ;;
esac

if [[ -z $hostval ]]
then
  echo "You didn't supply a host name. Not setting one. Re-run provision.sh or change the hostname yourself."
else
  echo "Setting hostname to $hostval."
  echo $hostval | sudo tee /etc/hostname
fi

if [[ -z $w ]] || [[ -z $h ]] || [[ -z $b ]]
then
  echo "You didn't supply all LEDs dimensions. Not setting config. Run provision.sh again, or change the leds.config file yourself."
else
  echo "Setting leds.config to $w,$h,$b"
  echo "$w,$h,$b" > leds.config
fi

echo "Backing up your original uBoot config to `/boot/uEnv.txt.old`"
sudo cp /boot/uEnv.txt /boot/uEnv.txt.old
echo "Installing the bagel uBoot config."
sudo cp scripts/uEnv.txt /boot/uEnv.txt



