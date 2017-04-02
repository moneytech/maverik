#include <stddef.h>
#include <stdint.h>

#include "uart.h"

static inline void mmio_write(uint32_t reg, uint32_t data)
{
	*(volatile uint32_t*)reg = data;
}
 
static inline uint32_t mmio_read(uint32_t reg)
{
	return *(volatile uint32_t*)reg;
}

static inline void sleep(uint32_t count)
{
	asm volatile("__sleep_%=: subs %[count], %[count], #1; bne __sleep_%=\n"
				 : "=r"(count): [count]"0"(count) : "cc");
}

void init_uart()
{
	mmio_write(UART0_CR, 0x00000000);                                                                               // disable UART0, init
	
	mmio_write(GPPUD, 0x00000000);                                                                                  // Disable pull up/down GPIO
	sleep(150);

	mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));                                                                   // Disable pull up/down pin 14/15
	sleep(150);

	mmio_write(GPPUDCLK0, 0x00000000);                                                                              // trigger GPPUDCLK0
	mmio_write(UART0_ICR, 0x7FF);                                                                                   // clearing interrupts
	mmio_write(UART0_IBRD, 1);                                                                                      // Divider = UART_CLOCK / (16 * Baud)
	                                                                                                                // -> 3000000 / (16 * 115200) = 1.627 = ~1

	mmio_write(UART0_FBRD, 40);                                                                                     // Fractional part reg = (Fractional part * 64) + 0.5
	                                                                                                                // -> (0.627 * 64) + 0.5 = 40.628 = ~40
	
	mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));                                                         // FIFO and 8 bit data transmission
	mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10)); // mask interrupts
	mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));                                                           // enable UART0
}

void uart_putc(const unsigned char c)
{
	while(mmio_read(UART0_FR) & (1 << 5));
	mmio_write(UART0_DR, c);
}

unsigned char uart_getc()
{
	while(mmio_read(UART0_FR) & (1 << 4));
	return mmio_read(UART0_DR);
}

void uart_print(const char *str)
{
	for(size_t i = 0; str[i] != '\0'; ++i)
		uart_putc((unsigned char) str[i]);
}
