#include <stddef.h>
#include <stdint.h>

static inline void sleep(uint32_t count)
{
	asm volatile("__sleep_%=: subs %[count], %[count], #1; bne __delay_%=\n"
				 : "=r"(count): [count]"0"(count) : "cc");
}

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
	(void) r0;
	(void) r1;
	(void) atags;
}
