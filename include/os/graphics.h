#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <stdint.h>
#include <os/globals.h>

typedef struct {
    uint16_t x;
    uint16_t y;
} vector2d_t;

typedef struct {
    float x;
    float y;
} vector2df_t;

typedef struct {
    vector2d_t position;
    vector2d_t size;
    uint8_t color;
} rectangle_t;

typedef struct {
    char* content;
    vector2d_t position;
    uint8_t color;
    uint8_t bg_color;
} text_t;

void draw_rectangle(rectangle_t rect);
void draw_text(text_t text);
void draw_c_map(const char c_map[7][5], uint8_t color, uint8_t bg_color, vector2d_t position);
void display_buffer();
void clear_buffer(uint8_t color);

vector2d_t make_vector2d(uint16_t x, uint16_t y);
rectangle_t make_rectangle(vector2d_t position, vector2d_t size, uint8_t color);
text_t make_text(char* content, vector2d_t position, uint8_t color, uint8_t bg_color);
vector2df_t make_vector2df(float x, float y);
uint16_t center_text_x(uint16_t text_length);
uint16_t center_y(uint16_t size_y);

#endif