#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

// 13.3.1 Alternate Functions of Port B

// 15.11.1 TCCR1A - Timer/Counter1 Control Register A

// PWM: Pulse Width Modulation is a technique by which the width of a pulse is varied while keeping the frequency constant
//Exemple: If it's ON for 50% of the period it has a duty cycle or 50% 
// ex02 request a duty cycle of 10% and a frequency of 1Hz, so the LED should be ON 100ms and OFF 900ms

#define PRESCALER 1024
#define TOP (F_CPU / (PRESCALER * 1) - 1)
#define MIN_DUTY_CYCLE 10
#define MAX_DUTY_CYCLE 100
#define DUTY_CYCLE_INCREMENT 10

int main()
{
    uint8_t dutyCycle = MIN_DUTY_CYCLE;
    uint8_t sw1Pressed = 0;
    uint8_t sw2Pressed = 0;
    // Set the bit 1 of register DDRB to output (1) doesn't change the other bits
    DDRB |= (1 << PB1);

    // Set the bit 2 and 4 of register PORTD to ON (1) doesn't change the other bits
    PORTD |= (1 << PD2) | (1 << PD4);

    // [Timer setup]
    // 15.11.1 and Table 15-3
    // Set COM1A1 bit to 1 to clear OC1A on compare match and set OC1A at BOTTOM
    TCCR1A |= (1 << COM1A1) | (1 << WGM11);

    // Table 15-5
    // Set WGM12 bit to 1 and WGM13 bit to 1 to enable Fast PWM mode 14 (TOP = ICR1)
    TCCR1B |= (1 << WGM12) | (1 << WGM13);
    // Set prescaler to 1024
    TCCR1B |= (1 << CS12) | (1 << CS10);
    
    //15.2.1 Registers
    // ICR1 defines the TOP value (maximum Timer/Counter value)
    // ICR1 = F_CPU / (prescaler * freq(PWD)) - 1
    ICR1 = TOP;
    OCR1A = ICR1 / 100 * dutyCycle ;
    
    while (1)
    {
        if (!(PIND & (1 << PD2)) && !sw1Pressed)
        {
            sw1Pressed = 1;
            dutyCycle += DUTY_CYCLE_INCREMENT;
        }
        else if ((PIND) & (1 << PD2))
            sw1Pressed = 0;

        if (!(PIND & (1 << PD4)) && !sw2Pressed)
        {
            sw2Pressed = 1;
            dutyCycle -= DUTY_CYCLE_INCREMENT;
        }
        else if ((PIND) & (1 << PD4))
            sw2Pressed = 0;

        if (dutyCycle < MIN_DUTY_CYCLE)
            dutyCycle = MIN_DUTY_CYCLE;
        else if (dutyCycle > MAX_DUTY_CYCLE)
            dutyCycle = MAX_DUTY_CYCLE;
        OCR1A = ICR1 / 100 * dutyCycle ;
        _delay_ms(25);
    }
}
