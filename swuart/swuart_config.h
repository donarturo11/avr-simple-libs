#ifndef SWUART_CONFIG_H
#define SWUART_CONFIG_H

#ifdef TXPIN
#define TXMASK (1<<TXPIN)
#endif
#ifdef RXPIN
#define RXMASK (1<<RXPIN)
#endif

#define STOP_BITS 2

/** 
 * see AVR305 p. 3
 * download:
 * https://www.microchip.com/en-us/application-notes/an0952
**/

#define BIT_CYCLES (F_CPU/BAUD_RATE) 
#define BAUD_DELAY (BIT_CYCLES - 23)/6
#endif
