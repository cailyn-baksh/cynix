/*
 * Kernel error reporting.
 */

#ifndef _ERRNO_H_
#define _ERRNO_H_

#define ENONE	0x0  // No error

#define EDOM	0x1  // Parameter outside function's domain
#define ERANGE	0x2  // Result outside function's range
#define EILSEQ	0x3  // Illegal byte sequence

#ifdef __ASSEMBLER__

#else

#include <stdint.h>

#ifndef NOEXTERN
extern uint32_t errno;
#endif  // NOEXTERN
#endif  // __ASSEMBLER__
#endif  // _ERRNO_H_

