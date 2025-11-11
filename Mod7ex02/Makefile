# ---- Project config ----
BIN := prog
SRC := src/main.c src/uart.c src/utils.c src/eeprom.c

# ---- Target MCU ----
MCU := atmega328p
DEVICE := m328p
F_CPU := 16000000UL
PROGRAMMER := arduino

# ---- Compil options ----
WARNING := -Wall -Werror -Wextra
OPTI := -Os
INC_PATH = -Iincludes -I/usr/lib/avr/include

# ---- Serial Port ----
BAUDRATE := 115200
UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
  PORT := $(shell ls /dev/tty.usb* 2>/dev/null | head -n1)
else ifeq ($(UNAME),Linux)
  PORT := $(shell ls /dev/ttyUSB* /dev/ttyACM* 2>/dev/null | head -n1)
endif

# ---- Rules ----
.DEFAULT_GOAL := all

all: hex flash

hex: ${BIN}.hex

${BIN}.bin: ${SRC}
	avr-gcc ${WARNING} -mmcu=${MCU} -DF_CPU=${F_CPU} ${OPTI} ${INC_PATH} -o ${BIN}.bin ${SRC}

${BIN}.hex: ${BIN}.bin
	avr-objcopy -O ihex ${BIN}.bin ${BIN}.hex

flash: ${BIN}.hex
	avrdude -c ${PROGRAMMER} -p ${DEVICE} -P ${PORT} -b ${BAUDRATE} -U flash:w:${BIN}.hex

clean:
	rm -f ${BIN}.bin ${BIN}.hex

.PHONY: all hex flash clean
