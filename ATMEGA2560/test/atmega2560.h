#ifndef ATMEGA2560_H
#define ATMEGA2560_H

#include <avr/io.h>
#include <util/delay.h>

/*
!!!!! PIN 13 doesn't mean DIGITAL PIN 13 (BUILTIN LED) !!!!!

https://docs.arduino.cc/retired/hacking/hardware/PinMapping2560/
*/

#define F_CPU 16000000UL

#define OUTPUT 1
#define INPUT 0
#define HIGH 1
#define LOW 0
#define LED_ON 1
#define LED_OFF 0


/*
uint8_t digit -> 7 segments + dot
0 0 0 0 0 0 0 0
A B C D E F G .
*/
enum Digit
{
    DISPLAY_0 = 0xFC,
    DISPLAY_1 = 0x60,
    DISPLAY_2 = 0xDA,
    DISPLAY_3 = 0xF2,
    DISPLAY_4 = 0x66, 
    DISPLAY_5 = 0xB6,
    DISPLAY_6 = 0xBE,
    DISPLAY_7 = 0xE0,
    DISPLAY_8 = 0xFE,
    DISPLAY_9 = 0xF6,
    DISPLAY_DASH = 0x02,
    DISPLAY_DOT = 0x01,
    DISPLAY_UPPER_E = 0x9E,
    DISPLAY_LOWER_R = 0x0A,
    DISPALY_EMPTY = 0x00
};

// === UART FUNCTIONS === 

#define UART_BAUDRATE 115200
// Equation for Calculating Baud Rate Register Setting (UBBRn) in asynchronous double speed mode
// BAUD = F_CPU / (8 * (UBRRn + 1))
// UBRRn = F_CPU / (8 * BAUD) - 1
#define BAUD_PRESCALER (((F_CPU / (UART_BAUDRATE * 8UL))) - 1)

#define INT_TO_CHAR_DIGIT 48
#define BACKSPACE 127
#define END_OF_INPUT '\r'
#define REMOVE_CHAR "\b \b"
#define NEW_LINE "\r\n"
#define WRONG_INPUT "Format: #RRGGBBDX | #FULLRAINBOW\r\n"
#define TEXT_COLOR_RESET "\x1B[0m"
#define TEXT_COLOR_RED "\x1B[31m"

void uart_init(void);

void uart_write(char c);

char uart_read(void);

void uart_putstr(const char *str);

void uart_puthex(uint8_t hex);

void uart_putbin(uint8_t bin);

void uart_putnbr(uint32_t nbr);

char *uart_get_input(char *buffer, uint16_t bufferSize);

void uart_debug_print(const char *str);

#endif
