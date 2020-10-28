# Scripts
These are utility scripts which are used as part of the setup process.

* pin_layout.txt: a mapping of LED channel, gpio-bank-and-pin (ex: `gpio2[3]`), physical header (ex: `P8_09`), and kernel ID
* pin-setter: a shell script which sets the correct GPIO settings on a single pin, based on its args
* pin-config: a shell script which reads `pin_layout.txt` and uses `xargs` to set all the target pins quickly (accepts alternate input files as an argument)
* sane-pins: a utility script useful for debugging; it ensures your `pin_layout.txt` accurately maps physical headers to kernel IDs
* setup-services: installs the service definition files
* uEnv.txt: an example of what your uEnv.txt should look like. Not actually used as anything but reference.