#ifndef _UART_H_
#define _UART_H_

#include <stdint.h>

#ifdef __ASSEMBLER__
/* ASM */

#else
/* C */

#ifdef __cplusplus
extern "C" {
#endif
void pl011_uart_putc(void *base, char c);

/*
 * Kernel debug printf. This can be configured to print to any output device
 * by passing an underlying putc function. Newlines are translated into CRLF.
 *
 * putc		The underlying function called to output characters.
 * format	The format specifier to print
 *
 * format strings consist of the following syntax
 *  %[flags][width][length]type
 *
 * Flags field
 *  Value	Description
 *  -		Left Align
 *  +		Prepend plus for positive signed numeric types
 *  <space>	Prepend space for positive signed numeric types
 *  0		When width is specified, prepend zeroes for numeric types
 *  #		Use alternate form.
 *			  For type b, prepends `0b`
 *			  For type o, prepends `0o`
 *			  For type x and X, prepends `0x`
 *
 * Width Field
 *  Minimum number of characters to output. Does not truncate longer values.
 *  May either be a fixed number, or an `*`, meaning that the value is passed
 *  as an argument.
 *
 * Length field
 *  Value	Description
 *  hh		int-sized argument promoted from char
 *  h		int-sized argument promoted from short
 *  l		long-sized argument
 *  ll		long long sized argument
 *  q
 *  z		size_t sized argument
 *  j		intmax_t sized argument
 *  t		ptrdiff_t sized argument
 *
 * Type field
 *  Value	Description
 *  %		Literal '%'. Must be the only character in the specifier.
 *  d		Signed integer
 *  i
 *  u		Unsigned integer
 *  x		Hexadecimal formatted unsigned integer (lowercase)
 *  X		Hexadecimal formatted unsigned integer (uppercase)
 *  o		Octal formatted unsigned integer
 *  b		Binary formatted unsigned integer
 *  s		Null-terminated string
 *  c		Character
 *  p		Pointer
 *  n		Write nothing, but store number of characters printed so far to int ptr
 */
void kprintf(void *(*putc)(char), const char *format, ...) __attribute__((format(printf, 1, 2)));

/*
 * Like kprintf, but takes a va_list instead of variadic arguments. See kprintf
 */
void kvprintf(void *(*underlying_putc)(char), const char *format, va_list args) __attribute__((format(printf, 1, 2)));

/*
 * kprintf to UART1 (mini UART). This is equivalent to (but faster than) calling
 * uart_kprintf on UART1
 */
#define mini_kprintf(fmt, ...) kprintf(uart1_putc, fmt, __VA_ARGS__)

/*
 * kprintf to a specific UART
 *
 * uart		The uart to print to
 * fmt		The format string to use
 * Returns non-zero if the UART is invalid.
 */
int uart_kprintf(int uart, const char *fmt, ...) __attribute__((format(printf, 1, 2)));

#ifdef __cplusplus
}
#endif

#endif  // __ASSEMBLER__

#endif  // _UART_H_
