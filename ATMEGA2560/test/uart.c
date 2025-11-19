#include "atmega2560.h"

/* // UART FUNCTIONS */

/* #define UART_BAUDRATE 115200 */
/* // Equation for Calculating Baud Rate Register Setting (UBBRn) in asynchronous double speed mode */
/* // BAUD = F_CPU / (8 * (UBRRn + 1)) */
/* // UBRRn = F_CPU / (8 * BAUD) - 1 */
/* #define BAUD_PRESCALER (((F_CPU / (UART_BAUDRATE * 8UL))) - 1) */


/* #define NEW_LINE "\r\n" */

/* void uart_init(void); */

/* void uart_write(char c); */

/* char uart_read(void); */

/* void uart_putstr(const char *str); */

/* void uart_puthex(uint8_t hex); */

/* void uart_putnbr(uint16_t nbr); */

/* void uart_debug_print(const char *str); */

//

void uart_init(void)
{
	// 20.3 Clock Generation (4 modes)
	// Double Speed (asynchronous mode only)
    UCSR0A |= (1 << U2X0);

	// Set Baud Rate
	// 20.11.5 UBRR[11:0] is a 12-bit register which contains the USART baud rate
	UBRR0H = (unsigned char) (BAUD_PRESCALER >> 8);
	UBRR0L = (unsigned char) BAUD_PRESCALER;
	
	// 20.4 Frame Formats
	// 8N1 --> 8 data bits, no parity bit, 1 stop bit
	// Table 20-8 --> USART Mode Select: Asynchronous double speed
	UCSR0C |= (0 << UMSEL00) | (0 << UMSEL01);
	// Table 20-9 --> Parity Mode: Disabled
	UCSR0C |= (0 << UPM00) | (0 << UPM01);
	// Table 20-10 --> Stop Bit Select: 1bit
	UCSR0C |= (0 << USBS0);
	// Table 20-11 --> Character Size: 8bit
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
	
	//20.11.3 Set transmitter bit to 1 (enabled) and set receiver bit to 1 (enabled)
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
}

void uart_write(char c)
{
	// 20.11.2
	// Do nothing until UDR is ready (wait for empty transmit buffer)
	// If UDRE0 bit (bit 5 of UCSR0A) is set to 1 it means the buffer is empty and ready to be written
	// --> we stop loop and write data 
	while (!(UCSR0A & (1 << UDRE0))) {}

	// 20.11.1 UDRn - USART I/O Data Register 
	UDR0 = c;
}

char uart_read(void)
{
	// If RXC0 bit is set to 1 it mean that there is undread data in the receive buffer
	while (!(UCSR0A & (1 << RXC0))) {}

	// return the data received in the receive buffer
	return (UDR0);
}

void uart_putstr(const char *str)
{
	while (*str)
	{
		uart_write(*str++);
	}
}

void uart_puthex(uint8_t hex)
{
	const char *hexa = "0123456789ABCDEF";
	uart_write(hexa[hex >> 4]);
	uart_write(hexa[hex & 0x0F]);
}

void uart_putbin(uint8_t hex)
{
	uint8_t i = 0;
	const char *bin = "01";
	while (i < 8)
	{
		uart_write(bin[1 & (hex >> i)]);
		i++;
	}
}


void uart_putnbr(uint32_t nbr)
{
	if (nbr <= 9)
		uart_write(nbr + '0');
	else
	{
		uart_putnbr(nbr / 10);
		uart_putnbr(nbr % 10);
	}
}


void uart_debug_print(const char *str)
{
    uart_putstr("[DEBUG] ");
    uart_putstr(str);
    uart_putstr(NEW_LINE);
}


char *uart_get_input(char *buffer, uint16_t bufferSize)
{
	char c = 0;
	uint16_t i = 0;
	while (1)
	{
		c = uart_read();
		if (c == END_OF_INPUT)
		{
			buffer[i] = '\0';
			break;
		}
		else if (c == BACKSPACE)
		{
			if (i > 0)
			{
				uart_putstr(REMOVE_CHAR);
				i--;
			}
		}
		else if (i < bufferSize)
		{
			buffer[i] = c;
			i++;
		}
		uart_write(c);
	}
	uart_putstr(NEW_LINE);
	return (buffer);
}
