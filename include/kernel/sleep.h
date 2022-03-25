#ifndef _SLEEP_H_
#define _SLEEP_H_

#ifdef __ASSEMBLER__

#else

#include <stdint.h>

/*
 * Sleeps for n cycles
 */
void sleep_cycles(uint32_t cycles);

#endif  // __ASSEMBLER__

#endif  // _SLEEP_H_
