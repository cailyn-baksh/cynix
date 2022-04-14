#include "common.h"
#include "devicetree.h"
#include "uart.h"

/* macros to swap endianness of integers */
#define REV32(i) asm ( "rev %0,%1" : "+r" (i) )
#define REV64(i) asm (\
		"ldrd r0,r1,[%0]\n"\
		"rev r0,r0\n"\
		"rev r1,r1\n"\
		"strd r1,r0,[%1]"\
		: "+m" (i)\
		: : "r0", "r1"\
	)

void read_devicetree(void *devtree_ptr);

void handle_kernel_params(uintptr_t r2) {
	if (r2 == 0 || *(uint32_t *)(r2+4) == 0x54410001) {
		// ATAGS
		kprintf("ATAGs detected but not supported. Using default configuration\n");
	} else if (*(uint32_t *)(r2+4) == ATAG_CORE) {
		// r2 points to ATAGS
	} else {
		// devicetree
		kprintf("Reading device tree at %p\n", (void *)r2);
		read_devicetree((void *)r2);
	}
}

/*
 * Parse a node inside a flattened device tree.
 *
 * ptr		A pointer to the current position within the device tree. This is
 * 			updated as the tree is traversed
 * Returns the token that terminated parsing
 */
uint32_t fdt_parseStructNode(void *ptr) {
	uint32_t token;
	char *nodeUnitName = (char *)ptr;  // NOT BIG ENDIAN

	while (*ptr && ptr % 4 != 0) ++ptr;  // find end of string

	for (;;) {
		token = *(uint32_t *)ptr;
		ptr += sizeof(uint32_t);
		REV32(token);

		if (token == FDT_BEGIN_NODE) {
			token = fdt_parseStructNode(ptr);

			if (token != FDT_END_NODE) return token;  // we cant parse this
		} else if (token == FDT_PROP) {
			struct fdt_struct_prop *property = (struct fdt_struct_prop *)ptr;

			// swap endianness
			REV32(property->len);
			REV32(property->nameoff);

			// char *name = [STRINGS_OFFSET] + property->nameoff;

			uint8_t *value = (uint8_t *)ptr;
			ptr += property->len;

			// align pointer
			// checks if pointer is divisible by 4 (ptr & 7 == 0b100), and
			// if it isnt then add the difference (4 - ptr & 0b11)
			if (ptr & 7 != 4) ptr += 4 - (ptr & 3);

			// we have the values, now what?
		} else if (token == FDT_NOP) {
			// nothing to do
		} else {
			return token;
		}
	}
}

void read_devicetree(const void *devtree_ptr) {
	void *ptr = devtree_ptr;  // Pointer moved through device tree

	/* Header */
	struct fdt_header *header = (struct fdt_header *)ptr;
	ptr += sizeof(struct fdt_header);

	// swap endianness
	REV32(header->magic);
	REV32(header->totalsize);
	REV32(header->off_dt_struct);
	REV32(header->off_dt_strings);
	REV32(header->off_mem_rsvmap);
	REV32(header->version);
	REV32(header->last_comp_version);
	REV32(header->boot_cpuid_phys);
	REV32(header->size_dt_strings);
	REV32(header->size_dt_struct);

	/* Memory Reservation Block */
	while (ptr < (devtree_ptr + header->off_dt_struct)) {
		// Read entry
		struct fdt_reserve_entry *entry = (struct fdt_reserve_entry *)ptr;
		ptr += sizeof(struct fdt_reserve_entry);

		// Swap endianness
		REV64(entry->address);
		REV64(entry->size);

		// TODO: handle reserved blocks
	}

	/* Structure block */
	uint32_t token;

	char *nodeUnitName = NULL;

	do {
		token = *(uint32_t *)ptr;
		ptr += sizeof(uint32_t);
		REV32(token);

		switch (token) {
			case FDT_BEGIN_NODE:
				break;
			case FDT_END_NODE:
				break;
			case FDT_PROP:
				break;
			default:
				// FDT_NOP and FDT_END dont need handling
		}
	} while (token != FDT_END && ptr < (devtree_ptr + header->off_dt_struct + header->size_dt_struct));
}
