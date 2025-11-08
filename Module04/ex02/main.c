#include "main.h"

uint8_t counter = 0;
uint8_t switchAvailable = 0;


void update_display(void)
{
	uint8_t mask = counter;
	if (mask & (1 << 3))
	{
		mask &= ~(1 << PB3);
		mask |= (1 << PB4);
	}
	PORTB = mask;
}


//SW1
ISR(INT0_vect)
{
	if (switchAvailable)
	{
		switchAvailable = 0;
		if (counter < MAX_DISPLAY_VALUE)
			counter++;
		update_display();

		TCNT1 = 0; // reset timer1 to 0
		TCCR1B |= (1 << CS12); // restart timer
	}
}

//SW2
ISR(PCINT2_vect)
{
	if (switchAvailable)
	{
		switchAvailable = 0;
		if (counter > MIN_DISPLAY_VALUE)
			counter--;
		update_display();

		TCNT1 = 0; // reset timer1 to 0
		TCCR1B |= (1 << CS12); // restart timer
	}
}

ISR(TIMER1_COMPA_vect)
{
	TCCR1B &= ~(1 << CS12); // No clock source (Timer/Counter stopped)
	switchAvailable = 1;
}

void init_timer()
{
	// Timer1
	TCCR1A |= (1 << WGM12); // Mode 2 (CTC)
	TIMSK1 |= (1 << OCIE1A); // Enable Timer1 Compare match A interrupt

	OCR1A = TOP;

	// Timer1 prescaler = 256
	TCCR1B |= (1 << CS12); 
}

void init_led()
{
	// Set LED D1, D2, D3 and D4 as output
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);
}

void init_switch()
{
	// Set PD2 and PD4 as input (default value)
	DDRD &= ~(1 << PD2);
	DDRD &= ~(1 << PD4);

	// Set PD2 and PD4 to HIGH
	PORTD |= (1 << PD2);
	PORTD |= (1 << PD4);
	
	// Switch 1 (PD2 / INT0)
	// Choose mode: falling edge (HIGH to LOW) on INT0 generates an interrupt request
	EICRA |= (1 << ISC01);
	// (default value)
	EICRA &= ~(1 << ISC00);
	// Enable external interrupt 0
	EIMSK |= (1 << INT0);
	// clear pending flag (default value)
	EIFR &= ~(1 << INTF0);

	// Switch 2 (PD4 / PCINT20)
	//  13. External interrupts: PCI2 will trigger if any enabled PCINT[23:16] pin toggles
	// Enable pin change interrupt 2 (PCI2)
	PCICR |= (1 << PCIE2);
	// Enable pin change mask 20
	PCMSK2 |= (1 << PCINT20);
	// clear pending flag
	PCIFR &= ~(1 << PCIF2);
}


int main(void)
{
	init_led();
	init_switch();
	init_timer();

	// Enable global interrupt
	sei();

	while (1){}
	return 0;
}