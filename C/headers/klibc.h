#pragma once

#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>

#define ALIGN_DOWN(n, a) (((uint64_t)n) & ~((a) - 1ul))
#define ALIGN_UP(n, a) ALIGN_DOWN(((uint64_t)n) + (a) - 1ul, (a))

//void cursor_update();
void kputs(char *string);
void panic(char *string);
size_t strlen(char *string);
void kprintf(char *format,...);
int byte_to_page(int byte_number);
void reverse(char *number, int length);
char *convert(unsigned int number, int base);
void diagnostic_message(char *string, bool state);
void *memset(void *string, int character, size_t size);
void *memcpy(void *destination, const void *source, size_t size);
void *memmove(void *destination, const void *source, size_t size);
int memcmp(const void *source1, const void *source2, size_t size);