# Device Tree Configuration
Included in the distro currently is a device tree file for Linux kernel 4.19-TI. As future kernel releases happen, you may need to update and modify other device tree overlays to retain compatibility with future Linux releases.

## The Key Change
We need to ensure our device tree has a block of shared memory available to us. For kernel 4.19, we do this:

```
shmem@4a300000 {
  reg = <0x4a300000 0x2000>,
        <0x4a302000 0x2000>,
        <0x4a310000 0x3000>,
        <0x80000000 0x100000>;
  reg-names = "dram0",
        "dram1",
        "dram2",
        "ledbuff";
  compatible = "uio";
  symlink = "uio/pru-shmem";
};
```

Currently, this is done by adding the `0x80000000` reg entry, and naming it `ledbuff`. Hopefully, future kernel revisions won't require crazy updates to this.

## Building
You should find the set of overlays in `/opt/source/bb.org-overlays`, or you can clone it [from here](https://github.com/beagleboard/bb.org-overlays). Modify the appropriate overlay for your kernel as above, then `make`. You'll need to copy the resulting `.dbto` into this repo to include it with future distributions.