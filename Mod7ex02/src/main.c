#include "../includes/prog.h"

//READ
static inline void display_map_value(uint8_t *key)
{
	uint32_t addr = 0x00;

	while(addr <= E2END)
	{
		if(EEPROM_read(addr) == DATA_STRUCT_SIGNATURE)
		{
			uint8_t buffer[32];
			EEPROM_read_data(addr + 1, buffer, sizeof(buffer));
			if(!str_comp(buffer, key))
			{
				addr += sizeof(buffer) + 1;
				EEPROM_read_data(addr, buffer, sizeof(buffer));
				uart_printstr(buffer);
				uart_printstr((uint8_t *)"\n\r");
				return;
			}
		}
		addr += sizeof(t_eeprom_entry);
	}
	uart_printstr((uint8_t *)"Empty\n\r");
}

static inline uint8_t find_key(uint8_t *key)
{
	uint32_t addr = 0x00;

	while(addr <= E2END)
	{
		if(EEPROM_read(addr) == DATA_STRUCT_SIGNATURE)
		{
			uint8_t buffer[32];
			EEPROM_read_data(addr + 1, buffer, sizeof(buffer));
			if(!str_comp(buffer, key))
			{
				addr += sizeof(buffer);
				EEPROM_read_data(addr, buffer, sizeof(buffer));
				return 1;
			}
		}
		addr += sizeof(t_eeprom_entry);
	}
	return 0;
}

//WRITE
static inline void create_map_entry(uint8_t *new_key, uint8_t *new_value)
{
	if(find_key(new_key))
	{
		uart_printstr((uint8_t *)"already exists\n\r");
		return;
	}

	t_eeprom_entry new_entry;
	new_entry.data_signature = DATA_STRUCT_SIGNATURE;
	str_cpy(new_entry.key, new_key);
	str_cpy(new_entry.value, new_value);

	uint16_t addr = EEPROM_find_memblock(sizeof(new_entry));
	if (addr != E2END)
	{
		EEPROM_write_data(addr, (void *)&new_entry, sizeof(new_entry));
		uart_printstr((uint8_t *)"Block successfully written at address: \n\r");
		display_hexa_32b_addr(addr);
		uart_printstr((uint8_t *)"\n\r");
	}
	else
		uart_printstr((uint8_t *)"No space left\n\r");
}

//FORGET
static inline void delete_map_entry(uint8_t *key)
{
	uint32_t addr = 0x00;

	while(addr <= E2END)
	{
		if(EEPROM_read(addr) == DATA_STRUCT_SIGNATURE)
		{
			uint8_t buffer[32];
			EEPROM_read_data(addr + 1, buffer, sizeof(buffer));
			if(!str_comp(buffer, key))
			{
				EEPROM_write(addr, '\0');
				return;
			}
		}
		addr += sizeof(t_eeprom_entry);
	}
	uart_printstr((uint8_t *)"Not found\n\r");
}

static inline void parse_cmd(uint8_t words[NB_WORDS][WORD_MAX_SIZE])
{
	if(!str_comp(words[0], (uint8_t *)"READ"))
		display_map_value(words[1]);
	else if(!str_comp(words[0], (uint8_t *)"WRITE"))
		create_map_entry(words[1], words[2]);
	else if(!str_comp(words[0], (uint8_t *)"FORGET"))
	 	delete_map_entry(words[1]);
	else if(!str_comp(words[0], (uint8_t *)"PRINT"))
	 	EEPROM_hexdump(0x00, E2END, 16);
	else
		uart_printstr((uint8_t *)"Unknown kewyword\n\r");
}

int main()
{
	uart_init(CLOCK_SPEED, BAUD_RATE);
	uint8_t line[255];
	uint8_t words[NB_WORDS][WORD_MAX_SIZE];
	while (1)
	{
		uart_getline(line, 1024);
		uart_printstr((uint8_t *)"\n\r");
		if (splitline(line, words))
			parse_cmd(words);
	}
}

