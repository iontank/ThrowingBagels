#!/usr/bin/env bash
sudo git clone https://rna.iontank.com/iontank/blinkyblink /opt/ThrowingBagels #todo: replace this with the public repo URL
USER=$(whoami)
sudo chown -R $USER /opt/ThrowingBagels
cd /opt/ThrowingBagels
make

echo "Installing the bagel uBoot config."
sudo sed -i.bak "s/#disable_uboot_overlay_video=1/disable_uboot_overlay_video=1/g" /boot/uEnv.txt
sudo sed -i.bak "s/#disable_uboot_overlay_audio=1/disable_uboot_overlay_audio=1/g" /boot/uEnv.txt

echo "Updating the device tree."
sudo cp dts/*.dtbo /lib/firmware

echo "This script assumes that your uEnv.txt enables the PRU, which should be the default. Ensure that a uboot_overlay_pru is set."
echo "If not, manually change it."

echo "Installing services"
sudo scripts/setup_services

echo "Throwing Bagels has been installed. Services are installed, but not enabled. Check the READMe for more."
echo "Please reboot before trying the included software."
echo "After a reboot, you can use systemctl to enable/start either multidemo or led-udp"

echo "From /opt/ThrowingBagels run scripts/provision to configure your installation."
