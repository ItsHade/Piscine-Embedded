#include "atmega2560.h"

int main(void) {

    // PB7 -> DIGITAL PIN 13 & BUILTIN LED
    DDRB |= (1 << PB7);
    DDRA |= (1 << PA2);
    while(1) {
        PORTB |= (1 << PB7);
        PORTA |= (1 << PA2);
        _delay_ms(500);
        PORTB &= ~(1 << PB7);
        PORTA &= ~(1 << PA2);
        _delay_ms(500);
    }

    return 0;
}

