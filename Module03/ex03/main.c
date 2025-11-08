#include "main.h"


// Table 14-9 Port D Pins Alternate Functions
// PORTD3 Blue --> OC2B (Timer2 B)
// PORTD5 Red --> OC0B (Timer0 B)
// PORTD6 Green --> OC0A (Timer0 A)

ISR(TIMER0_COMPA_vect) {}

ISR(TIMER0_COMPB_vect) {}

ISR(TIMER2_COMPB_vect) {}

void init_uart()
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

void write_uart(char c)
{
	// 20.11.2
	// Do nothing until UDR is ready (wait for empty transmit buffer)
	// If UDRE0 bit (bit 5 of UCSR0A) is set to 1 it means the buffer is empty and ready to be written
	// --> we stop loop and write data 
	while (!(UCSR0A & (1 << UDRE0))) {}

	// 20.11.1 UDRn - USART I/O Data Register 
	UDR0 = c;
}

char read_uart(void)
{
	// If RXC0 bit is set to 1 it mean that there is undread data in the receive buffer
	while (!(UCSR0A & (1 << RXC0))) {}

	// return the data received in the receive buffer
	return (UDR0);
}

void putstr_uart(const char *str)
{
	while (*str)
	{
		write_uart(*str++);
	}
}

void init_rgb(void)
{
	// Set red, green and blue pins of RGB LED to 1 (outpout) 
	DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6);
}

void init_timer(void)
{
	// Set timers: Timer0 (A and B) and Timer2 (B)
	// Timer0
	TCCR0A |= (1 << WGM01) | (1 << WGM00); 	// Mode 3 (fast PWM)
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1); // Compare Output Mode A and B
	TIMSK0 |= (1 << OCIE0B) | (1 << OCIE0A); // Compare match A and B interrupt enable

	OCR0A = 0;
	OCR0B = 0;
	// Timer2
	TCCR2A |= (1 << WGM21) | (1 << WGM20); // Mode 3 (fast PWM)
	TCCR2A |= (1 << COM2B1); // Compare Output Mode B
	TIMSK2 |= (1 << OCIE2B); // Compare match B interrupt enable

	OCR2B = 0;

	// Global interrupt enable
	sei();
 
	// Set prescaler (clock source) to 1024
	// TCCR0B |= (1 << CS00) | (1 << CS02);
	TCCR0B |= (1 << CS01);
	// TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
	TCCR2B |= (1 << CS21);
}


void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	OCR0B = r;
	OCR0A = g;
	OCR2B = b; 
}	

uint8_t ft_strlen(char *buffer)
{
	uint8_t i = 0;
	while (buffer[i])
		i++;
	return (i);
}

// modified to not check first character '#'
uint8_t ft_iscorrectchar(char *buffer)
{
	uint8_t i = 1;
	while (buffer[i])
	{
		if (!((buffer[i] >= '0' && buffer[i] <= '9')
			|| (buffer[i] >= 'A' && buffer[i] <= 'F')))
			return (0);
		i++;
	}
	return (1);
}

char *ft_capitalize(char *str)
{
	uint8_t i = 1;
	while (str[i])
	{
		if (str[i] >= 'a' && str[i] <= 'z')
			str[i] -= ('a' - 'A');
		i++;
	}
	return (str);
}

uint8_t check_input(char *buffer)
{
	uint8_t i = 0;
	if (buffer[0] == '\0' || buffer[0] != '#')
		return (0);
	if (ft_strlen(buffer) != 7)
		return (0);
	buffer = ft_capitalize(buffer);
	if (!ft_iscorrectchar(buffer))
		return (0);
	return (1);
}

uint32_t ft_simple_atoi(char *str)
{
	uint8_t i = 0;
	uint32_t result;
	char c;
	while (str[i])
	{
		uint8_t digit;
		c = str[i];
		
        if (c >= '0' && c <= '9')
            digit = c - '0';
        else if (c >= 'A' && c <= 'F')
            digit = c - 'A' + 10;
        else
            break; // stop at first invalid character

        result = (result << 4) | digit;
		i++;
	}
	return (result);
}

void set_rbg_from_input(char *input)
{
	init_rgb();

	uint32_t color = ft_simple_atoi(input);

	uint8_t red   = (color >> 16) & 0xFF;
    uint8_t green = (color >> 8)  & 0xFF;
    uint8_t blue  =  color        & 0xFF;
	set_rgb(red, green, blue);
}

void get_input(void)
{
	char c = 0;
	uint8_t i = 0;
	char buffer[BUFFER_SIZE];
	while (1)
	{
		c = read_uart();
		if (c == '\r')
		{
			buffer[i] = '\0';
			break;
		}
		else if (c == 127)
		{
			if (i > 0)
			{
				write_uart('\b');
				write_uart(' ');
				write_uart('\b');
				i--;
			}
		}
		else if (i < BUFFER_SIZE)
		{
			buffer[i] = c;
			i++;
		}
		write_uart(c);
	}
	if (!check_input(buffer))
		return;
	else
		set_rbg_from_input(buffer + 1);
	return;
}

void start_input(void)
{
	putstr_uart(TEXT_COLOR_RED);
	putstr_uart(COLOR_SELECTOR_WELCOME);
	putstr_uart(TEXT_COLOR_RESET);
	while (1)
	{
		putstr_uart(REQUEST_COLOR);
		get_input();
		putstr_uart(NEW_LINE);
	}
}

int main(void)
{
	init_timer();
	init_uart();
	start_input();
	while (1) {}
	return 0;
}