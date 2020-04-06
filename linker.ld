ENTRY(start)

SECTIONS {
	_kernel_physical_offset = 0xFFFFFFFFC0000000;
	. = _kernel_physical_offset + 1M;

	.text ALIGN(4K) : AT(ADDR(.text) - _kernel_physical_offset){
		section_text = . - _kernel_physical_offset;
		KEEP(*(.multiboot))
		KEEP(*(.text*))
		section_text_end = . - _kernel_physical_offset;
	}
 
	.rodata ALIGN(4K) : AT(ADDR(.rodata) - _kernel_physical_offset) {
		section_rodata = . - _kernel_physical_offset;
		KEEP(*(.rodata*))
		section_rodata_end = . - _kernel_physical_offset;
	}

	.data ALIGN(4K) : AT(ADDR(.data) - _kernel_physical_offset){
		section_data = . - _kernel_physical_offset;
		KEEP(*(.data*))
		section_data_end = . - _kernel_physical_offset;
	}
 
	.bss ALIGN(4K) : AT(ADDR(.bss) - _kernel_physical_offset) {
		section_bss = . - _kernel_physical_offset;
		KEEP(*(COMMON))
		KEEP(*(.bss*))
		section_bss_end = . - _kernel_physical_offset;
	}

	_kernel_end = .;
}