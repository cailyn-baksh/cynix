#ifndef _COMMON_H_
#define _COMMON_H_

#ifndef __GNUC__
#error "Cynix uses GNU language extensions and requires GCC to compile"
#endif  // __GNUC__

#define _Q(s) #s
#define Q(s) _Q(s)

#define OSNAME "Cynix"

/* Create a mask for a specific bit */
#define BIT(n) (1<<n)
/* Create a mask for a series of bits. b is the bits and n is the index of the LSB */
#define BITS(b,n) (b<<n)

#ifdef __ASSEMBLER__
/* asm-only */

#else
/* C-only */

#include <stdint.h>

/*
 * Kernel panic. Prints the error code and halts the kernel. If a panic occurs
 * before mini uart has been initialized the error code will still be stored.
 */
_Noreturn void panic(uint32_t code);

extern const char *osversion;

#endif  // __ASSEMBLER__

#endif  // _COMMON_H_

