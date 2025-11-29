#include "main.h"


// a simple counter to set a flag half second (counter 1)
void    init_timer1(uint8_t freq_hz)
{
    TCNT1 = 0;
     // Set CS12 to one, modifying clock select.
    // CTC mode ; top will be OCR1A
    TCCR1B |= (1 << WGM12);
    // calculate 
    uint16_t top = (F_CPU / (2 * TIMER1_PRESCALER * freq_hz) - 1);
    OCR1A = top;
    // Enable interrupts on timer 1, then interrupts on CTC match for OCR1a 
    TIMSK1 |= (1 << OCIE1A);
    
    // Now timer 1 will increment every 256 ticks
    TCCR1B |= (1 << CS12);
}

int main(void)
{
	uint8_t addr = I2C_EXPANDER_ADDR;
    uint8_t display[12] = {DISPLAY_0, DISPLAY_1, DISPLAY_2, DISPLAY_3, DISPLAY_4, DISPLAY_5, DISPLAY_6, DISPLAY_7, DISPLAY_8, DISPLAY_9, DISPLAY_DASH, DISPLAY_DOT};
	uint16_t number = 0;
	i2c_init();
	init_timer1(1);

	i2c_set_pin(addr, CONFIG_PORT_0_COMMAND, ALL_OUTPUT);
	i2c_set_pin(addr, CONFIG_PORT_1_COMMAND, ALL_OUTPUT);
	while (1)
	{
		if (number >= 10000)
			number = 0;
		i2c_print_digits(addr, display[number / 1000 % 10], display[number / 100 % 10], display[number / 10 % 10], display[number % 10]);
		if (TIFR1 & (1 << OCF1A))
		{
			// clear timer1 compare A match flag
			TIFR1 |= (1 << OCF1A);
			number++;
		}
	}
}

