#include <stdint.h>
#include <limits.h>

#include "common.h"
#include "board.h"
#include "kparams.h"
#include "uart.h"

#define NOEXTERN
#include "errno.h"
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

	kprintf("Booting %s v%s\r\n", OSNAME, osversion);
	kprintf("Board %p %p\r\n", board, NULL);

	handle_kernel_params(r2);

	return 0;
}
