#include <os/globals.h>

char* video_mem = (char*) 0xa0000;
const uint16_t screen_height = 200;
const uint16_t screen_width = 320;
uint8_t background_color = 0;

uint16_t cursor_position = 0;

const uint8_t kbmap[129] = {
    0, 
    0, '1', '2', '3', '4', '5', '6', '7', '8', '9',
    '0', '-', '=', 0, 0, 'Q', 'W', 'E', 'R', 'T', 
    'Z', 'U', 'I', 'O', 'P', '[', ']', '\n', 0, 'A',
    'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'',
    0, 0, '\\', 'Y', 'X', 'C', 'V', 'B', 'N', 'M', 
    ',', '.', '/', 0, 0, 0, ' ', 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

char last_key_pressed = 0;
char last_key_released = 0;
uint8_t input_pressed = 0;
uint8_t input_released = 0;
uint16_t cursor_blink_passed_ms = 0;

char prompt_input_buffer[256];