#include <os/output.h>
#include <os/ints/interrupts.h>
#include <os/globals.h>


void kmain()
{
    fill_screen(3);

    idt_init();

    fill_screen(20);

    prints("LOPATA OS 20!", 15); // TODO: dodati tacku


    while(1)
    {
        if(changed_input)
        {
            if(!last_key_pressed) continue;
            changed_input = 0;
            printc(last_key_pressed, 7);
        }
    }

    return;
}


