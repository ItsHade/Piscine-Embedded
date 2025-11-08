#pragma once

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define F_CPU 16000000UL
#define PRESCALER 1024

#define USAR_BAUDRATE 115200
// Equation for Calculating Baud Rate Register Setting (UBBRn) in asynchronous double speed mode
// BAUD = F_CPU / (8 * (UBRRn + 1))
// UBRRn = F_CPU / (8 * BAUD) - 1
#define BAUD_PRESCALER (((F_CPU / (USAR_BAUDRATE * 8UL))) - 1)

#define BUFFER_SIZE 10



#define TOGGLE_BLUE (1 << PD3)
#define TOGGLE_RED (1 << PD5)
#define TOGGLE_GREEN (1 << PD6)
#define TOGGLE_YELLOW TOGGLE_GREEN + TOGGLE_RED 
#define TOGGLE_CYAN TOGGLE_BLUE + TOGGLE_GREEN
#define TOGGLE_MAGENTA TOGGLE_RED + TOGGLE_BLUE
#define TOGGLE_WHITE TOGGLE_RED + TOGGLE_BLUE + TOGGLE_GREEN

#define TEXT_COLOR_RESET "\x1B[0m"
#define TEXT_COLOR_RED "\x1B[31m"
#define COLOR_SELECTOR_WELCOME "Welcome to our new color selector 2000\n\r"
#define REQUEST_COLOR "Enter HEX RBG color (format #RRGGBB): "
#define NEW_LINE "\n\r"