#include "main.h"

// 20. USART0

// Table 14-9
// TXD (USART Output Pin) is alternate function of PD1
// RXD (USART Input Pin) is alternate function of PD0

void timer1_init()
{
	// Setup of Timer1
	// Set WGM12 bit to 1 to enable CTC mode
	TCCR1B |= (1 << WGM12);
	// Set prescaler to 1024
	TCCR1B |= (1 << CS12) | (1 << CS10);

	// OCR1A register defines the TOP value (maximum Timer value)
	OCR1A = TOP;

	// 16.11.8 Timer1 Interrupt Mask Register
	// Set the OCIE1A bit to 1 to enable the Timer1 Output Compare A Match
	TIMSK1 |= (1 << OCIE1A);

	// Set the I bit (flag) to 1 in the Status Register
	sei();
}

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

	
	//20.11.3 Set transmitter bit to 1 (enabled)
	UCSR0B |= (1 << TXEN0);

}

void uart_tx(char c)
{
	// 20.11.2
	// Do nothing until UDR is ready (wait for empty transmit buffer)
	// If UDRE0 bit (bit 5 of UCSR0A) is set to 1 it means the buffer is empty and ready to be written
	// --> we stop loop and write data 
	while (!( UCSR0A & (1 << UDRE0))) {}

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

// Interrupt Service Routine
// Runs every times TIFR1 is set (everytime the TIMER1_COMPA interrupt is raised)
ISR(TIMER1_COMPA_vect)
{
	uart_printstr("Hello World!\n\r");
}


int main(void)
{
	timer1_init();

	uart_init();
	while (1){}
	return 0;
}