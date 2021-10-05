#include <stdint.h>

uint64_t MMIO_BASE;

void kernel_main(uint64_t dtb_ptr32, uint64_t x1, uint64_t x2, uint64_t x3) {
    if (MMIO_BASE == 0) {
        // board type was not detected
    }
}
