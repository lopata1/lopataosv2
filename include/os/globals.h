#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdint.h>

extern char* video_mem;
extern const uint16_t screen_height;
extern const uint16_t screen_width;
extern uint8_t background_color;

extern uint16_t cursor_position;

extern const uint8_t kbmap[129];

extern char last_key_pressed;
extern uint8_t changed_input;
extern uint16_t cursor_blink_passed_ms;

extern char prompt_input_buffer[256];


#endif