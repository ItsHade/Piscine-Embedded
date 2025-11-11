#include "main.h"

int main(void)
{
	uint8_t addr = I2C_EXPANDER_ADDR;
	i2c_init();
	
	i2c_set_pin(addr, CONFIG_PORT_0_COMMAND, ALL_OUTPUT);
	while (1)
	{
		i2c_set_pin(addr, OUTPUT_PORT_0_COMMAND, ONLY_D9);
		_delay_ms(100); // blink at 1hz means nothing because how much time does it stays on for 
		i2c_set_pin(addr, OUTPUT_PORT_0_COMMAND, PORT_0_ALL_OFF);
		_delay_ms(900); //900 + 100 = 1000 1000ms between each activation of LED D9
	}
}

