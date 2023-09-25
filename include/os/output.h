#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include <stdint.h>

void fill_screen(uint8_t color);
void printc(char c, uint8_t color);
void prints(char* str, uint8_t color);

#endif