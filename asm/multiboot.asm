%include "asm/asm_macros.inc"

extern section_text
extern section_data_end
extern section_bss_end
extern start

MULTIBOOT_MEMORY_INFO equ 1<<1
MAGIC equ 0x1BADB002
FLAGS equ MULTIBOOT_MEMORY_INFO
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
align 4
dd MAGIC
dd FLAGS
dd CHECKSUM