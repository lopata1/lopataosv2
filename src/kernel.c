#include <os/output.h>
#include <os/ints/interrupts.h>
#include <os/globals.h>
#include <os/utils.h>
#include <os/prompt.h>


void poll_events();

void kmain()
{
    fill_screen(3);

    idt_init();

    fill_screen(20);

    prints("             LOPATA OS 2.0!\n\n", 15);
    prints(" DOBRODOSLI!\n\n\n", 4);
    prints("KOMANDA>", 15);

    input_buffer_pos = 0;
    prompt_input_buffer[0] = 0;
    input_pressed = 0;

    while(1)
    {
        poll_events();
        hlt();
    }

    return;
}


void poll_events()
{
    if(input_pressed && last_key_pressed)
    {
        input_pressed = 0;
        prompt_handle_input(last_key_pressed);
    }
    if(cursor_blink_passed_ms > 500)
    {
        blink_cursor();
        cursor_blink_passed_ms = 0;
    }
    return;
}