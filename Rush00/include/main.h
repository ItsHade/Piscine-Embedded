#pragma once

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

// general register define

#define BitSet(Reg, Bit)	Reg |=  (1 << Bit)
#define BitClr(Reg, Bit)	Reg &= ~(1 << Bit)
#define BitTog(Reg, Bit)	Reg ^=  (1 << Bit)
#define BitCheck(Reg, Bit)	(Reg & (1 << Bit))

// only for types
#include <stdint.h>

#define F_CPU 16000000UL
#define F_SCL 100000UL
#define PRESCALER 1024
#define TOP (F_CPU / (PRESCALER) - 1)
#define TIMER_VALUE 0xFFFF - TOP
#define TWBR_PRESCALER 1 

// RECEIVED BIT

#define SLAVE_READY_BIT 0x01
#define SLAVE_WINNER_BIT 0x06

#define MASTER_WINNER_BIT 0x08

// SENT BIT
#define MONITORING_BIT 0x00
#define START_COUNTDOWN_BIT 0x02


#define ERROR_CODE 0xFF


// winnner
#define MASTER 1
#define SLAVE 0
#define NONE 2

#define WRITE 0
#define READ 1

#define TIMEOUT 200

#define TOGGLE_BLUE (1 << PD3)
#define TOGGLE_RED (1 << PD5)
#define TOGGLE_GREEN (1 << PD6)
#define TOGGLE_YELLOW TOGGLE_GREEN + TOGGLE_RED
#define TOGGLE_CYAN TOGGLE_BLUE + TOGGLE_GREEN
#define TOGGLE_MAGENTA TOGGLE_RED + TOGGLE_BLUE
#define TOGGLE_WHITE TOGGLE_RED + TOGGLE_BLUE + TOGGLE_GREEN


#define TWBR_VALUE ((((F_CPU / F_SCL) / TWBR_PRESCALER) - 16 ) / 2) // 72
// SCL freq = F_CPU / (16 + 2 * TWBR * PRESCALER)
// SCL freq = 16000000 / (16 + 2 * 72 * 1)
// SCL freq = 16000000 / (16 + 144)
// SCL freq = 100000kHz

#define NEW_LINE "\n\r"

#define USAR_BAUDRATE 115200
#define BAUD_PRESCALER (((F_CPU / (USAR_BAUDRATE * 8UL))) - 1)


// Address
// Read bit (high level at SDA) = 1 | Write bit (low level at SDA) = 0

#define SLAVE_ADDR 0x20

// 01000111
#define MICRO_SLAVE_ADDR 0x08

#define SDA_PIN PC4
#define SCL_PIN PC5


/* UART Functions */

// void init_uart(void);

// void write_uart(char c);

// char read_uart(void);

// void putstr_uart(const char *str);

// void	puthex_uart(int8_t hex);

// twi

#define TWI_STATUS TWSR & TW_STATUS_MASK


void debug_print(const char *str);


/* TWI Functions */
void TWI_wait(void);

uint16_t TWI_wait_until(uint16_t ms);

void TWI_status(void);

void TWI_init_slave(void);

void TWI_init_master(void);

int TWI_start(void);

int TWI_write(uint8_t data);

uint8_t TWI_read_no_ack(void);

void TWI_stop(void);


// Timer

void init_timer(void);

uint8_t check_timer(uint8_t counter);