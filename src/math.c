#include <os/math.h>
#include <os/ints/interrupts.h>

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
    uint32_t res = 0;
    while(n > 0)
    {
        n /= 10;
        res++;
    }
    return res - 1;
}

static uint32_t next = 1;
 
uint32_t rand(uint32_t max)
{
    next = next * 1103515245 + 12345;
    return (unsigned int) (next / 65536) % (max + 1);
}
 
void srand(uint32_t seed)
{
    next = seed;
}

void int_to_string(uint32_t n, char* destination)
{
    uint32_t length = log10int(n) + 1;

    for(int i = length - 1; i >= 0; i--)
    {
        destination[length - i - 1] = (n / pow(10, i)) % 10 + '0';
    }
}