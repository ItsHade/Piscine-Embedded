#include "main.h"

char *username = "spectre";
char *password = "spec123!";
char buffer[BUFFER_SIZE];

void uart_init()
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
	// Table 20-8 --> USART Mode Select
	UCSR0C |= (0 << UMSEL00) | (0 << UMSEL01);
	// Table 20-9 --> Parity Mode
	UCSR0C |= (0 << UPM00) | (0 << UPM01);
	// Table 20-10 --> Stop Bit Select
	UCSR0C |= (0 << USBS0);
	// Table 20-11 --> Character Size
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
	
	//20.11.3 Set transmitter bit to 1 (enabled) and set receiver bit to 1 (enabled)
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
}

void uart_tx(char c)
{
	// 20.11.2
	// Do nothing until UDR is ready (wait for empty transmit buffer)
	// If UDRE0 bit (bit 5 of UCSR0A) is set to 1 it means the buffer is empty and ready to be written
	// --> we stop loop and write data 
	while (!(UCSR0A & (1 << UDRE0))) {}

	// 20.11.1 UDRn - USART I/O Data Register 
	UDR0 = c;
}

void uart_printstr(const char *str)
{
	while (*str)
	{
		uart_tx(*str++);
	}
}

char uart_rx(void)
{
	// If RXC0 bit is set to 1 it mean that there is undread data in the receive buffer
	while (!(UCSR0A & (1 << RXC0))) {}

	// return the data received in the receive buffer
	return (UDR0);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while ((s1[i] || s2[i]) && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

uint8_t check_username(void)
{
	char c = 0;
	uint8_t i = 0;
	while (1)
	{
		c = uart_rx();
		if (c == '\r')
		{
			buffer[i] = '\0';
			break;
		}
		else if (c == 127)
		{
			if (i > 0)
			{
				uart_tx('\b');
				uart_tx(' ');
				uart_tx('\b');
				i--;
			}
		}
		else if (i < BUFFER_SIZE)
		{
			buffer[i] = c;
			i++;
		}
		uart_tx(c);
	}
	return (ft_strcmp(buffer, username));
}

uint8_t check_password(void)
{
	char c = 0;
	uint8_t i = 0;
	while (1)
	{
		c = uart_rx();
		if (c == '\r')
		{
			buffer[i] = '\0';
			break;
		}
		else if (c == 127)
		{
			if (i > 0)
			{
				uart_tx('\b');
				uart_tx(' ');
				uart_tx('\b');
				i--;
			}
		}
		else if (i < BUFFER_SIZE)
		{
			buffer[i] = c;
			i++;
		}
		if (c != 127)
			uart_tx('*');
	}
	return (ft_strcmp(buffer, password));
}

void led_loop(void)
{
	while(1)
	{
		PORTB ^= (1 << PB4);
		_delay_ms(50);
		PORTB ^= (1 << PB4);
		PORTB ^= (1 << PB2);
		_delay_ms(50);
		PORTB ^= (1 << PB2);
		PORTB ^= (1 << PB1);
		_delay_ms(50);
		PORTB ^= (1 << PB1);
		PORTB ^= (1 << PB0);
		_delay_ms(50);
		PORTB ^= (1 << PB0);

	}
}

void succes_login(void)
{
	uart_printstr(SUCCESS_LOGIN);
	led_loop();
}

void start_login(void)
{
	uint8_t correct = 0;
	while (1)
	{
		uart_printstr(ENTER_LOGIN);
		uart_printstr(USERNAME);
		correct = check_username();
		uart_printstr(PASSWORD);
		correct += check_password();
		if (correct == 0)
		{
			succes_login();
			break;
		}
		uart_printstr(WRONG_LOGIN);
	}

}



int main(void)
{
	// Setting D1 to D4 LEDs to 1 (output)
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);
	uart_init();
	start_login();
	return 0;
}
