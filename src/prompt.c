#include <os/prompt.h>
#include <os/output.h>
#include <os/globals.h>
#include <os/string.h>
#include <os/pong/pong.h>

static uint8_t input_buffer_pos = 0;

void prompt_handle_input(char key)
{
    if(key == '\n')
    {
        printc('\n', 0);
        prompt_input_buffer[input_buffer_pos] = 0;
        prompt_handle_command();
        input_buffer_pos = 0;
        prompt_input_buffer[0] = 0;

        prints("KOMANDA>", 15);
        return;
    }

    prompt_input_buffer[input_buffer_pos++] = key;

    printc(key, 15);
}

void prompt_handle_command()
{
    if(streq("TEST", prompt_input_buffer))
    {
        prints("TEST RADI!\n", 1);
    }
    else if(streq("CLEAR", prompt_input_buffer))
    {
        fill_screen(background_color);
        cursor_position = 0;
    }
    else if(streq("PONG", prompt_input_buffer))
    {
        uint8_t bg = background_color;
        pong_start();
        background_color = bg;
        fill_screen(background_color);
    }
    else
    {
        prints("NEPOZNATA KOMANDA!\n\n", 15);
    }
    return;
}