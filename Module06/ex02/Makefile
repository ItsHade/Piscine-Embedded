
NAME = main
SRCS = $(NAME).c uart.c i2c.c
# ELF = $(NAME).elf
BIN = $(NAME).bin
HEX = $(NAME).hex

MCU = atmega328p
F_CPU = 16000000UL
BAUDRATE = 115200
PORT = /dev/ttyUSB0
CC = avr-gcc
OBJCOPY = avr-objcopy

# -Os optimize for size
# specify cpu frequency and target microcontroller
CFLAGS = -Os -Wall -Wextra -Werror -DF_CPU=$(F_CPU) -mmcu=$(MCU)

RM = rm -rf

all: hex flash

hex: $(SRCS)
#  should use -c to only compile and not link (way better practice)
	$(CC) $(CFLAGS) $(SRCS) -o $(BIN)

# 	allows us to link separately from compiling to a file we can use with avr-gdb
# 	$(CC) -mmcu=atmega328p $(BIN) -o $(ELF)

# 	-O ihex (output format: Intel HEX)
# 	-R .eeprom (remove section: .eeprom, to avoid causing errors by trying to write EEPROM content into Flash when flashing)

	avr-objcopy -O ihex -R .eeprom $(BIN) $(HEX)

flash:
	avrdude -F -V -c arduino -p ATMEGA328P -P $(PORT) -b $(BAUDRATE) -U flash:w:$(HEX)

clean:
	$(RM) $(BIN) $(HEX)

show: all
	screen $(PORT) $(BAUDRATE)


.PHONY: all hex flash clean show
