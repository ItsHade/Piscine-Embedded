#include "main.h"

// a simple counter to set a flag half second (counter 1)
void    timer1_init(uint8_t freq_hz)
{
    TCNT1 = 0;
     // Set CS12 to one, modifying clock select.
    // CTC mode ; top will be OCR1A
    TCCR1A |= (1 << WGM12);
    // calculate 
    uint16_t top = (F_CPU / (2 * TIMER1_PRESCALER * freq_hz) - 1);
    OCR1A = top;
    // Enable interrupts on timer 1, then interrupts on CTC match for OCR1a 
    TIMSK1 |= (1 << OCIE1A);
    
    // Now timer 1 will increment every 256 ticks
    TCCR1B |= (1 << CS12);
}


void led_init(void)
{
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);
	DDRD |= D5_RED | D5_GREEN | D5_BLUE;
}

int main(void)
{
	uint8_t addr = I2C_EXPANDER_ADDR;
    static uint8_t color_off[3][3] = {SPI_COLOR_OFF, SPI_COLOR_OFF, SPI_COLOR_OFF};
    static uint8_t color_red[3][3] = {SPI_COLOR_RED, SPI_COLOR_RED, SPI_COLOR_RED};
	uint16_t toggle = 0;
	i2c_init();
	timer1_init(2);
	led_init();

	i2c_set_pin(addr, CONFIG_PORT_0_COMMAND, ALL_OUTPUT);
	i2c_set_pin(addr, CONFIG_PORT_1_COMMAND, ALL_OUTPUT);
	spi_init();
    spi_set_led_all(SPI_BRIGHTNESS_MAX, color_off);
	spi_close();

	while (1)
	{

		i2c_print_digits_led(addr, DISPLAY_UPPER_E, DISPLAY_LOWER_R, DISPLAY_LOWER_R, 0, toggle);
		if (TIFR1 & (1 << OCF1A))
		{
			// clear timer1 compare A match flag
			TIFR1 |= (1 << OCF1A);
			toggle = !toggle;
		}
		if (toggle)
		{
			spi_init();
			spi_set_led_all(SPI_BRIGHTNESS_MAX, color_red);
			spi_close();
			PORTB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);
			PORTD |= D5_RED;
		}
		else
		{
			spi_init();
			spi_set_led_all(SPI_BRIGHTNESS_MAX, color_off);
			spi_close();
			PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4));
			PORTD &= ~(D5_RED);
		}

	}
}

