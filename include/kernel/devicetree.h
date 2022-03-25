/*
 * Implements support for DTB version 17
 * https://github.com/devicetree-org/devicetree-specification/releases/tag/v0.4-rc1
 */
#ifndef _DEVICETREE_H_
#define _DEVICETREE_H_

#define ATAG_NONE		0
#define ATAG_CORE		0x54410001
#define ATAG_MEM		0x54410002
#define ATAG_VIDEOTEXT	0x54410003
#define ATAG_RAMDISK	0x54410004
#define ATAG_INITRD2	0x54410005
#define ATAG_SERIAL		0x54410006
#define ATAG_REVISION	0x54410007
#define ATAG_VIDEOLFB	0x54410008
#define ATAG_CMDLINE	0x54410009

/*
 * Flattened DeviceTree Layout
 *
 * ╭──────────────────────────╮
 * │     struct fdt_header    │
 * ├──────────────────────────┤
 * │       (free space)       │
 * ├──────────────────────────┤
 * │ memory reservation block │
 * ├──────────────────────────┤
 * │       (free space)       │
 * ├──────────────────────────┤
 * │      structure block     │
 * ├──────────────────────────┤
 * │       (free space)       │
 * ├──────────────────────────┤
 * │      strings block       │
 * ├──────────────────────────┤
 * │       (free space)       │
 * ╰──────────────────────────╯
 * 
 * Lower addresses are at the top
 */

#define DT_MAGIC	0xd00dfeed

#ifdef __ASSEMBLER__

#else

#include <stddef.h>
#include <stdint.h>

/*
 * From https://github.com/devicetree-org/devicetree-specification/releases/tag/v0.4-rc1
 * page 49 section 5.2
 *
 * all values are stored as big endian integers
 */
struct fdt_header {
	uint32_t magic;
	uint32_t totalsize;
	uint32_t off_dt_struct;
	uint32_t off_dt_strings;
	uint32_t off_mem_rsvmap;
	uint32_t version;
	uint32_t last_comp_version;
	uint32_t boot_cpuid_phys;
	uint32_t size_dt_strings;
	uint32_t size_dt_struct;
} __attribute__ ((aligned (8)));

/*
 * Ibid page 50 section 5.3.2
 *
 * all values are stored as big endian integers
 */
struct fdt_reserve_entry {
	uint64_t address;
	uint64_t size;
} __attribute__ ((aligned (8)));

/*
 * Ibid page 51 section 5.4.1
 *
 * all values are stored as big endian integers
 */
#define FDT_BEGIN_NODE		0x00000001
#define FDT_END_NODE		0x00000002
#define FDT_PROP			0x00000003
#define FDT_NOP				0x00000004
#define FDT_END				0x00000009

struct fdt_struct_prop {
	uint32_t len;
	uint32_t nameoff;
} __attribute__ ((aligned (4)));

// consider only supporting device tree
/*
 * Handles ATAGs or device tree, depending on which is present.
 */
void handle_kernel_params(uintptr_t r2);

#endif  // __ASSEMBLER__

#endif  // _DEVICETREE_H_

