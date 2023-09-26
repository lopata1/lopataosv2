#include <os/output.h>
#include <os/globals.h>
#include <os/characters.h>

#define TEXT_WIDTH 40
#define FONT_HEIGHT 7

static const char cursor_c_map[7][5] =
    {{0, 0, 0, 0, 0}, 
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0},
     {1, 1, 1, 1, 1},
     {1, 1, 1, 1, 1},
     };

void fill_screen(uint8_t color)
{
    background_color = color;
    for(int i = 0; i < screen_height*screen_width; i++)
        *(video_mem + i) = color;
    return;
}

void printc(char c, uint8_t color)
{
    if(c == '\n')
    {
        display_c_map(characters[0], 15, cursor_position);
        cursor_position = (cursor_position + TEXT_WIDTH) / TEXT_WIDTH * TEXT_WIDTH;
        display_c_map(cursor_c_map, 15, cursor_position);
        return;
    }

    display_c_map(characters[c], color, cursor_position++);
    display_c_map(cursor_c_map, 15, cursor_position);
    return;
}


void prints(char* str, uint8_t color)
{
    int i = 0;
    while(str[i] != 0)
    {
        printc(str[i], color);
        i++;
    }
    return;
}

void display_c_map(const char c_map[7][5], uint8_t color, uint16_t pos)
{
    const int characters_space = 8;
    const int padding = screen_width * 2 + 2;

    for(int i = 0; i < 7; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            *(video_mem + j + i * screen_width + pos * characters_space + (pos / TEXT_WIDTH * (FONT_HEIGHT + 2) * screen_width) + padding) = 
            (c_map[i][j] * color) + (!c_map[i][j] * background_color); 
        }
    }
}

static uint8_t blink = 0;

void blink_cursor()
{
    blink = !blink;
    display_c_map((blink) ? cursor_c_map : characters[0], 15, cursor_position);
}