#include "main.h"

void init_timer(void)
{
    // [Timer setup]
    // Table 15-5
    // Set prescaler to 1024
    TCCR1B |= (1 << CS12) | (1 << CS10);
}


uint8_t check_timer(uint8_t counter)
{
    if (counter >= 1)
        PORTB &= ~(1 << PB4);
    if (counter >= 2)
        PORTB &= ~(1 << PB2);
    if (counter >= 3)
        PORTB &= ~(1 << PB1);
    if (counter >= 4)
        PORTB &= ~(1 << PB0);
    if (TIFR1 & (1 << TOV1))
    {
        BitSet(TIFR1, TOV1);
        TCNT1 = TIMER_VALUE;
        return (1);
    }
    return (0);
}