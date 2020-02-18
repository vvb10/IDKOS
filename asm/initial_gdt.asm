%include "asm/asm_macros.inc"

bits 64

global gdt_ptr
global gdt_ptrl

section .data
    align 16
    gdt_ptrl:
        dw gdt_end - gdt_start - 1 ; GDT size
        dd gdt_start - _kernel_physical_offset ; GDT start

    gdt_ptr:
        dw gdt_end - gdt_start -1 ; GDT size
        dq gdt_start ; GDT start

    gdt_start:
        null_descriptor:
            dw 0
            dw 0
            db 0
            db 0
            db 0
            db 0

        kernel_code:
            dw 0
            dw 0
            db 0
            db 10011010b 
            db 00100000b
            db 0

        kernel_data:
            dw 0
            dw 0
            db 0
            db 10010010b
            db 0
            db 0
    gdt_end: