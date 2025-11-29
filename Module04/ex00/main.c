#include "main.h"

// 13. External Interrupts
// bouton SW1 -> PD2 (INT0)

uint8_t switchAvailable = 0;

// Toggle LED D1 on INT0 interrupt if debounce timer is done
ISR(INT0_vect)
{
	if (switchAvailable)
	{
		switchAvailable = 0;
		PORTB ^= (1 << PB0);
		TCNT1 = 0; // reset timer1 to 0

		TCCR1B |= (1 << CS12); // restart timer
	}
}

ISR(TIMER1_COMPA_vect)
{
	TCCR1B &= ~(1 << CS12); // No clock source (Timer/Counter stopped)
	switchAvailable = 1;
}

void init_led()
{
	// Set bit 0 of Port B to 1 (output) --> LED D1
	DDRB |= (1 << PB0);
}

void init_switch()
{
	// Set PD2 as input (default value)
	DDRD &= ~(1 << PD2);
	// Set PD2 to HIGH
	PORTD |= (1 << PD2);

	// Choose mode: falling edge (HIGH to LOW) on INT0 generates an interrupt request
	EICRA |= (1 << ISC01);
	// (default value)
	EICRA &= ~(1 << ISC00);
	// Enable external interrupt 0
	EIMSK |= (1 << INT0);
	// clear pending flag (default value)
	EIFR &= ~(1 << INTF0);

}

void init_timer()
{
	// Timer1
	TCCR1B |= (1 << WGM12); // Mode 2 (CTC)
	TIMSK1 |= (1 << OCIE1A); // Enable Timer1 Compare match A interrupt

	OCR1A = TOP;

	// Timer1 prescaler = 256
	TCCR1B |= (1 << CS12);
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
