#include "headers/vga.h"
#include "headers/gdt.h"
#include "headers/idt.h"
#include "headers/klibc.h"
//#include "driver.h"

void kernel_main() {
    while(1) {}
    color = color_constructor(black, white);
    clear();
    kputs("WELCOME TO IDKOS\n");
    init_gdt();
    diagnostic_message("GLOBAL DESCRIPTOR TABLE -----------------", true);
    init_idt();
    diagnostic_message("INTERRUPT DESCRIPTOR TABLE --------------", true);
    //init_keyboard();
}