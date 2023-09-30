#ifndef _PROMPT_H_
#define _PROMPT_H_

#include <stdint.h>

void prompt_handle_input(char key);
void prompt_handle_command();

extern uint8_t input_buffer_pos;

#endif