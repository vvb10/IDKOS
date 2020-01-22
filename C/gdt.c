#include "headers/gdt.h"

extern void gdt_flush(uint64_t);

gdte GDT[5];
pgdt pGDT;

#define null 0
#define kcode 1
#define kdata 2
#define ucode 3
#define udata 4

void init_gdt() {
    pGDT.limit = (sizeof(gdte)*5)-1;
    pGDT.base = (uint64_t)&GDT;

    // null descriptor
    GDT[null].ignored1 = 0;
    GDT[null].tss_selector = 0;
    GDT[null].ignored2 = 0;
    GDT[null].flags = 0;
    GDT[null].code_flags = 0;
    GDT[null].ignored3 = 0;

    // kernel code
    GDT[kcode].ignored1 = 0;
    GDT[kcode].tss_selector = 0;
    GDT[kcode].ignored2 = 0;
    GDT[kcode].flags = 0b10010010;
    GDT[kcode].code_flags = 0b00100000;
    GDT[kcode].ignored3 = 0;

    // kernel data
    GDT[kdata].ignored1 = 0;
    GDT[kdata].tss_selector = 0;
    GDT[kdata].ignored2 = 0;
    GDT[kdata].flags = 0b10010010;
    GDT[kdata].code_flags = 0;
    GDT[kdata].ignored3 = 0;

    // user code
    GDT[ucode].ignored1 = 0;
    GDT[ucode].tss_selector = 0;
    GDT[ucode].ignored2 = 0;
    GDT[ucode].flags = 0b11110010;
    GDT[ucode].code_flags = 0b00100000;
    GDT[ucode].ignored3 = 0;

    // user data
    GDT[udata].ignored1 = 0;
    GDT[udata].tss_selector = 0;
    GDT[udata].ignored2 = 0;
    GDT[udata].flags = 0b11110010;
    GDT[udata].code_flags = 0;
    GDT[udata].ignored3 = 0;

    gdt_flush((uint64_t)&pGDT);
}