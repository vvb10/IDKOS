#pragma once

#include <stdint.h>

enum vga_color {
	black = 0,
	blue = 1,
	green = 2,
	cyan = 3,
	red = 4,
	magenta = 5,
	brown = 6,
	light_gray = 7,
	dark_gray = 8,
	light_blue = 9,
	light_green = 10,
	light_cyan = 11,
	light_red = 12,
	light_magenta = 13,
	light_brown = 14,
	white = 15,
};

void clear();
void printc(char character);
uint16_t color_constructor(uint8_t forground, uint8_t background);
int row;
int index;
int column;
int color;