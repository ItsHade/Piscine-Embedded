#include "main.h"

// 24. Analog-to-Digital Converter
// RV1: ADC_NTC -> PC2, ADC_POT -> PC0, ADC_LDR -> PC1

// ADC: Analog-to-Digital Converter, a system that converts an analog signal (finger touching a touchscreen, sound entering a microphone, light entering a digital camera) into a digital signal.
// NTC: Negative Temperature Coefficient, NTC thermistors are resistors with a negative temperature coefficient (resistance decreases with increasing temperatur)
// LDR: Light Dependent Resistor, a type of passive electronic sensor used to detect light
// POT: Potentiometer, a variable resistor in which a wiper sweeps from on end of the resistive element to the other (resistance proportional to the wiper's position)

void init_adc()
{
	// 24.8 Temperature Measurement
	ADMUX |= (1 << REFS0) | (1 << REFS1); // internal 1.1V voltage reference must be selected
	
	// select ADC8 channel
	ADMUX |= (1 << MUX3);

	// 24.9.2
	// ADC Enable 
	ADCSRA |= (1 << ADEN); 
	// ADC Prescaler selection (128)
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
	
}	

uint16_t read_adc()
{
	// 24.9.2
	// ADC Start Conversion (needed if not in free running mode)
	ADCSRA |= (1 << ADSC);
	// Wait for conversion to be done
	while (ADCSRA & (1 << ADSC));
	return ADC;
}

// NOTE: The voltage sensitivity is ~ 1mV/C (degree) and the accuracy of the temperature measurement is +/-10C
uint16_t convert_to_celsius(uint16_t adc)
{
	float adc25 = 314.0; // ADC value at 25C
	float slope_adc = (380.0 - 242.0) / (85.0 - (-45.0)); // ~1mv/C --> 24.8 | 1.0615 counts/C
	// T = T0 + (ADC - ADC0) / slope | slope means how many ADC counts change per C													
	float tempC = 25.0 + (adc - adc25) / slope_adc; // (
	return ((uint16_t)tempC);
}

int main(void)
{
	uint16_t adc = 0;
	init_adc();
	init_uart();

	while (1)
	{
		// 
		adc = read_adc();
		putnbr_uart(convert_to_celsius(adc));
		putstr_uart(NEW_LINE);
		_delay_ms(20);
	}
	return 0;
}
