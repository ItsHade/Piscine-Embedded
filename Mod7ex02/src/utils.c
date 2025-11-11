#include "../includes/prog.h"

void	str_cpy(uint8_t *dst, uint8_t *src)
{
	for (uint8_t i = 0; i < 32; i++)
		dst[i] = 0;
	for (uint8_t i = 0; src[i]; i++)
		dst[i] = src[i];
}

void	display_hexa_16b_addr(uint16_t addr)
{
	uart_hex8((uint8_t)((addr & 0xF0) >> 4));
	uart_hex8((uint8_t)(addr & 0xF));
}

void	display_hexa_32b_addr(uint32_t addr)
{
	uint16_t	left;
	uint16_t	right;

	left = (uint16_t)((addr & 0xFFFF0000) >> 16);
	right = (uint16_t)(addr & 0xFFFF);
	uart_hex8((uint8_t)((left & 0xFF00) >> 8));
	uart_hex8((uint8_t)(left & 0xFF));
	uart_hex8((uint8_t)((right & 0xFF00) >> 8));
	uart_hex8((uint8_t)(right & 0xFF));
}

uint8_t	word_len(uint8_t *str)
{
	size_t	size;

	size = 0;
	while (*str++)
		size++;
	return (size);
}

uint8_t	str_comp(uint8_t *s1, uint8_t *s2)
{
	uint8_t	s1_size;
	uint8_t	s2_size;

	s1_size = word_len(s1);
	s2_size = word_len(s2);
	if (s1_size != s2_size)
		return 1;
	for (uint8_t i = 0; i < s1_size; i++)
	{
		if (s1[i] != s2[i])
			return 1;
	}
	return 0;
}

static uint8_t	put_key_value(uint8_t **line, uint8_t *buffer,
		uint8_t buffer_size)
{
	uint8_t	i;
	uint8_t	quoted;

	i = 0;
	quoted = 0;
	if (buffer_size == 0)
		return 1;
	if (**line == '"')
	{
		quoted = 1;
		(*line)++;
	}
	else
		return -1;
	while (**line)
	{
		if(i > buffer_size)
			return 1;
		if (quoted)
		{
			if (**line == '"')
			{
				(*line)++;
				break ;
			}
			buffer[i++] = **line;
			(*line)++;
		}
		else
		{
			if (**line <= ' ')
				break ;
			buffer[i++] = **line;
			(*line)++;
		}
	}
	buffer[i] = '\0';
	return 0;
}

static inline void	put_keyword(uint8_t **line, uint8_t *buffer,
		uint8_t buffer_size)
{
	uint8_t	i;

	i = 0;
	if (buffer_size == 0)
		return ;
	while (**line && **line > ' ' && i + 1 < buffer_size)
	{
		buffer[i++] = **line;
		(*line)++;
	}
	buffer[i] = '\0';
}

uint8_t	splitline(uint8_t *line, uint8_t words[NB_WORDS][WORD_MAX_SIZE])
{
	for (uint8_t i = 0; i < NB_WORDS; i++)
	{
		while (*line && *line <= ' ')
			line++;
		if (i == 0)
			put_keyword(&line, words[i], WORD_MAX_SIZE -1);
		else
		{
			if (put_key_value(&line, words[i], WORD_MAX_SIZE -1) == 1)
				return 0;
		}
	}
	return 1;
}
