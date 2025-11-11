#include "main.h"


int main(void)
{
	uint8_t addr = I2C_EXPANDER_ADDR;
    uint8_t display[12] = {DISPLAY_0, DISPLAY_1, DISPLAY_2, DISPLAY_3, DISPLAY_4, DISPLAY_5, DISPLAY_6, DISPLAY_7, DISPLAY_8, DISPLAY_9, DISPLAY_DASH, DISPLAY_DOT};
	uint16_t read = 0;
	i2c_init();
	init_adc();

	i2c_set_pin(addr, CONFIG_PORT_0_COMMAND, ALL_OUTPUT);
	i2c_set_pin(addr, CONFIG_PORT_1_COMMAND, ALL_OUTPUT);
	while (1)
	{
		read = read_adc();
		i2c_print_digits(addr, display[read / 1000 % 10], display[read / 100 % 10], display[read / 10 % 10], display[read % 10]);
	}
}

