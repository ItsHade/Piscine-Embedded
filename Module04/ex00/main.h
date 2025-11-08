#pragma once

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define F_CPU 16000000UL
#define PRESCALER 256
#define FREQUENCY 5 // Hz
#define TOP (F_CPU / (PRESCALER * FREQUENCY) - 1) //12499