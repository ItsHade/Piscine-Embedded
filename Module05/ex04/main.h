#pragma once

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define F_CPU 16000000UL
#define PRESCALER 1024
#define TOP (F_CPU / PRESCALER) - 1

#define ADR_8BIT_MAX 0xFF
#define ADR_8BIT_MIN 0x00

