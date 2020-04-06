#include "headers/vga.h"
#include "headers/gdt.h"
#include "headers/idt.h"
#include "headers/pmm.h"
#include "headers/klibc.h"
#include "headers/multiboot.h"

void kernel_main(multiboot_info_t *mb_info, uint64_t magic) {
    color = color_constructor(black, white);
    clear();
    kputs("WELCOME TO IDKOS\n");
    init_gdt();
    diagnostic_message("GLOBAL DESCRIPTOR TABLE -----------------", true);
    init_idt();
    diagnostic_message("INTERRUPT DESCRIPTOR TABLE --------------", true);

    if(magic != 0x2BADB002) {
        clear();
        panic("multiboot not detected: panicking...");
    }

    pmm_init(mb_info);
    while(1) {}
}