#include <stddef.h>
#include <stdint.h>

enum {
	GPIO_BASE = 0x3F200000,           // GPIO base address
	GPPUD = (GPIO_BASE + 0x94),       // Pull GPIO pins up and down
	GPPUDCLK0 = (GPIO_BASE + 0x98),   // Pull special GPIO pins up and down
	UART0_BASE = 0x3F201000,          // UART base address
	/* UART offsets */
    UART0_DR     = (UART0_BASE + 0x00),
    UART0_RSRECR = (UART0_BASE + 0x04),
    UART0_FR     = (UART0_BASE + 0x18),
    UART0_ILPR   = (UART0_BASE + 0x20),
    UART0_IBRD   = (UART0_BASE + 0x24),
    UART0_FBRD   = (UART0_BASE + 0x28),
    UART0_LCRH   = (UART0_BASE + 0x2C),
    UART0_CR     = (UART0_BASE + 0x30),
    UART0_IFLS   = (UART0_BASE + 0x34),
    UART0_IMSC   = (UART0_BASE + 0x38),
    UART0_RIS    = (UART0_BASE + 0x3C),
    UART0_MIS    = (UART0_BASE + 0x40),
    UART0_ICR    = (UART0_BASE + 0x44),
    UART0_DMACR  = (UART0_BASE + 0x48),
    UART0_ITCR   = (UART0_BASE + 0x80),
    UART0_ITIP   = (UART0_BASE + 0x84),
    UART0_ITOP   = (UART0_BASE + 0x88),
    UART0_TDR    = (UART0_BASE + 0x8C),
};

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
	while(mmio_read(UART0_FR) & (1 << 5)) {}
	mmio_write(UART0_DR, c);
}

unsigned char uart_getc()
{
	while(mmio_read(UART0_FR) & (1 << 4)) {}
	return mmio_read(UART0_DR);
}

void uart_print(const char *str)
{
	for(size_t i = 0; str[i] != '\0'; ++i)
		uart_putc((unsigned char) str[i]);
}

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
	(void) r0;
	(void) r1;
	(void) atags;

	init_uart();
	uart_print("maverik v0\r\n");

	while(1) uart_putc(uart_getc());
}
