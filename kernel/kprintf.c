#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "kstring.h"
#include "uart.h"

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
 * Print a string with padding applied
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
size_t printWidth(const char *str, int length, const char *prefix, int width, bool left, bool zero) {
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

			} else if (*format == 'u') {

			} else if (*format == 'x' || *format == 'X') {
				char *hexChars;
				if (*format == 'x') {
					hexChars = "0123456789abcdef";
				} else {
					hexChars = "0123456789ABCDEF";
				}

				// Get value and size
				uintmax_t value;
				size_t size = getIntArg(&value, &args, length);
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

			} else if (*format == 'b') {

			} else if (*format == 's') {
				// Print string
				char *s = va_arg(args, char *);
				charsPrinted += printWidth(s, kstrlen(s), NULL, width, leftAlign, false);
			} else if (*format == 'c') {
				// Print character
				char c = (char)va_arg(args, int);
				charsPrinted += printWidth(&c, 1, width, NULL, leftAlign, false);
			} else if (*format == 'p') {

			} else if (*format == 'n') {
				// Store number of chars printed in a pointer
				unsigned int *ptr = va_arg(args, unsigned int *);
				*ptr = charsPrinted;
			}
		} else {
			uart1_putc(*format);
			++charsPrinted;
		}
	}

	va_end(args);

	return;
}


