#include <os/math.h>
#include <os/ints/interrupts.h>


static uint64_t factorials[22];
static uint8_t calculated_factorials = 0;
static uint32_t max_terms = 20;

const float pi = 3.14159;


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

float powf(float n, uint32_t e)
{
    if(e == 0) return 1;
    float res = n;
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


void calculate_factiorials()
{
    uint64_t res = 1;
    factorials[0] = 1;
    factorials[1] = res;
    for(int i = 2; i < max_terms; i++)
    {
        res *= i;
        factorials[i] = res; 
    }
    calculated_factorials = 1;
}

// sin(x) = x - (x^3)/3! + (x^5)/5! - (x^7)/7!
// cos(x) = 1 - (x^2)/2! + (x^4)/4! - (x^6)/6!

float sin(float rads)
{
    if(!calculated_factorials) calculate_factiorials();

    float result = rads;

    for(uint32_t i = 1; i < max_terms/2; i++)
    {
        uint32_t term = 2*i+1;
        float term_value = powf(rads, term) / factorials[term];
        result += (i & 1) ? -term_value : term_value;
    }
    return result;
}

float cos(float rads)
{
    if(!calculated_factorials) calculate_factiorials();

    float result = 1;

    for(uint32_t i = 1; i < max_terms/2; i++)
    {
        uint32_t term = 2*i;
        float term_value = powf(rads, term) / factorials[term];
        result += (i & 1) ? -term_value : term_value;
    }
    return result;
}