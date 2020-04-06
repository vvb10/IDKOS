#pragma once

typedef struct interrupt_frame {
    uint64_t ip;
    uint64_t cs;
    uint64_t flags;
    uint64_t sp;
    uint64_t ss;
}iframe;

__attribute__((interrupt)) void isr_handler(__attribute__ ((unused)) void *frame_ptr);
__attribute__((interrupt)) void errorcode_isr_handler(__attribute__ ((unused)) void *frame_ptr, uint64_t error_code);
__attribute__((interrupt)) void panicking_isr_handler(__attribute__ ((unused)) void *frame_ptr);
__attribute__((interrupt)) void panicking_errorcode_isr_handler(__attribute__ ((unused)) void *frame_ptr, __attribute__ ((unused)) uint64_t error_code);
__attribute__((interrupt)) void double_fault(iframe *frame, uint64_t error_code);
__attribute__((interrupt)) void general_protection_fault(iframe *frame, __attribute__ ((unused)) uint64_t error_code);
__attribute__((interrupt)) void bad_tss(__attribute__ ((unused)) void *frame_ptr);
__attribute__((interrupt)) void page_fault(__attribute__ ((unused)) void *frame_ptr, __attribute__ ((unused)) uint64_t error_code);
