#include <os/output.h>
#include <os/globals.h>
#include <os/characters.h>

void fill_screen(uint8_t color)
{
    background_color = color;
    for(int i = 0; i < screen_height*screen_width; i++)
        *(video_mem + i) = color;
    return;
}


void printc(char c, uint8_t color)
{
    const int characters_space = 8;
    const int padding = screen_width * 2 + 2;

    for(int i = 0; i < 7; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            *(video_mem + j + i * screen_width + cursor_position * characters_space + (cursor_position / 40 * 9 * screen_width) + padding) = 
            (characters[c][i][j] * color) + (!characters[c][i][j] * background_color); 
        }
    }
    cursor_position++;
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