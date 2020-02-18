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
		je hang ; cpuid is supported if je
        ret

	long_mode_check:
    	mov eax, 0x80000000
    	cpuid                 
    	cmp eax, 0x80000001 
    	jb hang  

		mov eax, 0x80000001
		cpuid
		test edx, 1 << 29
		jz hang
        ret

    hang: 
        cli
        hlt
    hanging: jmp hanging