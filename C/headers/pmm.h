#pragma once

#include "multiboot.h"

void pmm_init(multiboot_info_t *mb_info);
unsigned long *pmm_alloc(unsigned long requested_memory);
void pmm_free(unsigned long *ptr, unsigned long length);