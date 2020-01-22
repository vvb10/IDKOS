#ifndef GDT_H
#define GDT_H

#include <stdint.h>
#include <stdbool.h>

typedef struct gdt_entry {
    uint16_t ignored1;
    uint16_t tss_selector;
    uint8_t ignored2; 
    uint8_t flags;
    uint8_t code_flags;
    uint8_t ignored3;
} __attribute__ ((packed)) gdte;

typedef struct gdt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__ ((packed)) pgdt;

void init_gdt();

#endif