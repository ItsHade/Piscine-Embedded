#include "main.h"

void display_mode_binary(uint8_t addr, uint8_t counter)
{
	uint8_t port0 = 0xFF;
	// default value is 1 (OFF)
	// 0 means ON
	if (counter & 0x01)
		port0 &= ~LED_D11;
	if (counter & 0x02)
		port0 &= ~LED_D10;
	if (counter & 0x04)
		port0 &= ~LED_D9;
	i2c_set_pin(addr, OUTPUT_PORT_0_COMMAND, port0);
}

uint8_t check_switch(uint8_t addr, uint8_t counter)
{
	static uint8_t switch3Pressed = 0;
	if (!(i2c_get_pin(addr, INPUT_PORT_0_COMMAND) & SW3) && !switch3Pressed)
	{
		switch3Pressed = 1;
		counter++;
	}
	if ((i2c_get_pin(addr, INPUT_PORT_0_COMMAND) & SW3))
		switch3Pressed = 0;

	if (switch3Pressed)
		_delay_ms(150); // to test
	return (counter);
}

int main(void)
{
	uint8_t addr = I2C_EXPANDER_ADDR;
	uint8_t counter = 0;
	i2c_init();

	i2c_set_pin(addr, CONFIG_PORT_0_COMMAND, ALL_OUTPUT_SWITCH_INPUT);
	while (1)
	{
		counter = check_switch(addr, counter);
		display_mode_binary(addr, counter);

	}
}

