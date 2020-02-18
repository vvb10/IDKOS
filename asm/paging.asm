%include "asm/asm_macros.inc"

global page_tables
global set_up_page_tables

section .bss
    align 4096
    page_tables:
        p4: resb 4096
        p3l: resb 4096
        p3h: resb 4096
        p2: resb 4096
        p1: resb 4096 * 16
    page_tables_end:

bits 32

section .text
    set_up_page_tables:
        xor eax, eax
        mov edi, page_tables - _kernel_physical_offset
        mov ecx, (page_tables_end - page_tables) / 4
        rep stosd

        mov eax, 3
        mov edi, p1 - _kernel_physical_offset
        mov ecx, 512 * 16

    loop1:
        stosd
        push eax
        xor eax, eax
        stosd
        pop eax
        add eax, 0x1000
        loop loop1

        mov eax, p1 - _kernel_physical_offset
        or eax, 3
        mov edi, p2 - _kernel_physical_offset
        mov ecx, 16

    loop2:
        stosd
        push eax
        xor eax, eax
        stosd
        pop eax
        add eax, 0x1000
        loop loop2

        mov eax, p2 - _kernel_physical_offset
        or eax, 3
        mov edi, p3l - _kernel_physical_offset
        stosd
        xor eax, eax
        stosd

        mov eax, p2 - _kernel_physical_offset
        or eax, 3
        mov edi, p3h - _kernel_physical_offset + 511 * 8
        stosd
        xor eax, eax
        stosd

        mov eax, p3l - _kernel_physical_offset
        or eax, 3
        mov edi, p4 - _kernel_physical_offset
        stosd
        xor eax, eax
        stosd

        mov eax, p3l - _kernel_physical_offset
        or eax, 3
        mov edi, p4 - _kernel_physical_offset + 256 * 8
        stosd
        xor eax, eax
        stosd

        mov eax, p3h - _kernel_physical_offset
        or eax, 3
        mov edi, p4 - _kernel_physical_offset + 511 * 8
        stosd
        xor eax, eax
        stosd

    enable_paging:
        mov eax, page_tables - _kernel_physical_offset
        mov cr3, eax
        mov eax, cr4

        or eax, 1 << 5
        or eax, 1 << 7
        mov cr4, eax

        mov ecx, 0xC0000080
        rdmsr
        or eax, 1 << 8
        wrmsr

        mov eax, cr0
        or eax, 1 << 31
        mov cr0, eax
        
        ret