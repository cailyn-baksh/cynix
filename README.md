# Cynix Kernel

## Building

Building this kernel requires binutils and a GCC cross compiler built for the aarch64-elf target following [this tutorial](https://wiki.osdev.org/GCC_Cross-Compiler). You should pass `--enable-languages=c,c++` to the GCC `configure` script. Make sure to add the install location to your `$PATH` if it isn't already.

When you first clone this repo, the `bin/` and `sysroot/` directory structures will not be present. Run `make setup` to create this structure.

`make` will compile, assemble, and link the code into `sysroot/kernel8.img`. To remove all build files run `make clean`.

## Testing

To test this code on qemu, install `qemu-system-aarch64`, then run the following command

```
qemu-system-aarch64 -M raspi3b -serial null -serial stdio -kernel bin/cynix.elf
```

## Acknowledgements

 - [OSDev](https://wiki.osdev.org/)
 - [bzt's *Bare Metal Programming on Raspberry Pi 3* tutorial](https://github.com/bztsrc/raspi3-tutorial)
