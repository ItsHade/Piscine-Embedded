#include <avr/io.h>
#include <util/delay.h>

// mask used to isolate only specific bit
#define BIT0_MASK 0x01
#define BIT1_MASK 0x02
#define BIT2_MASK 0x04
#define BIT3_MASK 0x08
#define BIT4_MASK 0x10
#define MAX_BYTE_VALUE 255
int main(void) 
{
    unsigned int button1Pressed = 0;
    unsigned int button2Pressed = 0;
    int counter = 0;
    // bit 2 and 4 of port D (PD2 and PD4) is set to 0 (input) by default

    // Set bit 0, 1, 2 and 4 of port B to output (LED D1)
    DDRB = 0x17;

    // Set bit 2 and 4 of port D to ON (SW1)
    PORTD = 0x14;

    PORTB = 0;

    while (1)
    {
        if (!(PIND & BIT2_MASK) && button1Pressed == 0)
        {
            button1Pressed = 1;
            counter++;
        }
        else if (PIND & BIT2_MASK)
            button1Pressed = 0;

        if (!(PIND & BIT4_MASK) && button2Pressed == 0)
        {
            button2Pressed = 1;
            counter--;
        }
        else if (PIND & BIT4_MASK)
            button2Pressed = 0;

        if (counter & BIT0_MASK)
            PORTB = PORTB | BIT0_MASK;
        else
            PORTB = PORTB & (MAX_BYTE_VALUE - BIT0_MASK);
        if (counter & BIT1_MASK)
            PORTB = PORTB | BIT1_MASK;
        else
            PORTB = PORTB & (MAX_BYTE_VALUE - BIT1_MASK);
        if (counter & BIT2_MASK)
            PORTB = PORTB | BIT2_MASK;
        else
            PORTB = PORTB & (MAX_BYTE_VALUE - BIT2_MASK);
        if (counter & BIT3_MASK)
            PORTB = PORTB | BIT4_MASK;
        else
            PORTB = PORTB & (MAX_BYTE_VALUE - BIT4_MASK);

        _delay_ms(100);
    }
}
