#ifndef _UART_H_
#define _UART_H_

#define AUX_ENABLE_OFFSET			0x00215004
#define AUX_MU_IO_OFFSET			0x00215040
#define AUX_MU_IIR_OFFSET			0x00215044
#define AUX_MU_IER_OFFSET			0x00215048
#define AUX_MU_LCR_OFFSET			0x0021504C
#define AUX_MU_MCR_OFFSET			0x00215050
#define AUX_MU_LSR_OFFSET			0x00215054
#define AUX_MU_MSR_OFFSET			0x00215058

#define AUX_MU_SCRATCH_OFFSET		0x0021505C
#define AUX_MU_CNTL_OFFSET			0x00215060
#define AUX_MU_STAT_OFFSET			0x00215064
#define AUX_MU_BAUD_OFFSET			0x00215068

#ifdef __ASSEMBLER__
/* ASM */

#else
/* C */

void *init_uart1();
void uart1_putc(char c);

/*
 * Kernel debug printf. Prints to UART1. Newlines are translated into
 * CRLF.
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
void kprintf(const char *format, ...) __attribute__((format(printf, 1, 2)));

#endif  // __ASSEMBLER__

#endif  // _UART_H_

