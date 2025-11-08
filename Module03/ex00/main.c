#include "main.h"

void toggle_color(uint8_t color)
{
	PORTD ^= color;
}

int main(void)
{
	// PORTD3 -> blue
	// PORTD5 -> red
	// PORTD6 -> green
	DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6);

	while (1)
	{
		toggle_color(TOGGLE_RED);
		_delay_ms(1000);
		toggle_color(TOGGLE_RED);
		toggle_color(TOGGLE_GREEN);
		_delay_ms(1000);
		toggle_color(TOGGLE_GREEN);
		toggle_color(TOGGLE_BLUE);
		_delay_ms(1000);
		toggle_color(TOGGLE_BLUE);

		// PORTD ^= (1 << PD5);
		// _delay_ms(1000);
		// PORTD ^= (1 << PD5);
		// PORTD ^= (1 << PD6);
		// _delay_ms(1000);
		// PORTD ^= (1 << PD6);
		// PORTD ^= (1 << PD3);
		// _delay_ms(1000);
		// PORTD ^= (1 << PD3);
	}
	return 0;
}