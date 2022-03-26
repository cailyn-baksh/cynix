NAME = cynix
VERSION := $(shell cat version)
BUILDNUM = $(shell ./buildnum.sh)
SRCS = $(filter-out %.swp %.inc %.h,$(wildcard kernel/*))
OBJS = $(addsuffix .o,$(patsubst %,bin/%,$(SRCS)))
INCLUDES = include/
TOOLCHAIN = arm-none-eabi
CFLAGS = -march=armv6k -mabi=aapcs -ffreestanding -nostdlib -nostartfiles -O2 -Wall $(addprefix -I,$(INCLUDES))
CSTD = gnu17

build: $(OBJS)
	rm -f bin/$(NAME)-$(VERSION)-*.elf
	$(TOOLCHAIN)-ld -nostdlib -T linker.ld -o bin/$(NAME)-$(VERSION)-$(BUILDNUM).elf $^
	$(TOOLCHAIN)-objcopy bin/$(NAME)-$(VERSION)-$(BUILDNUM).elf -O binary bin/kernel.img
	./buildnum.sh update

bin/kernel/%.c.o: kernel/%.c
	$(TOOLCHAIN)-gcc $(CFLAGS) -std=$(CSTD) -c -o $@ $<

bin/kernel/version.c.o:: version
	./buildnum.sh 0
	sleep 0.2

bin/kernel/version.c.o:: kernel/version.c FORCE
	$(TOOLCHAIN)-gcc $(CFLAGS) -DOSVERSION=$(VERSION)-$(BUILDNUM) -c -o $@ $<

bin/kernel/%.S.o: kernel/%.S
	$(TOOLCHAIN)-gcc $(CFLAGS) -c -o $@ $<

setup:
	mkdir -p bin
	mkdir -p bin/kernel
	mkdir -p sysroot

clean:
	find bin/* -type f -delete

rebuild: clean build

qemu-rpi2:
	qemu-system-arm -M raspi2 -serial null -serial stdio -kernel bin/$(NAME)-*.elf

.PHONY: build setup clean rebuild test FORCE
FORCE:

