PROJECT_NAME = test_uart
CONFIG ?= attiny85
DEFINITIONS = 

include config/$(CONFIG)/Makefile.mk

# UART_MODE: 
# - SWUART (Very simple software UART written in pure C)
# - AVR305 (assembly software UART described in AVR305 datasheet with small modifications)
UART_MODE ?= SWUART
BAUD_RATE ?= 19200

UARTPORT ?= B
TX ?= 1
RX ?= 0

# UART definitions
UART_PORT = PORT${UARTPORT}
UART_PIN = PIN${UARTPORT}
UART_DDR = DDR${UARTPORT}
DEFINITIONS += -D$(UART_MODE)
DEFINITIONS += -DBAUD_RATE=$(BAUD_RATE)

# software UART
DEFINITIONS += -DUART_PORT=$(UART_PORT)
DEFINITIONS += -DUART_DDR=$(UART_DDR)
DEFINITIONS += -DUART_PIN=$(UART_PIN)
DEFINITIONS += -DTXPIN=$(TX)
DEFINITIONS += -DRXPIN=$(RX)
##

FIRMWARENAME = $(PROJECT_NAME)-$(CONFIG)
CC = avr-gcc
OBJCOPY = avr-objcopy
DUDE = avrdude
PROGRAMMER ?= siprog
DUDEFLAGS = -v -p $(MCU) -c $(PROGRAMMER)

CFLAGS = -Wall -Os -I. -mmcu=$(MCU) -DF_CPU=$(F_CPU)
CFLAGS += -I./config/$(CONFIG)/
CFLAGS += $(DEFINITIONS)
OBJFLAGS = -j .text -j .data  
OBJECTS = main.o 

ifeq ($(UART_MODE), SWUART)
OBJECTS += swuart.o
else ifeq ($(UART_MODE), AVR305)
OBJECTS += avr305.o 
endif

all: $(FIRMWARENAME).hex $(FIRMWARENAME).bin $(FIRMWARENAME).asm

%.hex: %.elf
	$(OBJCOPY) $(OBJFLAGS) -O ihex $< $@

%.bin: %.elf
	$(OBJCOPY) $(OBJFLAGS) -O binary $< $@

$(FIRMWARENAME).asm: $(FIRMWARENAME).hex
	avr-objdump --no-show-raw-insn $(OBJFLAGS) -m$(AVRARCH) -D -S $(FIRMWARENAME).elf > $@

$(FIRMWARENAME).elf: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
	avr-size --format=avr --mcu=$(MCU) $(FIRMWARENAME).elf

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.S
	$(CC) $(CFLAGS) -x assembler-with-cpp -c -o $@ $<

fuse:
	$(DUDE) $(DUDEFLAGS) -e $(FUSES)

flash:
	$(DUDE) $(DUDEFLAGS) -Uflash:w:$(FIRMWARENAME).hex:i

simulavr:
	simulavr -d $(MCU) -o tracelist-$(MCU).txt
	@echo -e "\nPress Ctrl+C to abort\n"
	simulavr -d $(MCU) -F $(F_CPU) -c vcd:tracelist-$(MCU).txt:trace-$(MCU).vcd -f $(FIRMWARENAME).elf

clean:
	$(RM) *.o *.elf *.hex *.asm *.bin tracelist*.txt *.vcd
