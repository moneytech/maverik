/**
 * main file
 *
 * 2017
 * by Florian Büstgens
 *
 */
	.section .data

	.section .init

	.section .text

sleep:
	ldr r2,=0x2000000

sleeploop:
	subs r2,r1
	bne sleeploop
	bx lr

	/* --------------------------- MAIN --------------------------- */
kernel_main:
	ldr r0,=0x20200000

	mov r1,#1
	lsl r1,#18
	str r1,[r0,#4]
	
	mov r1,#1
	lsl r1,#16
	str r1,[r0,#40] // LED
	
halt$:
	b halt$
