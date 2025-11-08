#include "main.h"


// Table 14-9 Port D Pins Alternate Functions
// PORTD3 Blue --> OC2B (Timer2 B)
// PORTD5 Red --> OC0B (Timer0 B)
// PORTD6 Green --> OC0A (Timer0 A)

ISR(TIMER0_COMPA_vect) {}

ISR(TIMER0_COMPB_vect) {}

ISR(TIMER2_COMPB_vect) {}
	
void init_rgb()
{
	// Set red, green and blue pins of RGB LED to 1 (outpout) 
	DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6);

	// Set timers: Timer0 (A and B) and Timer2 (B)
	// Timer0
	TCCR0A |= (1 << WGM01) | (1 << WGM00); 	// Mode 3 (fast PWM)
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1); // Compare Output Mode A and B
	TIMSK0 |= (1 << OCIE0B) | (1 << OCIE0A); // Compare match A and B interrupt enable

	OCR0A = 0;
	OCR0B = 0;

	// Timer2
	TCCR2A |= (1 << WGM21) | (1 << WGM20); // Mode 3 (fast PWM)
	TCCR2A |= (1 << COM2B1); // Compare Output Mode B
	TIMSK2 |= (1 << OCIE2B); // Compare match B interrupt enable

	OCR2B = 0;

	// Global interrupt enable
	sei();
 
	// Set prescaler (clock source) to 1024
	TCCR0B |= (1 << CS00) | (1 << CS02);
	TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
}


void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	OCR0B = r;
	OCR0A = g;
	OCR2B = b; 
}

void wheel(uint8_t pos) {
	pos = 255 - pos;
	if (pos < 85) 
	{
		set_rgb(255 - pos * 3, 0, pos * 3);
	} 
	else if (pos < 170) 
	{
		pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3);
	} 
	else 
	{
		pos = pos - 170;
		set_rgb(pos * 3, 255 - pos * 3, 0);
	}
}

int main(void)
{
	init_rgb();
	uint8_t pos = 0;
	while (1)
	{
		wheel(pos);
		pos++;
		_delay_ms(60);
	}
	return 0;
}