#include "../includes/prog.h"

uint8_t EEPROM_read(uint32_t address)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE))
	{}

	/* Set up address register */
	EEAR = address;

	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);

	/* Return data from Data Register */
	return EEDR;
}

void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE))
	{}

	/* Set up address and Data Registers */
	EEAR = uiAddress;
	EEDR = ucData;

	/* Write logical one to EEMPE */
	EECR |= (1<<EEMPE);

	/* Start eeprom write by setting EEPE */
	EECR |= (1<<EEPE);
}

void EEPROM_write_data(uint32_t addr, void *data, size_t size)
{
	uint8_t *raw_data = (uint8_t*)data;
	for(size_t i = 0; i < size; i++)
		EEPROM_write(addr + i, raw_data[i]);
}

void EEPROM_read_data(uint32_t addr, uint8_t *buffer, size_t buffer_size)
{
	for(size_t i = 0; i < buffer_size; i++)
		buffer[i] = EEPROM_read(addr + i);
}

uint32_t EEPROM_find_memblock(size_t data_size)
{
	uint32_t addr = 0x00;
	while(addr <= E2END)
	{
		if(EEPROM_read(addr) == DATA_STRUCT_SIGNATURE)
			addr += data_size;
		else
			return addr;
	}
	return E2END;
}

static void display_addr_Hex_val(uint32_t start_addr, uint32_t end_addr, uint8_t bytes_per_line)
{
	uint8_t byte_nbr = 0;
	while(byte_nbr < bytes_per_line && start_addr <= end_addr)
	{
		uart_hex8(EEPROM_read(start_addr++));
		if(byte_nbr != 0 && byte_nbr % 2 != 0 )
			uart_tx(' ');
		byte_nbr++;
	}
}

static void display_addr_ASCII_val(uint32_t start_addr, uint32_t end_addr, uint8_t bytes_per_line)
{
	uint8_t byte_nbr = 0;
	uart_printstr((uint8_t *)" |");
	while(byte_nbr < bytes_per_line && start_addr <= end_addr)
	{
		uint8_t c = EEPROM_read(start_addr++);
		if (c >= 32 && c <= 126)
			uart_tx(c);
		else
			uart_tx(' ');
		byte_nbr++;
	}
	uart_printstr((uint8_t *)"|\n\r");
}

void EEPROM_hexdump(uint32_t start_addr, uint32_t end_addr, uint8_t bytes_per_line)
{
	while(start_addr <= end_addr)
	{
		display_hexa_32b_addr(start_addr);
		uart_printstr((uint8_t *)": ");
		display_addr_Hex_val(start_addr, end_addr, bytes_per_line);
		display_addr_ASCII_val(start_addr, end_addr, bytes_per_line);
		start_addr += bytes_per_line;
	}
}
