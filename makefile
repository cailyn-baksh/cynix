# Build aarch64-elf cross compiler following https://wiki.osdev.org/GCC_Cross-Compiler
OBJS = $(addsuffix .o,$(patsubst %,bin/%,$(wildcard kernel/*)))
NAME = cynix
INCLUDES = include/

bin/%.S.o: %.S
	aarch64-elf-as -c -o $@ $^

bin/%.c.o: %.c
	aarch64-elf-gcc $(addprefix -I,$(INCLUDES)) -ffreestanding -O2 -Wall -Wextra -c -o $@ $^

bin/%.cpp.o: %.cpp
	aarch64-elf-g++ $(addprefix -I,$(INCLUDES)) -ffreestanding -fno-exceptions -fno-rtti -O2 -Wall -Wextra -c -o $@ $^

all: $(OBJS)
	aarch64-elf-g++ -T linker.ld -ffreestanding -O2 -nostdlib -o bin/$(NAME).bin $^

clean:
	find bin/* -type f -delete
