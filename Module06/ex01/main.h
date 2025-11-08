#pragma once

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

// only for types
#include <stdint.h>

#define F_CPU 16000000UL
#define F_SCL 100000UL // 100kHz
#define TWBR_VALUE ((((F_CPU / F_SCL) / 1) - 16 ) / 2) // 72


#define AHT20_ADDR 0x38// AHT20 Datasheet -> 5.3 Send Command
#define I2C_WRITE 0
#define I2C_READ 1


// UART FUNCTIONS

#define USAR_BAUDRATE 115200
// Equation for Calculating Baud Rate Register Setting (UBBRn) in asynchronous double speed mode
// BAUD = F_CPU / (8 * (UBRRn + 1))
// UBRRn = F_CPU / (8 * BAUD) - 1
#define BAUD_PRESCALER (((F_CPU / (USAR_BAUDRATE * 8UL))) - 1)


#define NEW_LINE "\r\n"

void init_uart(void);

void write_uart(char c);

char read_uart(void);

void putstr_uart(const char *str);

void puthex_uart(uint8_t hex);

void putnbr_uart(uint16_t nbr);

void debug_print(const char *str);

//
