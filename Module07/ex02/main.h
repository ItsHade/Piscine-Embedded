#pragma once

#include <avr/io.h>
#include <avr/eeprom.h>
// only for types
#include <stdint.h>

#define F_CPU 16000000UL

#define HEX_TO_DIGIT 48 // + '0'
#define HEX_TO_UPPERCASE 55 // + 'A' + 10
#define CMD_INPUT 0
#define KEY_INPUT 1
#define VALUE_INPUT 2
#define STR_MAX_SIZE 32
#define CMD_MAX_SIZE 6
#define INPUT_BUFFER_SIZE ((STR_MAX_SIZE + 1) * 3 )

#define MAX_ADDRESS 1023

#define NB_BYTE_PER_LINE 16
#define BACKSPACE_CHAR 127
#define SPACE_CHAR 32
#define DOUBLE_QUOTE_CHAR 34

// UART FUNCTIONS

#define USAR_BAUDRATE 115200
// Equation for Calculating Baud Rate Register Setting (UBBRn) in asynchronous double speed mode
// BAUD = F_CPU / (8 * (UBRRn + 1))
// UBRRn = F_CPU / (8 * BAUD) - 1
#define BAUD_PRESCALER (((F_CPU / (USAR_BAUDRATE * 8UL))) - 1)


#define NEW_LINE "\r\n"
#define WRONG_INPUT "INPUT HAS WRONG FORMAT!\r\n"
#define TEXT_COLOR_RESET "\x1B[0m"
#define TEXT_COLOR_RED "\x1B[31m"

void init_uart(void);

void write_uart(char c);

char read_uart(void);

void putstr_uart(const char *str);

void puthex_uart(uint8_t hex);

void putnbr_uart(uint32_t nbr);

void debug_print(const char *str);


// EEPROM FUNCTIONS

uint8_t EEPROM_read(uint16_t address);

void EEPROM_write(uint16_t address, uint8_t byte);

void EEPROM_print(uint16_t startAddr, uint16_t stopAddr, uint16_t highlight);

// UTILS FUNCTIONS

uint8_t ft_ishexchar(char *s);

uint16_t ft_strlen(char *s);

char *ft_capitalize(char *str);

uint8_t ft_isdigit(char c);

void ft_bzero(void *s, uint16_t n);
