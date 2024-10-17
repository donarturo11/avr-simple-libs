#include <avr/io.h>
#include "buffer.h"

int main()
{
    char bufdata[8];
    BUFFER buf = BUFFER_SETUP(bufdata, 8);
    PORTB = 0xFF;
    for(;;)
    {
        buffer_put(0xAA, &buf);
    }
    return 0;
}
