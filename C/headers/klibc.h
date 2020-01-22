#ifndef LIBC_H
#define LIBC_H

#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>

//void cursor_update();
void kputs(char *string);
void panic(char *string);
size_t strlen(char *string);
void kprintf(char *format,...);
void reverse(char *number, int length);
void diagnostic_message(char *string, bool state);
char *itoa(int number, char *string, int base);
void *memset(void *string, int character, size_t size);
void *memcpy(void *destination, const void *source, size_t size);
void *memmove(void *destination, const void *source, size_t size);
int memcmp(const void *source1, const void *source2, size_t size);

#endif