#include <os/graphics.h>
#include <os/globals.h>
#include <os/characters.h>

static uint8_t graphics_buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

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
        draw_c_map(characters[text.content[i]], text.color, make_vector2d(text.position.x + i * CHARACTERS_SPACE, text.position.y));
        i++;
    }
    return;
}

void draw_c_map(const char c_map[7][5], uint8_t color, vector2d_t position)
{
    for(int i = 0; i < 7; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            graphics_buffer[i + position.y][j + position.x] = (c_map[i][j] * color) + (!c_map[i][j] * background_color); 
        }
    }
}


extern void memcpy(char* source, char* destination);
extern void memset(char* destination, uint8_t value);

void clear_buffer(uint8_t color)
{
    memset((char*)graphics_buffer, color);
}

void display_buffer()
{
    memcpy((char*)graphics_buffer, (char*)video_mem);
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

text_t make_text(char* content, vector2d_t position, uint8_t color)
{
    text_t t = {content, position, color};
    return t;
}

uint16_t center_text_x(uint16_t text_length)
{
    return (SCREEN_WIDTH - text_length*(CHAR_WIDTH+2)) / 2;
}

uint16_t center_y(uint16_t size_y)
{
    return (SCREEN_HEIGHT - size_y) / 2;
}