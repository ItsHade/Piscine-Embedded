#include "main.h"

// a simple counter to set a flag half every 80 ms (counter 0)
// this counter will be used in sensor mode, to wait before every reading without blocking main loop
void    init_timer0(void) {
    TCNT0 = 0;
    // p.116 setting prescaller value at 1024
    TCCR0B |= (1 << CS02) | (1 << CS00);
    // p.115 clear compare mode, will set top ar OCRA
    TCCR0A |= (1 << WGM01);
    // Maths has been done outside code, its the value at which top is atteined every 80 ms
    OCR0A = 195;
    // enable interrupts on comapare match a
    TIMSK0 |= (1 << OCIE0A);
}

// a simple counter to set a flag half second (counter 1)
void    init_timer1(uint8_t freq_hz)
{
    TCNT1 = 0;
     // Set CS12 to one, modifying clock select.
    // CTC mode ; top will be OCR1A
    TCCR1B |= (1 << WGM12);
    // calculate
    uint16_t top = (F_CPU / (2 * TIMER1_PRESCALER * freq_hz) - 1);
    OCR1A = top;
    // Enable interrupts on timer 1, then interrupts on CTC match for OCR1a
    TIMSK1 |= (1 << OCIE1A);

    // Now timer 1 will increment every 256 ticks
    TCCR1B |= (1 << CS12);
}

