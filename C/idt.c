#include "headers/idt.h"
#include "headers/isr.h"
#include "headers/asm.h"
#include "headers/klibc.h"

#define disable_ist 0
#define kcode 8
#define interrupt_gate_dpl_0_present 0b10001110

extern void idt_flush(uint64_t);

idte IDT[256];
pidt pIDT;

void idt_set_gate(uint8_t number, void *voffset, uint8_t interrupt_stack_table) {
    uintptr_t offset = (uintptr_t)voffset;
    IDT[number].offset_low = offset & 0xFFFFFFFF;
    IDT[number].segment_selector = kcode;
    IDT[number].interrupt_stack_table = interrupt_stack_table;
    IDT[number].type_dpl_present = interrupt_gate_dpl_0_present;
    IDT[number].offset_middle = (offset >> 16) & 0xFFFFFFFF;
    IDT[number].offset_high = (offset >> 32) & 0xFFFFFFFF;
}

idte IDT[256];
pidt pIDT;

void init_idt() {
    pIDT.limit = sizeof(idte)*256-1;
    pIDT.base = (uint64_t)&IDT;
    memset(&IDT, 0, sizeof(IDT));
    idt_set_gate(0, *panicking_isr_handler, disable_ist);
    idt_set_gate(1, *isr_handler, disable_ist);
    idt_set_gate(2, *panicking_isr_handler, disable_ist);
    idt_set_gate(3, *isr_handler, disable_ist);
    idt_set_gate(4, *panicking_isr_handler, disable_ist);
    idt_set_gate(5, *panicking_isr_handler, disable_ist);
    idt_set_gate(6, *panicking_isr_handler, disable_ist);
    idt_set_gate(7, *isr_handler, disable_ist);
    idt_set_gate(8, *double_fault, disable_ist);
    idt_set_gate(9, *isr_handler, disable_ist);
    idt_set_gate(10, *bad_tss, disable_ist);
    idt_set_gate(11, *panicking_errorcode_isr_handler, disable_ist);
    idt_set_gate(12, *panicking_errorcode_isr_handler, disable_ist);
    idt_set_gate(13, *general_protection_fault, disable_ist);
    idt_set_gate(14, *page_fault, disable_ist);
    idt_set_gate(15, *panicking_isr_handler, disable_ist);
    idt_set_gate(16, *isr_handler, disable_ist);
    idt_set_gate(17, *panicking_isr_handler, disable_ist);
    idt_set_gate(18, *panicking_isr_handler, disable_ist);

    for(int i=19;i<32;i++) {
        idt_set_gate(i, *panicking_isr_handler, disable_ist);
    }

    idt_flush((uint64_t)&pIDT);
}