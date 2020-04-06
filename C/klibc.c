#include "headers/vga.h"
#include "headers/klibc.h"
//#include "asm.h"

extern void halt();

void reverse(char *string, int length) {
    int start, end;
    char temp;

    if(length == 0) {
        return;
    }

    for(start=0, end=length-1; start<end; start++, end--) {
        temp = string[start];
        string[start] = string[end];
        string[end] = temp;
    }
}

char *itoa(uint64_t number, int base) {
    static char charset[] = "0123456789ABCDEF";
    static char buffer[65] = {0};
    char *ptr = &buffer[65];

    do {
        *--ptr = charset[number % base];
        number /= base;
    } while (number != 0);
    return ptr;
}

void *memset(void *source, int character, size_t size) {
    uint8_t *ptr = source;
    while(size--) {
        *ptr++ = (uint8_t)character;
    }
    return source;
}

void *memcpy(void *destination, const void *source, size_t size) {
    char *dest_ptr = destination;
    const char *src_ptr = source;
    while(size--) {
        *dest_ptr++ = *src_ptr++;
    }
    return destination;
}

void *memmove(void *destination, const void *source, size_t size) {
    uint8_t *dest_ptr = destination;
    const uint8_t *src_ptr = source;
    if(src_ptr < dest_ptr) {
        for(dest_ptr += size; src_ptr += size; size--) {
            *--dest_ptr = *--src_ptr;
        }
    } else {
        while(size--) {
            *dest_ptr++ = *src_ptr++;
        }
    }
    return destination;
}

int memcmp(const void *source1, const void *source2, size_t size) {
    const uint8_t *ptr1 = source1, *ptr2 = source2;
    while(size--) {
        if(*ptr1 != *ptr2) {
            return *ptr1 - *ptr2;
        } else {
            ptr1++, ptr2++;
        }
    }
    return 0;
}

size_t strlen(char *string) {
    size_t size = 0;
    for(;string[size]!= 0; size++);
    return size;
} 

//void cursor_update() {
//    uint16_t position = column + row * 80;
//    outb(0x3D4, 0x0F);
//    outb(0x3D5, (uint8_t)(position & 0x0FF));
//    outb(0x3D4, 0x0E);
//    outb(0x3D5, ((uint8_t)(position >> 8) & 0xFF));
//}

void diagnostic_message(char *string, bool state) {
    kprintf("SETTING UP %s [", string);

    if(state) {
        color_constructor(black, green);
        kputs("SUCSESS");
    } else {
        color = color_constructor(black, red);
        kputs("FAILURE");
    }

    color = color_constructor(black, white);
    kputs("]\n");
}

void kprintf(char *format, ...) {
    char *traverse;
    long integer;
    char *string;

    va_list argument;
    va_start(argument, format);

    for(traverse = format; *traverse != '\0'; traverse++) {
        while(*traverse != '%') {
            if(*traverse == '\r') {
                printc(' ');
                traverse++;
                continue;
            } else if(*traverse == '\0') {
                return;
            } else {
                printc(*traverse);
                traverse++;
            }
        }

        traverse++;

        switch (*traverse) {
            case 'c': {
                integer = va_arg(argument, int);
                printc(integer);
                break;
            }

            case 'd': {
                integer = va_arg(argument, int64_t);

                if(integer<0) {
                    integer = -integer;
                    printc('-');
                }

                char *string = itoa(integer, 10);
                for(int x=0; string[x] != '\0'; x++) {
                    printc(string[x]);
                }
                break;
            }

            case 'x': {
                integer = va_arg(argument, uint64_t);
                printc('0');
                printc('x');
                char *string = itoa(integer, 16);
                for(int x=0; string[x] != '\0'; x++) {
                    printc(string[x]);
                }
                break;
            }

            case 'b': {
                integer = va_arg(argument, uint64_t);
                printc('0');
                printc('b');
                char *string = itoa(integer, 2);

                for(int x=0; string[x] != '\0'; x++) {
                    printc(string[x]);
                }
                break;
            }

            case 's': {
                string = va_arg(argument, char *);
                for(int x=0; string[x] != '\0'; x++) {
                    printc(string[x]);
                }
                break;
            }
        }
    }
    va_end(argument);
    //cursor_update();
}

void kputs(char *string) {
    for(int i=0; string[i] != 0; i++) {
        printc(string[i]);
    }
}

int byte_to_page(int byte_number) {
    int i = byte_number % 4096;
    if(i != 0) {
        return byte_number / 4096 + 1;
    }
    return byte_number / 4096;
}

void panic(char *string) {
    kputs(string);
    printc('\n');
    halt();
}