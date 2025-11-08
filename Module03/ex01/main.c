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
		toggle_color(TOGGLE_YELLOW);
		_delay_ms(1000);
		toggle_color(TOGGLE_YELLOW);
		toggle_color(TOGGLE_CYAN);
		_delay_ms(1000);
		toggle_color(TOGGLE_CYAN);
		toggle_color(TOGGLE_MAGENTA);
		_delay_ms(1000);
		toggle_color(TOGGLE_MAGENTA);
		toggle_color(TOGGLE_WHITE);
		_delay_ms(1000);
		toggle_color(TOGGLE_WHITE);
	}
	return 0;
}