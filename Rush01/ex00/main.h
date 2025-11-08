#pragma once

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
// only for types
#include <stdint.h>
#include "isr.h"

// for test
#include <stdio.h>


#define F_CPU 16000000UL
#define F_SCL 100000UL
#define TWBR_PRESCALER 1 
#define TWBR_VALUE ((((F_CPU / F_SCL) / TWBR_PRESCALER) - 16 ) / 2) // 72

#define TIMER1_PRESCALER 256
#define TIMER1_FREQ 2 // hz
/* #define TIMER1_TOP (F_CPU / (TIMER1_PRESCALER * TIMER1_FREQ) - 1) */

#define USAR_BAUDRATE 115200
// Equation for Calculating Baud Rate Register Setting (UBBRn) in asynchronous double speed mode
// BAUD = F_CPU / (8 * (UBRRn + 1))
// UBRRn = F_CPU / (8 * BAUD) - 1
#define BAUD_PRESCALER (((F_CPU / (USAR_BAUDRATE * 8UL))) - 1)

#define BACKSPACE 127
#define END_OF_INPUT '\r'
#define NEW_LINE "\r\n"
#define TEXT_COLOR_RESET "\x1B[0m"
#define TEXT_COLOR_RED "\x1B[31m"
#define TEXT_COLOR_GREEN "\x1B[32m"
#define TEXT_COLOR_BLUE "\x1B[34m"


#define INPUT_BUFFER_SIZE 40 // need to be adjusted --> "28/02/2023 18:03:17"

#define MODES_COUNT 3 // don't like the name and only 3 for now (testing) | will be 12 
// Address
// Read bit (high level at SDA) = 1 | Write bit (low level at SDA) = 0
#define I2C_EXPANDER_ADDR 0x20

#define I2C_WRITE 0
#define I2C_READ 1

#define OUTPUT_PORT_0_COMMAND 0x02
#define OUTPUT_PORT_1_COMMAND 0x03
#define CONFIG_PORT_0_COMMAND 0x06
#define CONFIG_PORT_1_COMMAND 0x07 
#define ALL_OUTPUT 0x00
#define ALL_INPUT 0xFF
#define PORT_0_ALL_OFF 0xFF
#define PORT_0_ALL_ON 0x00
#define PORT_0_ALL_DIGIT_ON 0x0F

#define PORT_1_ALL_OFF 0x00
#define PORT_1_ALL_ON 0xFF

// Need rework
#define ONLY_D9 0xF7
#define ONLY_D10 0xFB
#define ONLY_D11 0xFD

#define SPI_START_FRAME_BYTE 0x00
#define SPI_END_FRAME_BYTE 0xFF


// devkit names with binary value to put on
// USE: Setting CA to 0 means that all 7 segment + dot is ON
// Default value is 1 (OFF)
enum Port0
{
    CA1 = 0xEF,
    CA2 = 0xDF,
    CA3 = 0xBF,
    CA4 = 0x7F
};


// USE: Setting pin to 0 means the segment (or dot) is OFF
// Default value is 1 (ON)
enum Port1
{
    SEGMENT_A = 0xFE,
    SEGMENT_B = 0xFD,
    SEGMENT_C = 0xFB,
    SEGMENT_D = 0xF7,
    SEGMENT_E = 0xEF,
    SEGMENT_F = 0xDF,
    SEGMENT_G = 0xBF,
    DPX = 0x7F
};

enum Spi
{
    LED_D6 = 0,
    LED_D7 = 1,
    LED_D8 = 2
};


enum Digit
{
    DISPLAY_0 = 0x3F,
    DISPLAY_1 = 0x06,
    DISPLAY_2 = 0x5B,
    DISPLAY_3 = 0x4F,
    DISPLAY_4 = 0x66, 
    DISPLAY_5 = 0x6D,
    DISPLAY_6 = 0x7D,
    DISPLAY_7 = 0x07,
    DISPLAY_8 = 0x7F,
    DISPLAY_9 = 0x6F,
    DISPLAY_DASH = 0x40,
    DISPLAY_DOT = 0x80
};
 // === I2C FUNCTIONS ===


 void i2c_init(void);

void i2c_start(void);

 void i2c_stop(void);

 void i2c_write(unsigned char data);

 uint8_t i2c_read(void);

 uint8_t i2c_read_nack(void);

// 7SEGMENTS DISPLAY

void i2c_print_digits(uint8_t addr, uint8_t digit1, uint8_t digit2, uint8_t digit3, uint8_t digit4);

void i2c_set_pin(uint8_t addr, uint8_t command, uint8_t port0);

// === UART FUNCTIONS ===

 
void init_uart(void);

void write_uart(char c);

char read_uart(void);

void putstr_uart(const char *str);

void puthex_uart(uint8_t hex);

void putnbr_uart(uint32_t nbr);

void debug_print(const char *str);


// === UTILS FUNCTIONS ===


uint8_t ft_ishexchar(char *s);

uint16_t ft_strlen(char *s);

char *ft_capitalize(char *str);

uint8_t ft_isdigit(char c);

void ft_bzero(void *s, uint16_t n);


// === TIMER FUNCTIONS ===

void init_timer1(uint8_t freq_hz);


// === SPI FUNCTIONS === 

void spi_init(void);

void spi_write(uint8_t write);

void spi_close(void);

void spi_set_led_all(uint8_t brightness, uint8_t color[3]);

void spi_set_led(uint8_t led, uint8_t brightness, uint8_t color[3]);
