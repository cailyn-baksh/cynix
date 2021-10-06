#include <stdint.h>

#include "kernel/mmio.h"

void kernel_main(uint64_t dtb_ptr32, uint64_t x1, uint64_t x2, uint64_t x3) {
    init_uart();

    k_uart_puts("Hello World!\n");

    while (1) {
        k_uart_getc();
    }
}
