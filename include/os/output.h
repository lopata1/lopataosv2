#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include <stdint.h>

void fill_screen(uint8_t color);
void printc(char c, uint8_t color);
void prints(char* str, uint8_t color);
void display_c_map(const char c_map[7][5], uint8_t color, uint16_t pos);
void blink_cursor();
void shift_text_up();

void display_c_map_g(const char c_map[7][5], uint8_t color, uint16_t x, uint16_t y);
void prints_g(char* str, uint8_t color, uint16_t x, uint16_t y);

#endif