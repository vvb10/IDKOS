#pragma once

#include <stdint.h>
#include "isr.h"

typedef struct idt_entry {
    uint16_t offset_low;
    uint16_t segment_selector;
    uint8_t interrupt_stack_table;
    uint8_t type_dpl_present;
    uint16_t offset_middle;
    uint32_t offset_high;
    uint32_t reserved;
} __attribute__ ((packed)) idte;

typedef struct idt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__ ((packed)) pidt;

void irq_handler();
extern void init_idt();