ENTRY (_start)

SECTIONS {
	/* bootloader at 0x8000 (for qemu -m raspi2 maybe 0x10000...) */
	. = 0x8000;
	
	__start = .;
	__text_start = .;

	.text : {
		  KEEP(*(.text.boot))
		  *(.text)
	}

	. = ALIGN(4096); /* page size */
	__text_end = .;

	/*----------------------------*/
	
	__rodata_start = .;
	
	.rodata : {
			*(.rodata)
	}

	. = ALIGN(4096); /* page size */
	__rodata_end = .;
	
	/*----------------------------*/

	__data_start = .;
	
	.data : {
			*(.data)
	}

	. = ALIGN(4096); /* page size */
	__data_end = .;

	/*----------------------------*/

	__bss_start = .;
	
	.bss : {
			*(.bss)
	}

	. = ALIGN(4096); /* page size */
	__bss_end = .;
	__end = .;

}
