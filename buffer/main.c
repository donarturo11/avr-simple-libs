#include <avr/io.h>
#include "buffer.h"

int main()
{
    char bufdata[16];
    buffer_t buf;
    buffer_t buf2;
    PORTB = 0xFF;
    buffer_init(&buf, bufdata, 8);
    //buffer_init(&buf, bufdata+8, 8);
    for(;;)
    {
        buffer_put(&buf, 0xAA);
        //char c = buffer_get(&buf);
        //buffer_put(&buf2, ~c);
    }
    return 0;
}
