#include <os/output.h>
#include <os/globals.h>
#include <os/characters.h>

#define TEXT_WIDTH 40
#define TEXT_HEIGHT 20
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
    }
    else
    {
        display_c_map(characters[c], color, cursor_position++);
    }
    

    if(cursor_position >= TEXT_WIDTH * TEXT_HEIGHT) shift_text_up();
    display_c_map(cursor_c_map, 15, cursor_position);
    return;
}

void shift_text_up()
{
    for(int i = 0; i < screen_width*screen_height-screen_width*(FONT_HEIGHT+2); i++)
    {
        *(video_mem + i) = *(video_mem + i + screen_width*(FONT_HEIGHT+2));
    }
    for(int i = screen_width*screen_height-screen_width*(FONT_HEIGHT+2); i < screen_width*screen_height; i++)
    {
        *(video_mem + i) = background_color;
    }
    cursor_position = TEXT_WIDTH*(TEXT_HEIGHT-1);
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