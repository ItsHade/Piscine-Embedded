#pragma once

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
// only for types
#include <stdint.h>
#include <stddef.h>


#define F_CPU 16000000UL

#define IS_PRESSED(pin) (!(PIND & (1 << (pin))))

#define MODE_COUNT 3
enum ColorMode
{
    MODE_R = 0,
    MODE_G = 1,
    MODE_B = 2
};

#define MAX_COLOR_VALUE 255
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


#define LED_COUNT 3
enum Spi
{
    LED_D6 = 0,
    LED_D7 = 1,
    LED_D8 = 2, 
    LED_ALL = 3
};




// === UART FUNCTIONS === 

#define USAR_BAUDRATE 115200
// Equation for Calculating Baud Rate Register Setting (UBBRn) in asynchronous double speed mode
// BAUD = F_CPU / (8 * (UBRRn + 1))
// UBRRn = F_CPU / (8 * BAUD) - 1
#define BAUD_PRESCALER (((F_CPU / (USAR_BAUDRATE * 8UL))) - 1)

#define INT_TO_CHAR_DIGIT 48
#define BACKSPACE 127
#define END_OF_INPUT '\r'
#define REMOVE_CHAR "\b \b"
#define NEW_LINE "\r\n"
#define WRONG_INPUT "Format: #RRGGBBDX | #FULLRAINBOW\r\n"
#define TEXT_COLOR_RESET "\x1B[0m"
#define TEXT_COLOR_RED "\x1B[31m"

void init_uart(void);

void write_uart(char c);

char read_uart(void);

void putstr_uart(const char *str);

void puthex_uart(uint8_t hex);

void putnbr_uart(uint32_t nbr);

char *get_input_uart(char *buffer, uint16_t bufferSize);

void debug_print(const char *str);

// === ADC FUNCTIONS ===

void init_adc(void);

uint16_t read_adc(void);

// === SPI FUNCTIONS === 

void spi_init(void);

void spi_write(uint8_t write);

void spi_close(void);

void spi_set_led_all(uint8_t brightness, uint8_t color[3][3]);

void spi_set_led(uint8_t led, uint8_t brightness, uint8_t color[3]);


// === UTILS FUNCTIONS ===

uint8_t ft_is_allowed_char(char c, char *allowed);

uint8_t ft_is_allowed_chars(char *s, char *allowed);

uint16_t ft_strlen(char *s);

char *ft_capitalize(char *str);

uint8_t ft_isdigit(char c);

void ft_bzero(void *s, uint16_t n);

uint8_t ft_atouint8(char *str);

uint16_t ft_strcmp(char *s1, char *s2);

void	*ft_memcpy(void *dest, const void *src, uint8_t n);
