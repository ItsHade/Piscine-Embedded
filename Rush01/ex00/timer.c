#include "main.h"


// a simple counter to increment a global value every half second (counter 1)
void    init_timer1(uint8_t freq_hz)
{
     // Set CS12 to one, modifying clock select.
    // Now timer 1 will increment every 256 ticks
    TCCR1B |= (1 << CS12);
    // CTC mode ; top will be OCR1A
    TCCR1A |= (1 << WGM12);
    // calculate 
    uint16_t top = (F_CPU / (2 * TIMER1_PRESCALER * freq_hz) - 1);
    OCR2A = top;
    // Enable interrupts on timer 1, then interrupts on CTC match for OCR1a 
    TIMSK1 |= (1 << OCIE1A);
}

FT_ISR(TIMER1_COMPA_vect) {
    // do stuff here
    
}

