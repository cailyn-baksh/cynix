#include <cstdarg>

#include "kernel/uart.h"

int pl011_kprintf(int uart, const char *fmt, ...) {
	void *(*putc)(char);

	if (uart == UART1) {
		putc = uart1_putc;
	} else {
		putc = [uart](char c){ pl011_uart_putc(uart, c); };
	}

	va_list args;
	va_start(args, fmt);

	kvprintf(putc, fmt, );

	va_end(args);
	return 0;
}
