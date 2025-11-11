#include "../includes/prog.h"

void uart_getline(uint8_t *line, uint32_t size)
{
	uint8_t pos = 0;

	while(pos < size)
	{
		uint8_t c = uart_rx();
		if(c == 13)
		{
			line[pos] = '\0';
			break ;
		}
		else if(c == 127 && pos > 0)
		{
			uart_printstr((uint8_t *)"\b\x1b[P");
			line[pos] = '\0';
			pos--;
		}
		else
		{
			uart_tx(c);
			line[pos] = c;
			pos++;
		}
	}
}

//Write a char in I/O buffer
void uart_tx(uint8_t c)
{
	//waiting for empty buffer register
	while (!( UCSR0A & (1 << UDRE0)))
	{}
	UDR0 = c;
}

uint8_t	uart_rx(void)
{
	while (!(UCSR0A & (1 << RXC0)))
		;
	return UDR0;
}

void uart_hex8(uint8_t value)
{
	const char hex[] = "0123456789abcdef";

	uart_tx(hex[(value >> 4) & 0x0F]);
	uart_tx(hex[value & 0x0F]);
}

// Write a string in I/O buffer
void uart_printstr(const unsigned char *str)
{
	while (*str)
		uart_tx(*str++);
}

//init UART protocl
void uart_init(uint32_t clock_speed, uint32_t baud_rate)
{
	UCSR0A |= (1 << U2X0); // switch to x2 speed mode

	//set the baud rate (cf. define and datasheet 20.6)
	uint32_t ubrr_value = (clock_speed / (8 * baud_rate)) - 1;
	UBRR0H = (uint8_t)(ubrr_value >> 8);
	UBRR0L = (uint8_t)ubrr_value;

	UCSR0B |= (1 << TXEN0); //enabling transmitter mode
	UCSR0B |= (1 << RXEN0); // enabling receiving mode

	//setting frame formatter (8 bits transmission, no parity bit, 1 stop bit)
	UCSR0C |= (1 << UCSZ01);
	UCSR0C |= (1 << UCSZ00);
}
