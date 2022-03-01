#ifndef _KPARAMS_H_
#define _KPARAMS_H_

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

#ifdef __ASSEMBLER__

#else

#include <stddef.h>
#include <stdint.h>
 
/*
 * Handles ATAGs or device tree, depending on which is present.
 */
void handle_kernel_params(uintptr_t r2);

#endif  // __ASSEMBLER__

#endif  // _KPARAMS_H_

