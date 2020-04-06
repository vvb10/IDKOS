#include <stdbool.h>
#include "headers/pmm.h"
#include "headers/klibc.h"
#include "headers/multiboot.h"

extern uint8_t _kernel_end;
extern uint8_t _kernel_physical_offset;

static uint_fast32_t bitmap_size;
static uint_fast32_t free_area_start;

void pmm_set(int start, int startbit, int end, int endbit, bool one) {
    uint64_t *ptr = (uint64_t*)&_kernel_end + start;
    bool is_first = true;

    if(start == end) {
        if(startbit == 0 && endbit == 63) {
            if(one)
                *ptr = 0xFFFFFFFFFFFFFFFF;
            else
                *ptr = 0;
        } else if(startbit == endbit) {
            if(one)
                *ptr |= (1ul << startbit);
            else
                *ptr &= ~(1ul << startbit);
        } else {
            for(;startbit != endbit; startbit++) {
                if(one)
                    *ptr |= (1ul << startbit);
                else
                    *ptr &= ~(1ul << startbit);    
            }  
        }
        return;
    }

    do {
        if(!is_first)
            if(one)
                *ptr = 0xFFFFFFFFFFFFFFFF;
            else
                *ptr = 0;

        else {
            for(;startbit != 64; startbit++) {
                if(one) {
                    *ptr |= (1ul << startbit);
                }
                else {
                    *ptr &= ~(1ul << startbit);
                }
            }
            is_first = false;
        }

        ptr++;
        start++;
        startbit = 0;
    } while(start != end);

    if(startbit == endbit && startbit != 0) {
        if(one)
            *ptr |= (1ul << startbit);
        else
            *ptr &= ~(1ul << startbit);

        return;
    }

    for(;startbit < endbit; startbit++) {
        if(one)
            *ptr |= (1ul << startbit);
        else
            *ptr &= ~(1ul << startbit);
    }
}

void pmm_init(multiboot_info_t *mb_info) {
    if(mb_info->flags & 1) {
        uint64_t *ptr = (uint64_t*)&_kernel_end;
        uint64_t  free_memory = ALIGN_UP(mb_info->mem_lower, 4096) + ALIGN_DOWN(mb_info->mem_upper, 4096);

        if(free_memory % 64) 
            bitmap_size = free_memory / 64 + 1;
        else
            bitmap_size = free_memory / 64;

        for(unsigned int i=0; i != bitmap_size; i++) 
            ptr[i] = 0xFFFFFFFFFFFFFFFF;

        ptr++;
        if(free_memory % 64)
            for(int i=0; i != free_memory % 64 + 1; i++)
                *ptr |= (1ul << i);

        else
            *ptr = 0xFFFFFFFFFFFFFFFF;

        ptr = (uint64_t*)&_kernel_end;
        if(mb_info->flags & (1 << 6)) {
            multiboot_memory_map_t *reader = (multiboot_memory_map_t*)(uint64_t)mb_info->mmap_addr;
            for(unsigned int i=0; i != mb_info->mmap_length / sizeof(multiboot_memory_map_t); i++) {
                if(reader[i].type == MULTIBOOT_MEMORY_AVAILABLE && reader[i].addr != 0) {
                    pmm_set(ALIGN_UP(reader[i].addr, 4096) / 4096 / 4 / 64, ALIGN_UP(reader[i].addr, 4096) / 4096 / 4 % 64, 
                    ALIGN_DOWN(reader[i].len, 4096) / 4096 / 4 / 64 + ALIGN_UP(reader[i].addr, 4096) / 4096 / 4 / 64, ALIGN_DOWN(reader[i].len, 4096) / 4096 / 4 % 64, false);
                    free_area_start = ALIGN_UP(reader[i].addr, 4096) / 4096 / 4 / 64;
                }
            }
        }
        pmm_set(0x100000 / 4096 / 4 / 64, 0x100000 / 4096 / 4 % 64, (&_kernel_end - &_kernel_physical_offset + bitmap_size) / 4096 / 4/ 64,
        (&_kernel_end - &_kernel_physical_offset + bitmap_size) / 4096 / 4 % 64, true);
    }
}

uint64_t *pmm_alloc(uint64_t requested_memory) {

    if(requested_memory == 0) {
        return NULL;
    }

    bool is_first = true;
    bool hit_free = false;
    bool hit_reserved = false;
    uint64_t free_area = 0;
    uint64_t startbit = 0;
    uint64_t areastart = 0;
    uint64_t itarator = free_area_start;
    uint64_t *ptr = (uint64_t*)&_kernel_end;

    while(free_area < requested_memory && itarator != bitmap_size) {
        if(ptr[itarator] == 0xFFFFFFFFFFFFFFFF && hit_free) {
            hit_reserved = true;
        } else if(ptr[itarator] == 0) {
            if(is_first) {
                hit_free = true;
                areastart = itarator;
            }

            while(ptr[itarator] == 0) {
                itarator++;
                free_area += 64;

                if(free_area >= requested_memory)
                    break;
            }
        } else {
            for(int i=0; i!=64; i++) {
                if((ptr[itarator] & (1ul << i)) == 0) {
                    if(is_first) {
                        startbit = i;
                        hit_free = true;
                        is_first = false;
                        areastart = itarator;
                    }
                    free_area++;
                } else if(hit_free) {
                    hit_reserved = true;
                }
            }
        }
        if(free_area >= requested_memory) {
            free_area -= (free_area - requested_memory);
            pmm_set(areastart, startbit, areastart + ((startbit + free_area) / 64), (startbit + free_area) % 64, true);
            return (uint64_t*)((areastart * 64 + startbit) * 4096 * 4);
        } if(hit_reserved) {
            startbit = 0;
            free_area = 0;
            areastart = 0;
            is_first = true;
            hit_free = false;
            hit_reserved = false;
        }
        itarator++;
    }
    return NULL;
}

void pmm_free(uint64_t *ptr, uint64_t length) {
    pmm_set((uintptr_t)ptr / 4096 / 4 / 64, (uintptr_t)ptr / 4096 / 4 % 64, (uintptr_t)ptr / 4096 / 4 / 64 + length / 64, (uintptr_t)ptr / 4096 / 4 % 64 + length % 64, false);
}