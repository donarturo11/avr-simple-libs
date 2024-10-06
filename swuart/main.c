#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
void setup();
void loop();
int main()
{
    setup();
    for(;;)
    {
        loop();
    }
    return 0;
}

void setup()
{
    PORTB = 0xFF;
    _delay_ms(0.5);
}
void loop()
{
    int c = UART_get();
    if (c != -1) {
        UART_put(c);
    }
}

