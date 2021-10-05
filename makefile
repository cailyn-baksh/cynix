OBJS = $(patsubst kernel/%.S,bin/%.o,$(wildcard kernel/*.S)) $(patsubst kernel/%.c,bin/%.o,$(wildcard kernel/*.c)) $(patsubst kernel/%.cpp,bin/%.o,$(wildcard kernel/*.cpp))
NAME = cynix

bin/%.o: kernel/%.S
	aarch64-elf-as -c -o $@ $^

bin/%.o: kernel/%.c
	aarch64-elf-gcc -ffreestanding -O2 -Wall -Wextra -c -o $@ $^

bin/%.o: kernel/%.cpp
	aarch64-elf-g++ -ffreestanding -fno-exceptions -fno-rtti -O2 -Wall -Wextra -c -o $@ $^

all: $(KERNEL_OBJS)
	aarch64-elf-g++ -T linker.ld -ffreestanding -O2 -nostdlib -c -o bin/$(NAME).bin $^

clean:
	find bin/* -type f -delete
