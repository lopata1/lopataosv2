#include <os/math.h>

uint32_t abs(int32_t n)
{
    int32_t mask = n >> 31;
    return (mask ^ n) - mask;
}

uint32_t pow(uint32_t n, uint32_t e)
{
    if(e == 0) return 1;
    uint32_t res = n;
    for(int i = 1; i < e; i++) res *= n;
    return res;
}

uint32_t log10int(uint32_t n)
{
    uint32_t res;
    while(n > 0)
    {
        n /= 10;
        res++;
    }
    return res - 1;
}