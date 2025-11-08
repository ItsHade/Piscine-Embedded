#include "main.h"

// 24. Analog-to-Digital Converter
// RV1: ADC_NTC -> PC2, ADC_POT -> PC0, ADC_LDR -> PC1

// ADC: Analog-to-Digital Converter, a system that converts an analog signal (finger touching a touchscreen, sound entering a microphone, light entering a digital camera) into a digital signal.
// NTC: Negative Temperature Coefficient, NTC thermistors are resistors with a negative temperature coefficient (resistance decreases with increasing temperatur)
// LDR: Light Dependent Resistor, a type of passive electronic sensor used to detect light
// POT: Potentiometer, a variable resistor in which a wiper sweeps from on end of the resistive element to the other (resistance proportional to the wiper's position)

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
	// ADC Prescaler selection (128)
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
	
	// Disable digital input buffer
	DIDR0 |= (1 << ADC0D);
	DIDR0 |= (1 << ADC1D);
	DIDR0 |= (1 << ADC2D);
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
	init_adc();
	init_uart();

	while (1)
	{
		// Table 24-4 Input Channel Selections
		// MUX [3:0] already set to 0 --> ADC0
		puthex_uart(read_adc());
		putstr_uart(HEX_SEPARATOR);
		// 0001 -> ADC1
		ADMUX |= (1 << MUX0);
		puthex_uart(read_adc());
		putstr_uart(HEX_SEPARATOR);
		ADMUX &= ~(1 << MUX0);
		// 0010 -> ADC2
		ADMUX |= (1 << MUX1);
		puthex_uart(read_adc());
		putstr_uart(NEW_LINE);
		ADMUX &= ~(1 << MUX1);
		_delay_ms(20);
	}
	return 0;
}
