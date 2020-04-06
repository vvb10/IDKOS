#include "headers/asm.h"
#include "headers/isr.h"
#include "headers/klibc.h"

__attribute__((interrupt)) void isr_handler(__attribute__ ((unused)) void *frame_ptr) {
    kputs("a recoverable error has occured\n");
}

__attribute__((interrupt)) void errorcode_isr_handler(__attribute__ ((unused)) void *frame_ptr, __attribute__ ((unused)) uint64_t error_code) {
    kputs("a recoverable error has occured\n");
}

__attribute__((interrupt)) void panicking_isr_handler(__attribute__ ((unused)) void *frame_ptr) {
    panic("an irrecoverable error has occured: panicking...");
}

__attribute__((interrupt)) void panicking_errorcode_isr_handler(__attribute__ ((unused)) void *frame_ptr, __attribute__ ((unused)) uint64_t error_code) {
    panic("an irrecoverable error has occured: panicking...");
}

__attribute__((interrupt)) void double_fault(iframe *frame, __attribute__ ((unused)) uint64_t error_code) {
    kprintf("phew, that was a close one: Error code: %d\n", frame->ip);
}

__attribute__((interrupt)) void general_protection_fault(__attribute__ ((unused)) iframe *frame, uint64_t error_code) {
    kprintf("%d ", error_code);
    panic("A GPF has occured: panicking...");
}

__attribute__((interrupt)) void bad_tss(__attribute__ ((unused)) void *frame_ptr) {
    panic("Bad TSS segment exception: panicking...");
}

__attribute__((interrupt)) void page_fault(__attribute__ ((unused)) void *frame_ptr, __attribute__ ((unused)) uint64_t error_code) {
    panic("Page Fault has occured: panicking...");
}