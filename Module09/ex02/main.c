#include "main.h"

int main(void)
{
	uint8_t addr = I2C_EXPANDER_ADDR;
	i2c_init();

	i2c_set_pin(addr, CONFIG_PORT_0_COMMAND, ALL_OUTPUT);
	i2c_set_pin(addr, CONFIG_PORT_1_COMMAND, ALL_OUTPUT);
	i2c_set_pin(addr, OUTPUT_PORT_0_COMMAND, CA4);
	i2c_set_pin(addr, OUTPUT_PORT_1_COMMAND, DISPLAY_2);
}

