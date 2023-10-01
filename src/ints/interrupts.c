#include <os/ints/interrupts.h>
#include <os/utils.h>
#include <os/output.h>
#include <os/globals.h>

extern void* timer_isr_stub();
extern void* keyboard_isr_stub();

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];
 
    descriptor->isr_low        = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x08; // this value can be whatever offset your kernel code selector is in your GDT
    descriptor->attributes     = flags;
    descriptor->isr_high       = (uint32_t)isr >> 16;
    descriptor->reserved       = 0;
}

extern void* isr_stub_table[];
 
void idt_init() {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * 256 - 1;
 
    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
    }

    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    timer_init();
    idt_set_descriptor(0x20, timer_isr_stub, 0x8E);

    keyboard_init();
    idt_set_descriptor(0x21, keyboard_isr_stub, 0x8E);
 
    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__ volatile ("sti"); // set the interrupt flag
}

uint32_t timer_ms_passed = 0;
uint32_t timer_ms_since_boot = 0;

void timer_init()
{
    outb(0x43, 0b00110110);

    outb(0x40, 1193 & 0xFF);
    outb(0x40, 1193 >> 8);
}

void timer_isr()
{
    timer_ms_since_boot++;
    timer_ms_passed++;
    cursor_blink_passed_ms++;
    return;
}

void sleep(uint32_t ms)
{
    timer_ms_passed = 0;
    while(timer_ms_passed < ms) { hlt(); }
    return;
}

void keyboard_init()
{
    outb(0x64, 0xFF);
    outb(0x60, 0xF4);


    //outb(0x64, 0xF3);
    //outb(0x60, 0b00000000);
    //outb(0x60, 0xF4);
}

void keyboard_isr()
{
    uint8_t scancode = inb(0x60);
    if(scancode > 128) 
    {
        last_key_released = kbmap[scancode-128];
        input_released = 1;
        return;
    }
    last_key_pressed = kbmap[scancode];
    input_pressed = 1;

    return;
}