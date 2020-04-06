bits 32

global hang
global cpuid_check
global long_mode_check

section .text    
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
		je near hang
		ret

	long_mode_check:
		mov eax, 0x80000000
		mov edi, ebx ; cause cpuid affects ebx for some reason
		cpuid
		cmp eax, 0x80000001
		jb near hang

		mov eax, 0x80000001
		cpuid
		test edx, 1 << 29
		jz near hang
		mov ebx, edi
		ret

	hang:
		sti
		hlt
	hanging: jmp hanging