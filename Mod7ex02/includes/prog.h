#ifndef PROG_H
# define PROG_H

// ---------- INCLUDES ----------
# include <avr/eeprom.h>
# include <avr/io.h>

// ---------- DEFINES ----------
# define CLOCK_SPEED 16000000UL
# define BAUD_RATE 115200


# define START_ADDR 0x00

// start of struct data signature
# define DATA_STRUCT_SIGNATURE 0x7f

# define NB_WORDS 3
# define WORD_MAX_SIZE 32

// ---------- struct ----------

typedef struct s_eeprom_entry
{
	uint8_t	data_signature;
	uint8_t	key[32];
	uint8_t	value[32];
}			t_eeprom_entry;

// ---------- uart.c ----------
void		uart_tx(uint8_t c);
uint8_t		uart_rx(void);
void		uart_getline(uint8_t *line, uint32_t size);
void		uart_hex8(uint8_t value);
void		uart_printstr(const unsigned char *str);
void		uart_init(uint32_t clock_speed, uint32_t baud_rate);

// ---------- eeprom.c ----------
void		EEPROM_write(unsigned int uiAddress, unsigned char ucData);
uint8_t		EEPROM_read(uint32_t address);
void		EEPROM_write_data(uint32_t addr, void *data, size_t size);
void		EEPROM_read_data(uint32_t addr, uint8_t *buffer,
				size_t buffer_size);
uint32_t	EEPROM_find_memblock(size_t data_size);
void		EEPROM_hexdump(uint32_t start_addr, uint32_t end_addr,
				uint8_t bytes_per_line);

// ---------- utils.c ----------
void		str_cpy(uint8_t *dst, uint8_t *src);
uint8_t		str_comp(uint8_t *s1, uint8_t *s2);
uint8_t		splitline(uint8_t *line, uint8_t words[NB_WORDS][WORD_MAX_SIZE]);
uint8_t		word_len(uint8_t *str);
void		display_hexa_16b_addr(uint16_t addr);
void		display_hexa_32b_addr(uint32_t addr);

#endif
