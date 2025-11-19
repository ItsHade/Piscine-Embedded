#include "atmega2560.h"

void set_pin(volatile uint8_t *port, uint8_t pin, uint8_t value)
{
    // Clear pin on port
    *port &= ~(1 << pin);

    // Set it to value
    *port |= (value << pin);
}


/*
ZRX543
4x4 Keypad
Pin:         1  2  3  4  5  6  7  8
DIGITAL PIN: 37 36 35 34 33 32 31 30
*/

void keypad_init(void)
{
    // Default value is INPUT
    // So only need to set port as high
    /* set_pin(&PORTC, PC7, HIGH); // 30 */
    /* set_pin(&PORTC, PC6, HIGH); // 31 */
    /* set_pin(&PORTC, PC5, HIGH); // 32 */
    /* set_pin(&PORTC, PC4, HIGH); // 33 */
    /* set_pin(&PORTC, PC3, HIGH); // 34 */
    /* set_pin(&PORTC, PC2, HIGH); // 35 */
    /* set_pin(&PORTC, PC1, HIGH); // 36 */
    /* set_pin(&PORTC, PC0, HIGH); // 37 */

    set_pin(&DDRE, PE4, INPUT); // 2
    set_pin(&DDRE, PE5, INPUT); // 3
    set_pin(&DDRG, PG5, INPUT); // 4
    set_pin(&DDRE, PE3, INPUT); // 5
    set_pin(&DDRH, PH3, INPUT); // 6
    set_pin(&DDRH, PH4, INPUT); // 7
    set_pin(&DDRH, PH5, INPUT); // 8
    set_pin(&DDRH, PH6, INPUT); // 9
    
    set_pin(&PORTE, PE4, HIGH); // 2
    set_pin(&PORTE, PE5, HIGH); // 3
    set_pin(&PORTG, PG5, HIGH); // 4
    set_pin(&PORTE, PE3, HIGH); // 5
    set_pin(&PORTH, PH3, HIGH); // 6
    set_pin(&PORTH, PH4, HIGH); // 7
    set_pin(&PORTH, PH5, HIGH); // 8
    set_pin(&PORTH, PH6, HIGH); // 9
}

uint8_t keypad_read(void)
{
    uint8_t read = 0x00;

    read |= PINE & (1 << PE4);
    read |= PINE & (1 << PE5);
    read |= PING & (1 << PG5);
    read |= PINE & (1 << PE3);
    read |= PINH & (1 << PH3);
    read |= PINH & (1 << PH4);
    read |= PINH & (1 << PH5);
    read |= PINH & (1 << PH6);
    return (read);
}

/*
5611AH
1 Digit 7 segment display:
DIGITAL PIN: 29 27 _  25 23  22 24 _   26 28
Pin:         1  2  3   4  5  6  7  8   9  10
Use:         E  D  GND C  DP B  A  VCC F  G
*/
// only works if plugged the exact same way

void segments_display(uint8_t digit)
{
#ifdef DEBUG
    uart_putstr("digit: ");
    uart_putnbr(digit);
    uart_putstr(" | 0x");
    uart_puthex(digit);
    uart_putstr(" | ");
    uart_putbin(digit);
    uart_putstr(NEW_LINE);
#endif
    set_pin(&PORTA, PA0, 1 & (digit >> 6)); // 22
    set_pin(&PORTA, PA1, 1 & (digit >> 0)); // 23
    set_pin(&PORTA, PA2, 1 & (digit >> 7)); // 24
    set_pin(&PORTA, PA3, 1 & (digit >> 5)); // 25
    set_pin(&PORTA, PA4, 1 & (digit >> 2)); // 26
    set_pin(&PORTA, PA5, 1 & (digit >> 4)); // 27
    set_pin(&PORTA, PA6, 1 & (digit >> 1)); // 28
    set_pin(&PORTA, PA7, 1 & (digit >> 3)); // 29
}

void segments_init(void)
{
    set_pin(&DDRA, PA0, OUTPUT); // 22
    set_pin(&DDRA, PA1, OUTPUT); // 23
    set_pin(&DDRA, PA2, OUTPUT); // 24
    set_pin(&DDRA, PA3, OUTPUT); // 25
    set_pin(&DDRA, PA4, OUTPUT); // 26
    set_pin(&DDRA, PA5, OUTPUT); // 27
    set_pin(&DDRA, PA6, OUTPUT); // 28
    set_pin(&DDRA, PA7, OUTPUT); // 29

}


int main(void)
{
    uint8_t value = 1;
    uint8_t digitIndex = 0;
    uint8_t digitArray[10] = {DISPLAY_0, DISPLAY_1, DISPLAY_2, DISPLAY_3, DISPLAY_4, DISPLAY_5, DISPLAY_6, DISPLAY_7, DISPLAY_8, DISPLAY_9};
    uart_init();
    segments_init();
    keypad_init();
    // PB7 -> DIGITAL PIN 13 & BUILTIN LED
    set_pin(&DDRB, PB7, OUTPUT);
    // RED LED -> DIGITAL PIN 53 = PB0
    // GREEN LED -> DIGITAL PIN 52 = PB1
    // BLUE LED -> DIGITAL PIN 50 = PB3
    set_pin(&DDRB, PB0, OUTPUT);
    set_pin(&DDRB, PB1, OUTPUT);
    set_pin(&DDRB, PB3, OUTPUT);
    while(1)
    {
        if (digitIndex >= 10)
            digitIndex = 0;
        set_pin(&PORTB, PB7, value);
        set_pin(&PORTB, PB0, value);
        set_pin(&PORTB, PB1, value);
        set_pin(&PORTB, PB3, value);
#ifdef DEBUG
        uart_putstr("digitIndex: ");
        uart_putnbr(digitIndex);
        uart_putstr(NEW_LINE);
#endif
        segments_display(digitArray[digitIndex]);
        uart_putstr("Keypad pin: ");
        uart_putbin(keypad_read());
        uart_putstr(NEW_LINE);
        digitIndex++;
        value = !value;

        _delay_ms(1000);
    }

    return 0;
}

