/*
 * Implementation of I/O functions for both PL011 UART and mini UART
 */

#include "kernel/board.h"

#include "kernel/hw/auxiliary.h"
#include "kernel/hw/gpio.h"
#include "kernel/hw/mailbox.h"

void mini_uart_init() {
	// enable mini uart
	MMIO(AUX_ENABLES) |= AUX_MU_ENABLED;

	// Disable Rx, Tx, RTS and CTS auto-flow control.
	MMIO(AUX_MU_CNTL_REG) = 0;
	// Set 8-bit mode
	MMIO(AUX_MU_LCR_REG) = AUX_MU_LCR_8_BIT;
	// Set RTS line high
	MMIO(AUX_MU_MCR_REG) = 0;
	// Disable Tx/Rx interrupts
	MMIO(AUX_MU_IER_REG) = 0;
	// Clear Tx/Rx FIFOs
	MMIO(AUX_MU_IIR_REG) = AUX_MU_IIR_CLEAR_RX_FIFO | AUX_MU_IIR_CLEAR_TX_FIFO | AUX_MU_IIR_FIFO_ENABLE;
	// Set baudrate
	MMIO(AUX_MU_BAUD_REG) = 270;

	// Map UART1 to GPIO pins 14 and 15
	MMIO(GPFSEL1) &= ~(0x3F << 12);  // clear bits 12 to 17
	MMIO(GPFSEL1) |= (2 << 12) | (2 << 15);  // select functions

	// Disable pullup/down on pins 14 and 15
	if (board->id == BOARD_RPI4_ID) {
		// BCM2711 lets us control pins directly
		MMIO(GPIO_PUP_PDN_CNTRL_REG0) &= ~BITS(0b1111, 28);  // Set pin 14 and 15 controls to zero
	} else {
		// Set control signal (disable PUD)
		MMIO(GPPUD) = GPPUD_OFF;

		// Wait 150 cycles for control signal setup
		for (int i=0; i < 150; ++i) asm("nop");

		MMIO(GPPUDCLK0) = BIT(14) | BIT(15);  // Assert clock on lines 14 and 15

		// Provide hold time for control signal
		for (int i=0; i < 150; ++i) asm("nop");

		// Remove the clock
		MMIO(GPPUDCLK0) = 0;
	}

	// Enable Rx, Tx
	MMIO(AUX_MU_CNTL_REG) = AUX_MU_CNTL_RX_ENABLE | AUX_MU_CNTL_TX_ENABLE;
}

void uart1_putc(char c) {
	while (!(AUX_MU_LSR_REG & 0x20)) asm("nop");

	MMIO(AUX_MU_IO_REG) = c;
}

void pl011_init_uart(void *uart) {
	PL011_REG(uart, PL011_UART_CR) = 0;  // disable uart

	
}
