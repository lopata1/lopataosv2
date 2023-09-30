#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdint.h>

extern void memcpy(char* source, char* destination, uint32_t count);
extern void memset(char* destination, uint8_t value, uint32_t count);

#endif