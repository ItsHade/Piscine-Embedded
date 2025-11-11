#include "main.h"

int main(void)
{
	uint8_t addr = I2C_EXPANDER_ADDR;
	uint8_t display[10] = {DISPLAY_0, DISPLAY_1, DISPLAY_2, DISPLAY_3, DISPLAY_4, DISPLAY_5, DISPLAY_6, DISPLAY_7, DISPLAY_8, DISPLAY_9};
	uint8_t i = 0;
	i2c_init();

	i2c_set_pin(addr, CONFIG_PORT_0_COMMAND, ALL_OUTPUT);
	i2c_set_pin(addr, CONFIG_PORT_1_COMMAND, ALL_OUTPUT);
	i2c_set_pin(addr, OUTPUT_PORT_0_COMMAND, CA4);
	while (1)
	{
		if (i >= DIGITS_NUMBER)
			i = 0;
		i2c_set_pin(addr, OUTPUT_PORT_1_COMMAND, display[i]);
		i++;
		_delay_ms(1000);
	}
}

