#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdint.h>

static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) :"memory");
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

static inline void int_end()
{
    outb(0x20, 0x20);
    asm volatile ( "iret" );
}

static inline void hlt()
{
    __asm__ volatile ( "hlt" );
}

#endif