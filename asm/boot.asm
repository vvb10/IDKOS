%include "asm/asm_macros.inc"

bits 32

global start
global cmdline
 
extern halt
extern gdt_ptr
extern gdt_ptrl
extern cpuid_check
extern kernel_main
extern section_bss
extern section_bss_end
extern long_mode_check
extern set_up_page_tables


section .bss
	resb 4096*4

section .text
	start:
		mov esi, eax
		mov esp, 0xeffff0
		mov edi, section_bss
		mov ecx, section_bss_end
		sub ecx, section_bss
		xor eax, eax
		rep stosb

		call near cpuid_check
		call near long_mode_check
		call near set_up_page_tables
		lgdt [gdt_ptrl - _kernel_physical_offset]
		jmp 8:long_mode_init - _kernel_physical_offset

	long_mode_init:
		bits 64
		mov ax, 16
		mov ss, ax
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax

	jump_to_high_code:
		mov rax, higher_half
		jmp rax

	higher_half:
		mov rsp, _kernel_physical_offset + 0xeffff0
		lgdt [gdt_ptr]
		xor rbp, rbp
		mov rdi, rbx
		call kernel_main

	halt:
		cli
		hlt
	halting: jmp halting