#pragma once

#include <avr/io.h>
#include <avr/eeprom.h>
// only for types
#include <stdint.h>

#define F_CPU 16000000UL

#define MAX_ADDRESS 1023
#define NB_BYTE_PER_LINE 16

// === UART FUNCTIONS === 

#define USAR_BAUDRATE 115200
// Equation for Calculating Baud Rate Register Setting (UBBRn) in asynchronous double speed mode
// BAUD = F_CPU / (8 * (UBRRn + 1))
// UBRRn = F_CPU / (8 * BAUD) - 1
#define BAUD_PRESCALER (((F_CPU / (USAR_BAUDRATE * 8UL))) - 1)

#define INT_TO_CHAR_DIGIT 48
#define BACKSPACE 127
#define NON_PRINT '.'
#define END_OF_INPUT '\r'
#define REMOVE_CHAR "\b \b"
#define NEW_LINE "\r\n"
#define TEXT_COLOR_RESET "\x1B[0m"
#define TEXT_COLOR_RED "\x1B[31m"

void init_uart(void);

void write_uart(char c);

char read_uart(void);

void putstr_uart(const char *str);

void puthex_uart(uint8_t hex);

void puthex_lower_uart(uint8_t hex);

void putnbr_uart(uint32_t nbr);

char *get_input_uart(char *buffer, uint16_t bufferSize);

void debug_print(const char *str);
