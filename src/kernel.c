#include <os/output.h>
#include <os/ints/interrupts.h>
#include <os/globals.h>
#include <os/utils.h>


void poll_events();

void kmain()
{
    fill_screen(3);

    idt_init();

    fill_screen(20);

    prints("             LOPATA OS 20!\n\n", 15); // TODO: dodati tacku
    prints(" DOBRODOSLI!\n\n\n", 4);
    prints("KOMANDA ", 15);


    while(1)
    {
        poll_events();
        hlt();
    }

    return;
}


void poll_events()
{
    if(changed_input && last_key_pressed)
    {
        changed_input = 0;
        printc(last_key_pressed, 7);
    }
    if(cursor_blink_passed_ms > 100)
    {
        blink_cursor();
        cursor_blink_passed_ms = 0;
    }
    return;
}