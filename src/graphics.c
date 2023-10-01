#include <os/graphics.h>
#include <os/globals.h>
#include <os/characters.h>
#include <os/memory.h>
#include <os/math.h>

static uint8_t graphics_buffer[200][320];

void draw_rectangle(rectangle_t rect)
{
    for(int i = 0; i < rect.size.y; i++)
    {
        for(int j = 0; j < rect.size.x; j++)
        {
            graphics_buffer[i + rect.position.y][j + rect.position.x] = rect.color;
        }
    }
}

void draw_text(text_t text)
{
    int i = 0;
    while(text.content[i] != 0)
    {
        vector2d_t position = make_vector2d(text.position.x + i * CHARACTERS_SPACE, text.position.y);
        draw_c_map(characters[text.content[i]], text.color, text.bg_color, position);
        i++;
        if(text.content[i] != 0)
        {
            position.x += CHAR_WIDTH;
            for(int j = 0; j < 3; j++)
            {
                for(int k = 0; k < CHAR_HEIGHT; k++)
                {
                    graphics_buffer[position.y + k][position.x + j] = text.bg_color;
                }
            }
        }
    }
    return;
}

void draw_c_map(const char c_map[7][5], uint8_t color, uint8_t bg_color, vector2d_t position)
{
    for(int i = 0; i < 7; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            graphics_buffer[i + position.y][j + position.x] = (c_map[i][j] * color) + (!c_map[i][j] * bg_color); 
        }
    }
}

void clear_buffer(uint8_t color)
{
    memset((char*)graphics_buffer, color, SCREEN_WIDTH*SCREEN_HEIGHT);
}

void display_buffer()
{
    memcpy((char*)graphics_buffer, (char*)video_mem, SCREEN_WIDTH*SCREEN_HEIGHT);
}


vector2d_t make_vector2d(uint16_t x, uint16_t y)
{
    vector2d_t v = {x, y};
    return v;
}

vector2df_t make_vector2df(float x, float y)
{
    vector2df_t v = {x, y};
    return v;
}

rectangle_t make_rectangle(vector2d_t position, vector2d_t size, uint8_t color)
{
    rectangle_t r = {position, size, color};
    return r;
}

text_t make_text(char* content, vector2d_t position, uint8_t color, uint8_t bg_color)
{
    text_t t = {content, position, color, bg_color};
    return t;
}

uint16_t center_text_x(uint16_t text_length)
{
    return (SCREEN_WIDTH - text_length*(CHAR_WIDTH+3)) / 2;
}

uint16_t center_y(uint16_t size_y)
{
    return (SCREEN_HEIGHT - size_y) / 2;
}

void draw_line(vector2d_t point1, vector2d_t point2, uint8_t color)
{
    int dx = point2.x - point1.x;
    int dy = point2.y - point1.y;

    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    float xi = (float) dx / steps;
    float yi = (float) dy / steps;

    float x = point1.x, y = point1.y;
    for(int i = 0; i < steps; i++)
    {
        x += xi;
        y += yi;
        draw_pixel(make_vector2d((int)x, (int)y), color);
    }
}

void draw_pixel(vector2d_t position, uint8_t color)
{
    graphics_buffer[position.y][position.x] = color;
}