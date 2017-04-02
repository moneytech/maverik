#include <stddef.h>
#include <stdint.h>

#include "uart.h"

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
	(void) r0;
	(void) r1;
	(void) atags;

	init_uart();
	uart_print("maverik v0\r\n");

	while(1) uart_putc(uart_getc());
}
