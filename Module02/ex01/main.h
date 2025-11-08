#pragma once

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define PRESCALER 1024
#define USAR_BAUDRATE 115200
// Equation for Calculating Baud Rate Register Setting (UBBRn) in asynchronous double speed mode
// BAUD = F_CPU / (8 * (UBRRn + 1))
// UBRRn = F_CPU / (8 * BAUD) - 1
#define BAUD_PRESCALER (((F_CPU / (USAR_BAUDRATE * 8UL))) - 1)

// TIMER_FREQ is the Timer1 tick frequency (every time is count +1)
// here it's 15625Hz --> counts +1 15625 times per second and activate TIMER1_COMP_A every seconds
#define TIMER_FREQ (F_CPU / PRESCALER)
// multiplied by 2 --> 2s
// -1 comes from how CTC mode works (from 0 to OCR1A and not 1 to OCR1A)
#define TOP (TIMER_FREQ * 2 - 1)