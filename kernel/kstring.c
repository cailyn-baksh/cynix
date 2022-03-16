#include "kstring.h"

size_t kstrlen(const char *s) {
	size_t len = 0;
	while (s[len]) ++len;
	return len;
}

