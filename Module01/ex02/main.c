#include <avr/io.h>
#include <stdint.h>

// 13.3.1 Alternate Functions of Port B

// 15.11.1 TCCR1A - Timer/Counter1 Control Register A

// PWM: Pulse Width Modulation is a technique by which the width of a pulse is varied while keeping the frequency constant
//Exemple: If it's ON for 50% of the period it has a duty cycle or 50% 
// ex02 request a duty cycle of 10% and a frequency of 1Hz, so the LED should be ON 100ms and OFF 900ms

#define PRESCALER 1024
#define TOP (F_CPU / (PRESCALER * 1) - 1)
#define DUTY_CYCLE 10 // percentage of the frequency (1Hz)

int main()
{
    // Set the bit 1 of register DDRB to output (1) doesn't change the other bits
    DDRB |= (1 << DDB1);
    
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
    OCR1A = ICR1 / DUTY_CYCLE ;
    
    
    while (1) {}
}
