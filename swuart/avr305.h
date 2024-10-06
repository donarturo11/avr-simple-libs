#include <avr/io.h>
#include "swuart_config.h"
#define TxD TXPIN
#define RxD RXPIN
#ifndef __ASSEMBLER__
void AVR305_init();
extern int AVR305_put(char c);
extern int AVR305_get();
#endif
