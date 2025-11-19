
NAME = main
SRCS = $(NAME).c uart.c 
ELF = $(NAME).elf
# BIN = $(NAME).bin
HEX = $(NAME).hex

MCU = atmega2560
F_CPU = 16000000UL
BAUDRATE = 115200 
PORT = /dev/ttyACM0
PROTOCOL = wiring 

CC = avr-gcc
OBJCOPY = avr-objcopy
# -Os optimize for size
# specify cpu frequency and target microcontroller
CFLAGS = -Os -Wall -Wextra -Werror -DF_CPU=$(F_CPU) -mmcu=$(MCU)
RM = rm -rf


# Enable debug mode: make DEBUG=1
ifeq ($(DEBUG),1)
CFLAGS += -DDEBUG
endif


all: $(HEX) flash

# compile and link to an ELF
$(ELF): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $@

# convert ELF to hex
$(HEX): $(ELF)
	$(OBJCOPY) -O ihex -R .eeprom $< $@

flash: $(HEX)
	avrdude -F -V -c $(PROTOCOL) -p $(MCU) -P $(PORT) -b $(BAUDRATE) -D -U flash:w:$(HEX):i

clean:
	$(RM) $(ELF) $(HEX)

show: all
	screen $(PORT) $(BAUDRATE)

uart:
	screen $(PORT) $(BAUDRATE)


.PHONY: all hex flash clean show uart
