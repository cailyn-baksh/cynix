#include <stdarg.h>
#include <stdint.h>

#define _DONT_DECLARE_MMIO_EXTERNS

#include "kernel/mmio.h"
#include "kernel/utils.h"

uint64_t MMIO_BASE;
volatile unsigned int  __attribute__((aligned(16))) mbox[9] = {
    9*4, 0, 0x38002, 12, 8, 2, 3000000, 0, 0
};

static inline void mmio_write(uint32_t reg, uint32_t data);
static inline uint32_t mmio_read(uint32_t reg);

void init_uart() {
    // Disable UART0
    mmio_write(UART0_CR, 0);

    /* setup pins 14 & 15 */
    mmio_write(GPPUD, 0);
    delay(150);

    mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
    delay(150);

    mmio_write(GPPUDCLK0, 0);

    // clear pending interrupts
    mmio_write(UART0_ICR, 0);

    // set system clock to 3mhz so we can set the baud rate
    unsigned int r = ((unsigned int)(&mbox) & ~0xF) | 8;
    while (mmio_read(MBOX_STATUS) & 0x80000000);
    mmio_write(MBOX_WRITE, r);
    while (mmio_read(MBOX_STATUS) & 0x40000000);

    /*
     * set integer and fractional part of baud rate
     * Divider = UART_CLOCK/(16 * Baud)
     * Fraction part register = (Fractional part * 64) + 0.5
     * Baud = 115200.
     */
    // Divider = 3000000 / (16 * 115200) = 1.627 = ~1.
    mmio_write(UART0_IBRD, 1);
    // Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40.
    mmio_write(UART0_FBRD, 40);

    // enable FIFO & 8 bit transmission
    mmio_write(UART0_LCRH, 0b1110000);

    // masc interrupts
    mmio_write(UART0_IMSC, 0b11111110010);

    // enable UART0
    mmio_write(UART0_CR, 0b1100000001);
}

void k_uart_puts(char *str) {
    for (; *str; ++str) {
        while (mmio_read(UART0_FR) & (1 << 5));  // wait for UART to be ready
        mmio_write(UART0_DR, *str);
    }
}

unsigned char k_uart_getc() {
    while (mmio_read(UART0_FR) & 0b10000);
    return mmio_read(UART0_DR);
}

static inline void mmio_write(uint32_t reg, uint32_t data) {
    *(volatile uint32_t*)(MMIO_BASE + reg) = data;
}

static inline uint32_t mmio_read(uint32_t reg) {
    return *(volatile uint32_t*)(MMIO_BASE + reg);
}
