
NAME = main
SRCS = src/$(NAME).c src/twi.c src/timer.c
BIN = $(NAME).bin
HEX = $(NAME).hex

MCU = atmega328p
F_CPU = 16000000UL
BAUDRATE = 115200
PORT = /dev/ttyUSB0
PORT2 = /dev/ttyUSB1
CC = avr-gcc
OBJCOPY = avr-objcopy

# -Os optimize for size
# specify cpu frequency and target microcontroller
CFLAGS = -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU) -Iinclude

RM = rm -rf

all: hex flash

hex: $(SRCS)
#  should use -c to only compile and not link (way better practice)
	$(CC) $(CFLAGS) $(SRCS) -o $(BIN)
	avr-objcopy -O ihex -R .eeprom $(BIN) $(HEX)

flash:
	avrdude -F -V -c arduino -p ATMEGA328P -P $(PORT) -b $(BAUDRATE) -U flash:w:$(HEX)
	avrdude -F -V -c arduino -p ATMEGA328P -P $(PORT2) -b $(BAUDRATE) -U flash:w:$(HEX)

clean:
	$(RM) $(BIN) $(HEX)

show: all
	screen $(PORT) $(BAUDRATE)

show2: all
	screen $(PORT2) $(BAUDRATE)


.PHONY: all hex flash clean show