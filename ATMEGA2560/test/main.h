#pragma once

#include <avr/io.h>
#include <util/delay.h>

// only for types
#include <stdint.h>
#include <stddef.h>

/*
    uint8_t tab[12] = {DISPLAY_0, DISPLAY_1, DISPLAY_2, DISPLAY_3, DISPLAY_4, DISPLAY_5, DISPLAY_6, DISPLAY_7, DISPLAY_8, DISPLAY_9, DISPLAY_DASH, DISPLAY_DOT};
    i2c_print_digits(I2C_EXPANDER_ADDR, tab[day / 10], tab[day % 10], tab[month / 10], tab[month % 10]);

*/


#define F_CPU 16000000UL
#define F_SCL 100000UL
#define TWBR_PRESCALER 1 
#define TWBR_VALUE ((((F_CPU / F_SCL) / TWBR_PRESCALER) - 16 ) / 2) // 72

#define IS_PRESSED(pin) (!(PIND & (1 << (pin))))

#define DIGITS_NUMBER 10

#define TIMER1_PRESCALER 256


#define MAX_ADC_VALUE 1023

// Address
// Read bit (high level at SDA) = 1 | Write bit (low level at SDA) = 0
#define I2C_EXPANDER_ADDR 0x20

#define I2C_WRITE 0
#define I2C_READ 1

#define INPUT_PORT_0_COMMAND 0x00
#define OUTPUT_PORT_0_COMMAND 0x02
#define OUTPUT_PORT_1_COMMAND 0x03
#define CONFIG_PORT_0_COMMAND 0x06
#define CONFIG_PORT_1_COMMAND 0x07 
#define ALL_OUTPUT 0x00
#define ALL_OUTPUT_SWITCH_INPUT 0x01
#define ALL_INPUT 0xFF
#define PORT_0_ALL_OFF 0xFF
#define PORT_0_ALL_ON 0x00
#define PORT_0_ONLY_DIGITS_ON 0x0F
#define PORT_0_DIGITS_OFF 0xF0

#define PORT_1_ALL_OFF 0x00
#define PORT_1_ALL_ON 0xFF

#define D5_RED (1 << PD5)
#define D5_GREEN (1 << PD6)
#define D5_BLUE (1 << PD3)

#define LED_D9 0x08
#define LED_D10 0x04
#define LED_D11 0x02

// Need rework
#define ONLY_D9 0xF7
#define ONLY_D10 0xFB
#define ONLY_D11 0xFD

#define D9_AND_DIGITS 0x07
#define D10_AND_DIGITS 0x0B
#define D11_AND_DIGITS 0x0D

#define SW3 0x01


// devkit names with binary value to put on
// USE: Setting CA to 0 means that all 7 segment + dot is ON
// Default value is 1 (OFF)
enum Port0
{
    CA1 = 0xEF,
    CA2 = 0xDF,
    CA3 = 0xBF,
    CA4 = 0x7F,
    CA1_AND_LED = 0xE1,
    CA2_AND_LED = 0xD1,
    CA3_AND_LED = 0xB1,
    CA4_AND_LED = 0x71
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
    DISPLAY_DOT = 0x80,
    DISPLAY_UPPER_E = 0x79,
    DISPLAY_LOWER_R = 0x50
};

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

// === I2C FUNCTIONS ===


void i2c_init(void);

void i2c_start(void);

 void i2c_stop(void);

 void i2c_write(unsigned char data);

 uint8_t i2c_read(void);

 uint8_t i2c_read_nack(void);

void    i2c_multiwrite(uint8_t *data, uint8_t size);

void    i2c_multiread(uint8_t *data, uint8_t size);

// 7SEGMENTS DISPLAY

void i2c_print_digits(uint8_t addr, uint8_t digit1, uint8_t digit2, uint8_t digit3, uint8_t digit4);

void i2c_print_digits_led(uint8_t addr, uint8_t digit1, uint8_t digit2, uint8_t digit3, uint8_t digit4, uint8_t ledState);

void i2c_set_pin(uint8_t addr, uint8_t command, uint8_t port);

void i2c_set_pins(uint8_t addr, uint8_t command, uint8_t port0, uint8_t port1);

uint8_t i2c_get_pin(uint8_t addr, uint8_t command);


// === SPI FUNCTIONS === 

void spi_init(void);

void spi_write(uint8_t write);

void spi_close(void);

void spi_set_led_all(uint8_t brightness, uint8_t color[3][3]);

void spi_set_led(uint8_t led, uint8_t brightness, uint8_t color[3]);

