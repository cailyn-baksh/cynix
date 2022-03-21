#include "common.h"
#include "kparams.h"

void read_atags(void *atags_ptr);

void read_devicetree(void *devtree_ptr);

void handle_kernel_params(uintptr_t r2) {
	if (r2 == 0) {
		// Device tree disabled
		if (*(uint32_t *)0x104 == ATAG_CORE) {
			// ATAGs at 0x100
			read_atags((void *)0x100);
		}
	} else if (*(uint32_t *)(r2+4) == ATAG_CORE) {
		// r2 points to ATAGS
		read_atags((void *)r2);
	} else {
		// devicetree
		read_devicetree((void *)r2);
	}
}

void read_atags(void *atags_ptr) {
	// Read first tag
	uint32_t size = *(uint32_t *)atags_ptr;
	uint32_t tag = *(uint32_t *)(atags_ptr+4);
	atags_ptr += 8;

	while (tag != ATAG_NONE) {
		// Handle this atag
		switch (tag) {
			case ATAG_CORE:
				break;
			case ATAG_MEM:
				break;
			case ATAG_VIDEOTEXT:
				break;
			case ATAG_RAMDISK:
				break;
			case ATAG_INITRD2:
				break;
			case ATAG_SERIAL:
				break;
			case ATAG_REVISION:
				break;
			case ATAG_VIDEOLFB:
				break;
			case ATAG_CMDLINE:
				break;
		}

		// Read next atag
		size = *(uint32_t *)atags_ptr;
		tag = *(uint32_t *)(atags_ptr+4);
		atags_ptr += 8;
	}
}

void read_devicetree(void *devtree_ptr) {

}
