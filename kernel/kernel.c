#include <stdint.h>
#include <limits.h>

#include "kernel/common.h"
#include "kernel/board.h"
#include "kernel/kparams.h"
#include "kernel/uart.h"

#define NOEXTERN
#include "kernel/kerrno.h"
#undef NOEXTERN

uint32_t errno = ENONE;

uint32_t kernel_main(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3) {
	switch ((r3 >> 4) & 0xFFF) {
		case 0xB76:
			board = &boards[0];
			break;
		case 0xC07:
			board = &boards[1];
			break;
		case 0xD03:
			board = &boards[2];
			break;
		case 0xD08:
			board = &boards[3];
			break;
	}

	init_uart1();

	//panic(5);

	kprintf("Hello World!\r\n");
	kprintf("char: '%-15c'\r\n", 'h');
	kprintf("string: '%-*s'\r\n", 10, "abc");
	kprintf("hexl: %-#8x\r\n", 0xacab);
	kprintf("hexu: %#08X\r\n", 0);
	kprintf("oct: %o\r\n", 8);
	kprintf("bin: %#08b\r\n", 10);

	//kprintf("Booting %s v%s\r\n", OSNAME, osversion);
	//kprintf("Board %p %p\r\n", board, NULL);

	handle_kernel_params(r2);

	return 0;
}
