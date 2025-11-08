#pragma once

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define F_CPU 16000000UL
#define TIMER1_PRESCALER 1UL
#define TIMER1_FREQUENCY 100000UL // Hz
#define TIMER1_TOP (F_CPU / (TIMER1_PRESCALER * TIMER1_FREQUENCY) - 1) // 160

#define TIMER0_PRESCALER 256UL
#define TIMER0_FREQUENCY 510UL
#define TIMER0_TOP (F_CPU / (TIMER0_PRESCALER * TIMER0_FREQUENCY) - 1) // 122.5
#define TIMER0_MAX 0xFF



// #define TIMER1_PRESCALER 256
#define TOP (F_CPU / (PRESCALER * 1) - 1)
#define MIN_DUTY_CYCLE 0
#define MAX_DUTY_CYCLE 255
#define DUTY_CYCLE_INCREMENT 1

