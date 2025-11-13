#pragma once

#include <avr/io.h>
#include <avr/eeprom.h>
// only for types
#include <stdint.h>

#define F_CPU 16000000UL

#define HEX_TO_DIGIT 48 // + '0'
#define HEX_TO_UPPERCASE 55 // + 'A' + 10
#define ADDR_INPUT 0
#define BYTE_INPUT 1
#define ADDR_INPUT_SIZE 8
#define BYTE_INPUT_SIZE 2
#define INPUT_BUFFER_SIZE 12
#define MAX_ADDRESS 1023
#define NB_BYTE_PER_LINE 16

#define BACKSPACE_CHAR 127

// UART FUNCTIONS

#define USAR_BAUDRATE 115200
// Equation for Calculating Baud Rate Register Setting (UBBRn) in asynchronous double speed mode
// BAUD = F_CPU / (8 * (UBRRn + 1))
// UBRRn = F_CPU / (8 * BAUD) - 1
#define BAUD_PRESCALER (((F_CPU / (USAR_BAUDRATE * 8UL))) - 1)

#define BACKSPACE 127
#define NON_PRINT '.'
#define END_OF_INPUT '\r'
#define REMOVE_CHAR "\b \b"
#define NEW_LINE "\r\n"
#define STATUS_STR "Status value: "
#define WRONG_INPUT "Format: 000001ab f8\r\n"
#define ADDRESS_RANGE "Valid address range: 00000000 - 000003FF\r\n"
#define TEXT_COLOR_RESET "\x1B[0m"
#define TEXT_COLOR_RED "\x1B[31m"

void init_uart(void);

void write_uart(char c);

char read_uart(void);

void putstr_uart(const char *str);

void puthex_uart(uint8_t hex);

void puthex_lower_uart(uint8_t hex);

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
