#include <os/string.h>

uint8_t streq(char* source, char* destination)
{
    int i = 0;
    do
    {
        if(source[i] != destination[i]) return 0;
        i++;
    } while(source[i] != 0 && destination[i] != 0);
    return 1;
}