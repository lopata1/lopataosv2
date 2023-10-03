#include <os/prompt.h>
#include <os/output.h>
#include <os/globals.h>
#include <os/string.h>
#include <os/games/pong/pong.h>
#include <os/games/snake/snake.h>
#include <os/games/test3d/test3d.h>
#include <os/characters.h>

uint8_t input_buffer_pos = 0;

void prompt_handle_input(char key)
{
    if(key == 0) return;

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
    else if(key == 8 && input_buffer_pos != 0) // backspace
    {
        prompt_input_buffer[--input_buffer_pos] = 0;
        display_c_map(characters[' '], 15, cursor_position);
        cursor_position -= 1;
        return;
    }

    if(key == 8) return;

    prompt_input_buffer[input_buffer_pos++] = key;

    printc(key, 15);
}

void prompt_handle_command()
{
    if(streq("TEST", prompt_input_buffer))
    {
        prints("TEST RADI!\n", 1);
    }
    else if(streq("POMOC", prompt_input_buffer))
    {
        prints("KOMANDE:\nPOMOC - OVA KOMANDA\nCLEAR - CISTI EKRAN\nPONG - IGRA PING-PONG\nSNAKE - IGRA ZMIJA\nTEST3D - TESTIRA 3D GRAFIKU\n\n", 15);
    }
    else if(streq("CLEAR", prompt_input_buffer))
    {
        fill_screen(background_color);
        cursor_position = 0;
    }
    else if(streq("PONG", prompt_input_buffer))
    {
        pong_start();
        fill_screen(background_color);
        cursor_position = 0;
    }
    else if(streq("SNAKE", prompt_input_buffer))
    {
        snake_start();
        fill_screen(background_color);
        cursor_position = 0;
    }
    else if(streq("TEST3D", prompt_input_buffer))
    {
        test3d_start();
        fill_screen(background_color);
        cursor_position = 0;
    }
    else
    {
        prints("NEPOZNATA KOMANDA!\n\n", 15);
    }
    return;
}