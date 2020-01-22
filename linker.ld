ENTRY(start)
 
SECTIONS {
	. = 1M;
	/* . = 0xFFFFFFFF80000000;
	_kernel_begin = .; */

	.text BLOCK(4K) : /* AT(ADDR(.text) - 0xFFFFFFFF80000000) */ {
		*(.multiboot)
		*(.text)
		/* . = ALIGN(4096); */
	}
 
	.rodata BLOCK(4K) : ALIGN(4K) /* : AT(ADDR(.rodata) - 0xFFFFFFFF80000000) */ {
		*(.rodata)
		/* . = ALIGN(4096); */
	}

	.data BLOCK(4K) : ALIGN(4K) /* : AT(ADDR(.data) - 0xFFFFFFFF80000000) */ {
		*(.data)
		/* . = ALIGN(4096); */
	}
 
	.bss BLOCK(4K) : ALIGN(4K) /* : AT(ADDR(.bss) - 0xFFFFFFFF80000000) */ {
		*(COMMON)
		*(.bss)
		/* . = ALIGN(4096); */
	}
	/* _kernel_end = .; */
}