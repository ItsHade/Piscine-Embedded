#include "main.h"

// 24. Analog-to-Digital Converter
// RV1: ADC_NTC -> PC2, ADC_POT -> PC0, ADC_LDR -> PC1

// ADC: Analog-to-Digital Converter, a system that converts an analog signal (finger touching a touchscreen, sound entering a microphone, light entering a digital camera) into a digital signal.
// NTC: Negative Temperature Coefficient, NTC thermistors are resistors with a negative temperature coefficient (resistance decreases with increasing temperatur)
// LDR: Light Dependent Resistor, a type of passive electronic sensor used to detect light
// POT: Potentiometer, a variable resistor in which a wiper sweeps from on end of the resistive element to the other (resistance proportional to the wiper's position)

void init_led()
{
	// LED D1, D2, D3, D4 as output
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);

	// LED D5 as output
	DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6); 

}

void init_timer()
{

	// Set timers: Timer0 (A and B) and Timer2 (B)
	// Timer0
	TCCR0A |= (1 << WGM01) | (1 << WGM00);  // Mode 3 (fast PWM)
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
	TCCR0B |= (1 << CS00) | (1 << CS02);
	TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);

}

void set_led(uint8_t adc)
{
	PORTB = 0;
	if (adc >= (ADR_8BIT_MAX / 4))
		PORTB |= (1 << PORTB0);
	if (adc >= (ADR_8BIT_MAX / 2))
		PORTB |= (1 << PORTB1);
	if (adc >= (ADR_8BIT_MAX / 4 * 3))
			PORTB |= (1 << PORTB2);
	if (adc == ADR_8BIT_MAX)
		PORTB |= (1 << PORTB4);
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	OCR0B = r;
	OCR0A = g;
	OCR2B = b;
}

// wheel function given in Module03 ex02
void wheel(uint8_t pos)
{
	pos = 255 - pos;
	if (pos < 85) {
		set_rgb(255 - pos * 3, 0, pos * 3);
	} else if (pos < 170) {
		pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3);
	} else {
		pos = pos - 170;
		set_rgb(pos * 3, 255 - pos * 3, 0);
	}
}

void init_adc()
{
	// 24.9.1
	// Table 24-3 Voltage Reference Selections for ADC
	ADMUX |= (1 << REFS0); // AVcc with external capacitor at AREF pin	
						   // ADLAR = 1 -> left adjusts the result of conversion | 0 -> right adjusts result of conversion (10 bits in 16bits register)
						   // 24.9.3.2 if the result is left adjusted and no more than 8bit precision required -> reading ADCH is sufficient
	ADMUX |= (1 << ADLAR);


	// ADC_POT -> PC0 (ADC0)
	// MUX[3:0] all set to 0

	// 24.9.2
	// ADC Enable 
	ADCSRA |= (1 << ADEN); 
	// ADC Prescaler selection (8)
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1);

	// Disable digital input buffer
	DIDR0 |= (1 << ADC0D);
}	

uint8_t read_adc()
{
	// 24.9.2
	// ADC Start Conversion (needed if not in free running mode)
	ADCSRA |= (1 << ADSC);
	// Wait for conversion to be done
	while (ADCSRA & (1 << ADSC));
	return ADCH;
}

int main(void)
{
	uint8_t adc = 0;
	init_led();
	init_adc();
	init_timer();

	while (1)
	{
		adc = read_adc();
		wheel(adc);
		set_led(adc);
		_delay_ms(20);
	}
	return 0;
}
