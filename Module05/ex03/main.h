#pragma once

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define F_CPU 16000000UL
#define PRESCALER 256
#define FREQUENCY 50 // Hz
#define TOP (F_CPU / (PRESCALER * FREQUENCY) - 1) //1249


// UART FUNCTIONS

#define USAR_BAUDRATE 115200
// Equation for Calculating Baud Rate Register Setting (UBBRn) in asynchronous double speed mode
// BAUD = F_CPU / (8 * (UBRRn + 1))
// UBRRn = F_CPU / (8 * BAUD) - 1
#define BAUD_PRESCALER (((F_CPU / (USAR_BAUDRATE * 8UL))) - 1)


#define NEW_LINE "\r\n"
#define HEX_SEPARATOR ", "

void init_uart(void);

void write_uart(char c);

char read_uart(void);

void putstr_uart(const char *str);

void puthex_uart(uint8_t hex);

void putnbr_uart(uint16_t nbr);

void debug_print(const char *str);

//
