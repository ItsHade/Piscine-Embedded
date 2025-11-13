#pragma once

#include <avr/io.h>
#include <avr/eeprom.h>
// only for types
#include <stdint.h>

#define F_CPU 16000000UL

#define INVALID_EEPROM_ADDR 0xFFFF
#define MAGIC_VALUE 0xF7
#define HEX_TO_DIGIT 48 // + '0'
#define HEX_TO_UPPERCASE 55 // + 'A' + 10
#define CMD_INPUT 0
#define KEY_INPUT 1
#define VALUE_INPUT 2
#define STR_MAX_SIZE 32
#define ADDR_SIZE 1
#define CHUNK_SIZE 32
#define CMD_MAX_SIZE 6
#define MAX_WORD_COUNT 3
#define MIN_WORD_COUNT 3
#define INPUT_BUFFER_SIZE ((STR_MAX_SIZE + 1) * MAX_WORD_COUNT )
#define WRITE_COMMAND "WRITE"
#define PRINT_COMMAND "PRINT"
#define READ_COMMAND "READ"
#define FORGET_COMMAND "FORGET"
#define WRONG_INPUT "Error: INPUT HAS WRONG FORMAT\r\n"
#define UNKNOWN_COMMAND "Known commands are: WRITE, READ, PRINT, FORGET\r\n"
#define NO_SPACE_LEFT "no space left\r\n"
#define EMPTY "empty\r\n"
#define NOT_FOUND "not found\r\n"
#define ALREADY_EXISTS "already exists\r\n"

#define START_EEPROM_ADDRESS 0
#define MAX_EEPROM_ADDRESS 1023

#define NB_BYTE_PER_LINE 16
#define BACKSPACE_CHAR 127
#define SPACE_CHAR 32
#define DOUBLE_QUOTE_CHAR 34

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

void putstr_debug_uart(const char *str, char c);

void puthex_uart(uint8_t hex);

void puthex_lower_uart(uint8_t hex);

void putnbr_uart(uint32_t nbr);

char *get_input_uart(char *buffer, uint16_t bufferSize);

void debug_print(const char *str);


// EEPROM FUNCTIONS

uint8_t EEPROM_read(uint16_t address);

void EEPROM_write(uint16_t address, uint8_t byte);

void EEPROM_print(uint16_t startAddr, uint16_t stopAddr, uint32_t highlight, uint16_t nbBytesPerLine);

void EEPROM_print_c(uint16_t startAddr, uint16_t stopAddr, uint32_t highlight, uint16_t nbBytesPerLine);

void EEPROM_write_chunk(uint16_t addr, char *chunk, uint16_t chunkSize);

char *EEPROM_read_chunk(uint16_t addr, char *chunk, uint16_t chunkSize);

// === UTILS FUNCTIONS ===

uint8_t ft_is_allowed_char(char c, char *allowed);

uint8_t ft_is_allowed_chars(char *s, char *allowed);

uint16_t ft_strlen(char *s);

char *ft_capitalize(char *str);

uint8_t ft_isdigit(char c);

uint8_t ft_isprint(char c);

void ft_bzero(void *s, uint16_t n);

uint8_t ft_atouint8(char *str);

uint16_t ft_strcmp(char *s1, char *s2);

void	*ft_memcpy(void *dest, const void *src, uint8_t n);
