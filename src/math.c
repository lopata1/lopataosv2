#include <os/math.h>

uint32_t abs(int32_t n)
{
    int32_t mask = n >> 31;
    return (mask ^ n) - mask;
}