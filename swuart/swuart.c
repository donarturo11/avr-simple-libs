#include "swuart.h"
#include <avr/io.h>
#include <util/delay_basic.h>
#include "swuart_config.h"
void SWUART_delay()
{
    _delay_loop_1(BAUD_DELAY);
}

int SWUART_put(char c)
{
    UART_DDR |= TXMASK;
    uint16_t frame = 0;
    for (uint8_t i=0; i<STOP_BITS; ++i) {
	frame |= 1<<i;
    }
    frame <<= 9;
    frame |= (c << 1);
    tx_loop:
        if (frame & 1) UART_PORT |= TXMASK;
        else UART_PORT &= ~TXMASK;
        SWUART_delay();
        SWUART_delay();
        frame >>= 1;
        if (frame) {
            goto tx_loop;
        }
    return 0;
}

int SWUART_get()
{
    UART_DDR &= ~RXMASK;
    unsigned char cnt = 0xFF;
    char byte = 0;
    wait_rx:
        if (!(UART_PIN & RXMASK))
            goto rx_init;
    if (--cnt) goto wait_rx;
    return -1;
    rx_init:
        cnt = 8;
        SWUART_delay();
    rx_loop:
        SWUART_delay();
        SWUART_delay();
        if (UART_PIN & RXMASK) byte |= 0x80;
        else byte &= 0x7F;
        if(--cnt) {
            byte >>= 1;
            goto rx_loop;
        }
    return (unsigned char) byte;
}                     
