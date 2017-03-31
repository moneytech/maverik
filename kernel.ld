SECTIONS {
	/* bootloader at 0x8000 */
	
	.init 0x8000 : {
		*(.init)
	}
	
	.text : {
		*(.text)
	}
	
	.data : {
		*(.data)
	}

	/* /DISCARD/ : {
		*(*)
	} */
}
