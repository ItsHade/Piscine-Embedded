#pragma once

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
// only for types
#include <stdint.h>

#define F_CPU 16000000UL

#define MAX_ADC_VALUE 1023

#define SPI_BRIGHTNESS 0xF0
#define SPI_BRIGHTNESS_MAX 0xFF
#define SPI_START_FRAME_BYTE 0x00
#define SPI_END_FRAME_BYTE 0xFF

#define SPI_LED_COUNT 3
#define SPI_COLOR_COUNT 7
#define SPI_COLOR_OFF {0x00, 0x00, 0x00}
#define SPI_COLOR_RED {0xFF, 0x00, 0x00}
#define SPI_COLOR_GREEN {0x00, 0xFF, 0x00}
#define SPI_COLOR_BLUE {0x00, 0x00, 0xFF}
#define SPI_COLOR_YELLOW {0xFF, 0xFF, 0x00}
#define SPI_COLOR_CYAN {0x00, 0xFF, 0xFF}
#define SPI_COLOR_MAGENTA {0xFF, 0x00, 0xFF}
#define SPI_COLOR_WHITE {0xFF, 0xFF, 0xFF}

enum Spi
{
    LED_D6 = 0,
    LED_D7 = 1,
    LED_D8 = 2
};

// === ADC FUNCTIONS ===

void init_adc(void);

uint16_t read_adc(void);

// === SPI FUNCTIONS === 

void spi_init(void);

void spi_write(uint8_t write);

void spi_close(void);

void spi_set_led_all(uint8_t brightness, uint8_t color[3][3]);

void spi_set_led(uint8_t led, uint8_t brightness, uint8_t color[3]);


// === UART FUNCTIONS ===

#define USAR_BAUDRATE 115200
// Equation for Calculating Baud Rate Register Setting (UBBRn) in asynchronous double speed mode
// BAUD = F_CPU / (8 * (UBRRn + 1))
// UBRRn = F_CPU / (8 * BAUD) - 1
#define BAUD_PRESCALER (((F_CPU / (USAR_BAUDRATE * 8UL))) - 1)


#define NEW_LINE "\r\n"
#define STATUS_STR "Status value: "

void init_uart(void);

void write_uart(char c);

char read_uart(void);

void putstr_uart(const char *str);

void puthex_uart(uint8_t hex);

void putnbr_uart(uint16_t nbr);

void debug_print(const char *str);

//
