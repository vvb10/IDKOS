%include "asm/asm_macros.inc"

extern section_text
extern section_data_end
extern section_bss_end
extern start

section .multiboot
	align 4
    multiboot_header:
        dd 0x1BADB002
        dd 0x00010000
        dd -(0x1BADB002 + 0x00010000)
        dd multiboot_header - _kernel_physical_offset
        dd section_text
        dd section_data_end
        dd section_bss_end
        dd start - _kernel_physical_offset