/**
 * boot file
 *
 * 2017
 * by Florian Büstgens
 *
 */

	.section ".text.boot"

	.globl _start

_start:
	mov sp,#0x8000 // loading boot address in sp

	// clear preserved registers (.bss)
	ldr r4,=__bss_start
	ldr r9,=__bss_end
	mov r5,#0
	mov r6,#0
	mov r7,#0
	mov r8,#0

	b 2f

1:
	stmia r4!, {r5-r8} // store multiple registers, inc after

2:
	cmp r4, r9 // compare __bss_start and __bss_end
	blo 1b

	ldr r3, =kernel_main
	blx r3

	// halt

halt:
	wfe // wait for interrupts
	b halt
