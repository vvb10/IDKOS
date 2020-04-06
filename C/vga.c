#include "headers/vga.h"
#include "headers/klibc.h"

volatile uint16_t *vga_buffer = (uint16_t*)0xFFFFFFFFC00B8000;
int row = 0;
int column = 0;
int color = 15;
int index = 0;

void clear() {
    for(int i=0; i<2000; i++) {
        vga_buffer[i] = color << 8 | ' ';
    }
    row = 0;
    column = 0;
}

void printc(char character) {
    index = 80 * row + column;

    if(row == 25 && column == 80) {
        row = 0;
        column = 0;
    }

    if(column == 80) {
        row++;
        column = 0;
    }

    if(character == '\n') {
        if(row == 25) {
            clear();
        } else {
            row++;
            column = 0;
        }
        return;
    }

    vga_buffer[index] = color << 8 | character;
    column++;
}

uint16_t color_constructor(uint8_t forground, uint8_t background) {
    color = forground << 4 | background;
    return color;
}
