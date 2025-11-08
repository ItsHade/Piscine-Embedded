#include <avr/io.h>
#include <stdint.h>

#define PRESCALER 1024
#define TOP (F_CPU / (2 * PRESCALER * 1) - 1)

// 13.3.1 Alternate Functions of Port B

// 15.11.1 TCCR1A - Timer/Counter1 Control Register A

int main()
{
    // Set the bit 1 of register DDRB to output (1) doesn't change the other bits
    DDRB |= (1 << PB1);
    
    // [Timer setup]
    // 15.11.1 and Table 15-3
    // Set COM1A0 bit to 1 to toggle OC1A on compare match
    // Table 13-3 --> The PB1 pin can serve as an external output for the Timer/Counter1 compare match
    TCCR1A |= (1 << COM1A0);

    // Table 15-5
    // Set WGM12 bit to 1 to enable CTC mode
    TCCR1B |= (1 << WGM12);
    // Set prescaler to 1024
    TCCR1B |= (1 << CS12) | (1 << CS10);

    //15.2.1 Registers
    // OCR1A defines the TOP value (maximum Timer/Counter value)
    // OCR1A = F_CPU / (2 * prescaler * Freq(LED)) - 1
    // -1 comes from how CTC mode works (from 0 to OCR1A and not 1 to OCR1A)
    OCR1A = TOP; 

    while (1) {}
}