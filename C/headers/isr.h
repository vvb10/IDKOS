#ifndef __ISR_H
#define __ISR_H

__attribute__((interrupt)) void isr_handler(__attribute__ ((unused)) void *frame_ptr);
__attribute__((interrupt)) void errorcode_isr_handler(__attribute__ ((unused)) void *frame_ptr, uint64_t error_code);
__attribute__((interrupt)) void panicking_isr_handler(__attribute__ ((unused)) void *frame_ptr);
__attribute__((interrupt)) void panicking_errorcode_isr_handler(__attribute__ ((unused)) void *frame_ptr, __attribute__ ((unused)) uint64_t error_code);
__attribute__((interrupt)) void double_fault(__attribute__ ((unused)) void *frame_ptr, uint64_t error_code);
__attribute__((interrupt)) void general_protection_fault(void __attribute__ ((unused)) *frame_ptr, __attribute__ ((unused)) uint64_t error_code);
__attribute__((interrupt)) void bad_tss(__attribute__ ((unused)) void *frame_ptr);
__attribute__((interrupt)) void page_fault(__attribute__ ((unused)) void *frame_ptr, __attribute__ ((unused)) uint64_t error_code);

#endif