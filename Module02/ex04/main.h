#pragma once

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define ENTER_LOGIN "Enter your login:\n\r"
#define USERNAME "username: "
#define PASSWORD "\n\rpassword: "
#define WRONG_LOGIN "\n\rBad combinaison username/password\n\n\r"
#define SUCCESS_LOGIN "\n\rHello spectre!\n\rShall we play a game?\n\r"
#define BUFFER_SIZE 20

#define F_CPU 16000000UL
#define PRESCALER 1024
#define USAR_BAUDRATE 115200
// Equation for Calculating Baud Rate Register Setting (UBBRn) in asynchronous double speed mode
// BAUD = F_CPU / (8 * (UBRRn + 1))
// UBRRn = F_CPU / (8 * BAUD) - 1
#define BAUD_PRESCALER (((F_CPU / (USAR_BAUDRATE * 8UL))) - 1)
