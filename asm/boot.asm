[BITS 32]

[extern kernel_main]
[global start]

section .multiboot
	align 4
	header_start:
		dd 0xE85250D6 ; magic
		dd 0 ; arch (i386)
		dd (header_end-header_start) ; lengthsorry
		dd 0x100000000 - (0xE85250D6 + 0 + (header_end-header_start)) ; checksum
		dw 0 ; type
    	dw 0 ; flags
    	dd 8 ; size
	header_end:

section .bss
	align 16
	stack_bottom:
		resb 16384
	stack_top:

section .text
	GDT64:
		.Null: equ $ - GDT64
			dw 0xFFFF                    ; Limit (low).
			dw 0                         ; Base (low).
			db 0                         ; Base (middle)
			db 0                         ; Access.
			db 1                         ; Granularity.
			db 0                         ; Base (high).
		.Code: equ $ - GDT64         ; The code descriptor.
			dw 0                         ; Limit (low).
			dw 0                         ; Base (low).
			db 0                         ; Base (middle)
			db 10011010b                 ; Access (exec/read).
			db 10101111b                 ; Granularity, 64 bits flag, limit19:16.
			db 0                         ; Base (high).
		.Data: equ $ - GDT64         ; The data descriptor.
			dw 0                         ; Limit (low).
			dw 0                         ; Base (low).
			db 0                         ; Base (middle)
			db 10010010b                 ; Access (read/write).
			db 00000000b                 ; Granularity.
			db 0                         ; Base (high).
		.Pointer:                    ; The GDT-pointer.
			dw $ - GDT64 - 1             ; Limit.
			dq GDT64  

	start:
		mov esp, stack_top
		jmp cpuid_check
	kernel:
		call kernel_main
		jmp hang

	cpuid_check:
		pushfd
		pop eax
		mov ecx, eax

		xor eax, 1 << 21
		push eax
		popfd

		pushfd
		pop eax
		push ecx

		popfd
		cmp eax, ecx
		je no_cpuid ; cpuid is supported if je

	long_mode_check:
    	mov eax, 0x80000000
    	cpuid                 
    	cmp eax, 0x80000001 
    	jb no_long_mode  

		mov eax, 0x80000001
		cpuid
		test edx, 1 << 29
		jz no_long_mode

	table_clear:
		mov edi, 0x1000
		mov cr3, edi
		xor eax, eax
		mov ecx, 4096
		rep stosd
		mov edi, cr3

	link_tables:
		mov DWORD [edi], 0x2003
		add edi, 0x1000
		mov DWORD [edi], 0x3003
		add edi, 0x1000
		mov DWORD [edi], 0x4003
		add edi, 0x1000
		mov ebx, 3
		mov ecx, 512

	entry_set:
		mov DWORD [edi], ebx
		add ebx, 0x1000
		add edi, 8
		loop entry_set

	set_lm_bit:
		mov ecx, 0xC0000080
		rdmsr
		or eax, 1 << 8
		wrmsr

	enable_paging:
		mov eax, cr4
		or eax, 1 << 5
		mov cr4, eax
		mov eax, cr0
		or eax, 1 << 31
		mov cr0, eax

	load_gdt:
		lgdt [GDT64.Pointer]
		jmp GDT64.Code:segment_reset

	no_long_mode:
		jmp hang

	no_cpuid:
		jmp hang

	hang:
		cli
		hlt
		
	hanging: 
			jmp hanging


	[BITS 64]
	segment_reset:
		mov ax, 0
		mov ss, ax
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		cli
		jmp kernel