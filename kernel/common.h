#ifndef _COMMON_H_
#define _COMMON_H_

#define _Q(s) #s
#define Q(s) _Q(s)

#define OSNAME "Cynix"

#ifdef __ASSEMBLER__
/* asm-only */

#else
/* C-only */

#include <stdint.h>

/*
 * Kernel panic. Prints the error code and halts the kernel.
 */
_Noreturn void panic(uint32_t code);

extern const char *osversion;

#endif  // __ASSEMBLER__

#endif  // _COMMON_H_

