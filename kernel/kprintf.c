#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "kernel/kstring.h"
#include "kernel/uart.h"

extern void doubledabble_shiftregister(void *reg, uint32_t size);

/*
 * Gets an integer argument from varargs. Helper function for kprintf.
 *
 * value	A buffer to place the value in
 * args		The varargs object to read from
 * length	The printf length specifier.
 * Returns the size of the type read
 */
static size_t getIntArg(intmax_t *value, va_list *args, char length) {
	size_t size;

	switch (length) {
		case 'H':
			*value = (char)va_arg(*args, int);
			size = sizeof(char);
			break;
		case 'h':
			*value = (short)va_arg(*args, int);
			size = sizeof(short);
			break;
		case 'l':
			*value = va_arg(*args, long);
			size = sizeof(long);
			break;
		case 'q':
			*value = va_arg(*args, long long);
			size = sizeof(long long);
			break;
		case 'z':
			*value = va_arg(*args, size_t);
			size = sizeof(size_t);
			break;
		case 'j':
			*value = va_arg(*args, intmax_t);
			size = sizeof(intmax_t);
			break;
		case 't':
			*value = va_arg(*args, ptrdiff_t);
			size = sizeof(ptrdiff_t);
			break;
		default:
			*value = va_arg(*args, int);
			size = sizeof(int);
	}

	return size;
}

/*
 * Print a string with padding applied. Helper function for kprintf.
 *
 * str		The string to print. Does not need to be null terminated
 * width	The width of the string to print
 * prefix	A prefix to put at the start of the string, after regular padding but
 *			before zero padding. This must be null terminated, and may be NULL
 * width	The minimum number of characters to print
 * left		Whether or not to left align the field
 * zero		Whether to pad with zeroes or not
 *
 * Returns number of chars printed
 */
static size_t printWidth(const char *str, int length, const char *prefix, int width, bool left, bool zero) {
	const char padding = zero ? '0' : ' ';
	size_t charsPrinted = 0;

	if (left && !zero) {
		if (prefix != NULL) {
			// print prefix
			while (*prefix) {
				uart1_putc(*(prefix++));
				++charsPrinted;
			}
		}

		// Print string first
		for (int i=0; i < length; ++i, ++charsPrinted) {
			uart1_putc(str[i]);
		}
		// then print padding
		for (int i=0; i < width - length; ++i, ++charsPrinted) {
			uart1_putc(padding);
		}
	} else {
		if (zero && prefix != NULL) {
			// Print prefix before zero padding
			while (*prefix) {
				uart1_putc(*(prefix++));
				++charsPrinted;
			}
		}

		// Print padding first
		for (int i=0; i < width - length; ++i, ++charsPrinted) {
			uart1_putc(padding);
		}

		if (!zero && prefix != NULL) {
			// Print prefix between padding and string
			while (*prefix) {
				uart1_putc(*(prefix++));
				++charsPrinted;
			}
		}

		// then print string
		for (int i=0; i < length; ++i, ++charsPrinted) {
			uart1_putc(str[i]);
		}
	}

	return charsPrinted;
}

/*
 * Calculate the size in bytes of the required buffer for double dabble
 * 
 * intSize	The size in bytes of the integer the algorithm will be performed on
 * Returns the minimum number of bytes required.
 */
static size_t doubleDabbleBufferSize(size_t intSize)  {
	if (intSize == 0) return 0;

	intSize *= 8;  // Convert to bits

	size_t digits = 1 + ((intSize - 1) / 3);  // ceil(intSize / 3)
	size_t nBits = intSize + digits * 4;

	return 1 + ((nBits - 1) / 8);  // ceil(nBits / 8);
}

static void doubleDabble() {

}

void kprintf(const char *format, ...) {
	va_list args;
	size_t charsPrinted = 0;  // Count of characters printed

	// Initialize varargs
	va_start(args, format);

	for (; *format; ++format) {
		if (*format == '%') {
			// Format specifier
			++format;
			if (!*format || *format == '%') {
				// Just print '%'
				uart1_putc('%');
				++charsPrinted;
				break;
			}

			// %[length]type
			bool leftAlign = false;
			bool prependPlus = false;
			bool prependSpace = false;
			bool prependZero = false;
			bool altForm = false;
			unsigned int width = 0;
			char length = '\0';

			// Check flags
			for (; true; ++format) {
				switch (*format) {
					case '-':  // left align flag
						leftAlign = true;
						continue;
					case '+':  // prepend plus flag
						prependPlus = true;
						continue;
					case ' ':  // prepend space flag
						prependSpace = true;
						continue;
					case '0':  // prepend zero flag
						prependZero = true;
						continue;
					case '#':  // alternate form flag
						altForm = true;
						continue;
				}
				break;  // Not a flag; move on to next thing
			}

			if (prependZero) leftAlign = false;  // prependZero overrides leftAlign
			if (prependSpace) prependPlus = false;

			// Width field
			if (*format >= '0' && *format <= '9') {
				// Width specifier
				for (; *format > '0' && *format < '9'; ++format) {
					width *= 10;
					width += *format - '0';
				} 
			} else if (*format == '*') {
				// Dynamic width specifier
				width = va_arg(args, unsigned int);
				++format;
			}

			// Length field
			switch (*format) {
				case 'h':
					if (*(format+1) == 'h') {
						length = 'H';
						format += 2;
						break;
					}
				case 'l':
					// NOTE: length specifier 'hl' will be treated as a long long
					// is this a problem?
					if (*(format+1) == 'l') {
						length = 'q';
						format += 2;
						break;
					}
				case 'q':
				case 'z':
				case 'j':
				case 't':
					length = *(format++);
					break;
			}

			// Type field
			if (*format == 'd' || *format == 'i') {
				// To format signed integers, check if the sign bit is set, and
				// if it is then clear it and change value to its 2s complement
				// then do unsigned double dabble and manually add the sign
				uintmax_t value;
				size_t size = getIntArg(&value, &args, length);

				const uintmax_t MSB = 1 << (size * 8 - 1);
				bool isNegative = value & MSB;  // check if MSB is set

				if (isNegative) {
					value ^= MSB;  // turn off msb
					value = (~value) + 1;  // convert to positive unsigned
				}


			} else if (*format == 'u') {
				uintmax_t value;
				size_t size = getIntArg(&value, &args, length);
			} else if (*format == 'x' || *format == 'X' || *format == 'p') {
				char *hexChars;
				if (*format == 'x') {
					hexChars = "0123456789abcdef";
				} else {
					hexChars = "0123456789ABCDEF";
				}

				// Get value and size
				uintmax_t value;
				size_t size;

				if (*format == 'p') {
					// Formatting pointer
					value = va_arg(args, void *);
					size = sizeof(void *);
				} else {
					// formatting integer
					size = getIntArg(&value, &args, length);
				}

				char buffer[size * 2];

				size_t index = 0;
				size_t shift = size * 8;
				do {
					shift -= 4;  // Move shift to 'point' to the bottom of the current nybble
					uintmax_t temp = value >> shift;
					// check if this is a leading zero
					if (temp != 0 || shift == 0) {
						buffer[index++] = hexChars[temp & 0xF];  // Lookup char in hexChars
					}
				} while (shift != 0);

				charsPrinted += printWidth(buffer, index, "0x", width, leftAlign, prependZero);
			} else if (*format == 'o') {
				// Get value and size
				uintmax_t value;
				size_t size = getIntArg(&value, &args, length);
				char buffer[size * 3];  // byte can hold up to 3 octal digits

				size_t index = 0;
				size_t shift = size * 8;
				do {
					shift -= 3;
					uintmax_t temp = value >> shift;
					// check if this is a leading zero
					if (temp != 0 || shift == 0) {
						buffer[index++] = (temp & 0xF) - '0';  // Convert to ASCII
					}
				} while (shift != 0);

				charsPrinted += printWidth(buffer, index, "0o", width, leftAlign, prependZero);
			} else if (*format == 'b') {
				uintmax_t value;
				size_t size = getIntArg(&value, &args, length) * 8;

				char buffer[size];
				size_t index = 0;
				uintmax_t mask = 1;
				while (mask != 0) {
					buffer[index++] = (value & mask) ? '1' : '0';
					mask <<= 1;
				}

				charsPrinted += printWidth(buffer, index, "0b", width, leftAlign, prependZero);
			} else if (*format == 's') {
				// Print string
				char *s = va_arg(args, char *);
				charsPrinted += printWidth(s, kstrlen(s), NULL, width, leftAlign, false);
			} else if (*format == 'c') {
				// Print character
				char c = (char)va_arg(args, int);
				charsPrinted += printWidth(&c, 1, width, NULL, leftAlign, false);
			} else if (*format == 'n') {
				// Store number of chars printed in a pointer
				unsigned int *ptr = va_arg(args, unsigned int *);
				*ptr = charsPrinted;
			}
		} else {
			if (*format == '\n') {
				uart1_putc('\r');
				++charsPrinted;
			}
			uart1_putc(*format);
			++charsPrinted;
		}
	}

	va_end(args);

	return;
}
