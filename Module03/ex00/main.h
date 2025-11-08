#pragma once

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define F_CPU 16000000UL

#define TOGGLE_BLUE (1 << PD3)
#define TOGGLE_RED (1 << PD5)
#define TOGGLE_GREEN (1 << PD6)