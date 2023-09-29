#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#define SCREEN_HEIGHT 200
#define SCREEN_WIDTH 320
#define TEXT_WIDTH 40
#define TEXT_HEIGHT 20
#define CHAR_HEIGHT 7
#define CHAR_WIDTH 5
#define CHARACTERS_SPACE 8

#include <stdint.h>

extern char* video_mem;
extern uint8_t background_color;
extern uint16_t cursor_position;

extern const uint8_t kbmap[129];

extern char last_key_pressed;
extern char last_key_released;
extern uint8_t input_pressed;
extern uint8_t input_released;
extern uint16_t cursor_blink_passed_ms;

extern char prompt_input_buffer[256];


#endif