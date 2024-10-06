;**** A P P L I C A T I O N   N O T E   A V R 3 0 5 ************************
;*
;* Title  : Half Duplex Interrupt Driven Software UART
;* Version  : rev. 1.2 (24-04-2002), reset vector added
;*   : rev. 1.1 (27.08.1997)
;* Last updated  : 24-04-2002
;* Target  : AT90Sxxxx (All AVR Device)
;*
;* Support email : avr@atmel.com
;*
;* Code Size  : 32 Words
;* Low Register Usage : 0
;* High Register Usage : 4
;* Interrupt Usage : None
;*
;* DESCRIPTION
;* This Application note contains a very code efficient software UART.
;* The example program receives one character and echoes it back.
;***************************************************************************

; modified by donarturo11
; added return integer for compatibility with stdio

#define __SFR_OFFSET 0 
#include <avr/io.h>
#include "avr305.h" // added declarations for use in C

;***** Pin definitions

;.equ RxD =   ;Receive pin is PD0
;.equ TxD =1   ;Transmit pin is PD1

.global AVR305_put
.global AVR305_get

;***** Global register variables

#define bitcnt r16 /* bit counter */
#define temp r17 /* temporary storage register */
#define Txbyte r18 /* Data to be transmitted */
#define Rxbyte r19 /* Received data */

;.section .text
;.org 0
;rjmp reset
;***************************************************************************
;*
;* "putchar"
;*
;* This subroutine transmits the byte stored in the "Txbyte" register
;* The number of stop bits used is set with the sb constant
;*
;* Number of words :14 including return
;* Number of cycles :Depens on bit rate
;* Low registers used :None
;* High registers used :2 (bitcnt,Txbyte)
;* Pointers used :None
;*
;***************************************************************************


AVR305_put:
   sbi UART_DDR,TxD
   mov Txbyte,r24      ; put passed by function character into buffer
putchar:
    ldi	bitcnt, 9+STOP_BITS	; 1+8+sb (sb is # of stop bits
	com Txbyte		; Inverte everything 
	sec				; Start bit 

putchar0:
	brcc putchar1 ;If carry set
	cbi UART_PORT, TxD ;    send a '0'
	rjmp putchar2 ;else 

putchar1:
	sbi UART_PORT, TxD ;    send a '1'
	nop

putchar2:
	rcall UART_delay ;One bit delay
	rcall UART_delay

	lsr Txbyte  ;Get next bit
	dec bitcnt  ;If not all bit sent
	brne putchar0 ;   send next
     ;else
        ldi r24,0
        ldi r25,0
	ret   ;   return


;***************************************************************************
;*
;* "getchar"
;*
;* This subroutine receives one byte and returns it in the "Rxbyte" register
;*
;* Number of words :14 including return
;* Number of cycles :Depens on when data arrives
;* Low registers used :None
;* High registers used :2 (bitcnt,Rxbyte)
;* Pointers used :None
;*
;***************************************************************************

;** modified by donarturo11 **
;* added:
;*  - returning value if success or -1 if failed
;*  - timeout waiting for start bit

AVR305_get:
  cbi UART_DDR,RxD     ; donarturo11: added set direction everytime
getchar: ldi  bitcnt,9 ; 8 data bit + 1 stop bit
         ldi  temp,0xff   ; set timeout
getchar1:
  dec temp    ; wait some time for start bit
  breq getchar4  ; if timeout return -1
  sbic  UART_PIN, RxD ; break loop if start bit detected
  rjmp  getchar1
  rcall UART_delay ;0.5 bit delay
getchar2:
    rcall UART_delay ;1 bit delay
    rcall UART_delay      
    clc   ;clear carry
    sbic UART_PIN, RxD ;if RX pin high
    sec   ;
    dec  bitcnt  ;If bit is stop bit
    breq  getchar3 ;   return
     ;else
    ror  Rxbyte  ;   shift bit into Rxbyte
    rjmp  getchar2 ;   go get next
; donarturo11 added return values
getchar3:          ; return byte if success
    ldi r25,0x00
    mov r24,Rxbyte
    ret
getchar4:        ; return -1 if failed
    ldi r25,0xff
    ldi r24,0xff
    ret
    

;***************************************************************************
;*
;* "UART_delay"
;*
;* This delay subroutine generates the required delay between the bits when
;* transmitting and receiving bytes. The total execution time is set by the
;* constant "b":
;*
;* 3·b + 7 cycles (including rcall and ret)
;*
;* Number of words :4 including return
;* Low registers used :None
;* High registers used :1 (temp)
;* Pointers used :None
;*
;***************************************************************************
; Some b values:  (See also table in Appnote documentation)
;
; 1 MHz crystal:
;   9600 bps - b=14
;  19200 bps - b=5
;  28800 bps - b=2
;
; 2 MHz crystal:
;  19200 bps - b=14
;  28800 bps - b=8
;  57600 bps - b=2

; 4 MHz crystal:
;  19200 bps - b=31
;  28800 bps - b=19
;  57600 bps - b=8
; 115200 bps - b=2

;.equ b =31 ;19200 bps @ 4 MHz crystal


UART_delay:
    ldi temp,BAUD_DELAY ; modified for setup baudrate from Makefile
UART_delay1:
    dec temp
    brne UART_delay1
    ret

;***** Program Execution Starts Here

;***** Test program
;**** added compiling test functions optionally **;
#ifdef AVR305TEST
reset:
    sbi UART_PORT,TxD ;Init port pins
    sbi UART_DDR,TxD
    ldi Txbyte,12 ;Clear terminal
    rcall putchar

forever:
    rcall getchar
    mov Txbyte,Rxbyte
    rcall putchar  ;Echo received char
    rjmp forever
#endif
