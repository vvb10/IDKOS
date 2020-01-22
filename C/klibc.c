#include "headers/vga.h"
#include "headers/klibc.h"
//#include "asm.h"

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

char *itoa(int number, char *string, int base) {
    int i = 0;
    bool is_negative = false;
    
    if(number == 0) {
        string[i] = '0';
        string[++i] = '\0';
        return string;
    }

    if(number < 0 && base == 10) {
        is_negative = true;
        number = -number;
    }

    while(number != 0) {
        int remainder = number % base;
        string[i++] = (remainder > 9) ? (remainder-10) + 'A' : remainder + '0';
        number = number/base;
    }

    if(is_negative) {
        string[i++] = '-';
    }

    string[i] = '\0';
    reverse(string, i);
    return string;
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

int32_t memcmp(const void *source1, const void *source2, size_t size) {
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
    for(;string[size] != 0; size++);
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
    char string_buffer[32];
    char *traverse;
    int integer;
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
                integer = va_arg(argument, int32_t);
                printc(integer);
                break;
            }

            case 'd': {
                integer = va_arg(argument, int32_t);

                if(integer<0) {
                    integer = -integer;
                    printc('-');
                }

                itoa(integer, string_buffer, 10);
                for(int32_t x=0; string_buffer[x] != '\0'; x++) {
                    printc(string_buffer[x]);
                }
                break;
            }

            case 's': {
                string = va_arg(argument, char *);
                for(int32_t x=0; string[x] != '\0'; x++) {
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

void panic(char *string) {
    kputs(string);
    printc('\n');
    while(1) {}
}