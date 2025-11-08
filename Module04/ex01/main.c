#include "main.h"


// LED D2 -> PB1 (OC1A)
uint8_t dutyCycle = MIN_DUTY_CYCLE;
uint8_t goingUp = 1;

ISR(TIMER0_COMPA_vect)
{
	if (dutyCycle >= MAX_DUTY_CYCLE)
		goingUp = 0;
	else if (dutyCycle <= MIN_DUTY_CYCLE)
		goingUp = 1;

	if (goingUp && dutyCycle < MAX_DUTY_CYCLE)
	{
		dutyCycle += DUTY_CYCLE_INCREMENT;
	}
	else if (!goingUp && dutyCycle > MIN_DUTY_CYCLE)
	{
		dutyCycle -= DUTY_CYCLE_INCREMENT;
	}
	TCNT1 = 0;
	OCR1A = ((uint32_t)dutyCycle * TIMER1_TOP) / MAX_DUTY_CYCLE;
}

void init_timer(void)
{

	// Timer0
	TCCR0A |= (1 << WGM01); // Mode 2 (CTC)
	TIMSK0 |= (1 << OCF0A); // Enable Timer0 Compare match A interrupt

	OCR0A = TIMER0_TOP;

	// Timer1
	TCCR1A |= (1 << COM1A1); // Compare Output Mode --> Set OC1A on Compare match
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM13) | (1 << WGM12); // Fast PWM Mode 14 (TOP = ICR1)
	
	// 	ICR1 decides the PWM frequency (how fast it turns on/off).
	// Freq 100kHz (100000 Hz) --> F_CPU / Freq = Timer ticks
	// 16000000 / 100000 = 160-
	// OCR1A decides the duty cycle (how long it stays on --> if blinks really fast can be used for brightness).

	ICR1 = TIMER1_TOP;
	OCR1A = ((uint32_t)dutyCycle * TIMER1_TOP) / MAX_DUTY_CYCLE; // 0 = 0% duty time, 160 (TIMER1_TOP) = 100% duty time

	
	// Timer0 prescaler = 256
	TCCR0B |= (1 << CS02); 
	// Timer1 prescaler = 1 (no prescaling)
	TCCR1B |= (1 << CS10); 
}

void init_led()
{
	// Set bit 1 of DDRB register to 1 (output) --> LED D2
	DDRB |= (1 << PB1);
}

int main(void)
{
	init_led();
	// Enable global interrupts
	sei();
	init_timer();
	while (1){}
	return 0;
}