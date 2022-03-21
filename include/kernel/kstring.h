/*
 * Limited implementation of string.h for within the kernel.
 */
#ifndef _KSTRING_H_
#define _KSTRING_H_

#include <stddef.h>

size_t kstrlen(const char *s);

#endif  // _KSTRING_H_

