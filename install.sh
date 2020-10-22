#!/usr/bin/env bash
sudo git clone https://rna.iontank.com/iontank/blinkyblink /opt/ThrowingBagels #todo: replace this with the public repo URL
USER=$(whoami)
sudo chown -R $USER /opt/ThrowingBagels
cd /opt/ThrowingBagels
make

echo "From /opt/ThrowingBagels run scripts/provision to complete the installation."
