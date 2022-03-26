#include <stddef.h>
#include <stdint.h>

#include "kernel/board.h"
#include "kernel/mailbox.h"

uint32_t mbox_read(uint8_t channel) {
	for (;;) {
		while ((MMIO_READ(uint32_t, MAIL0_STATUS) & MAIL_EMPTY) != 0);  // Wait for data

		// Read a word from the register
		uint32_t data = MMIO_READ(uint32_t, MAIL0_READ);
		uint8_t channelRead = data & 0xF;  // low 4 bits of data are channel
		data >>= 4;  // high 28 bits are data

		if (channelRead == channel) return data;
	}
}

void mbox_write(uint8_t channel, uint32_t value) {
	while ((MMIO_READ(uint32_t, MAIL1_STATUS) & MAIL_FULL) != 0);  // Wait for space

	MMIO_WRITE(uint32_t, MAIL1_WRITE, value);
}
