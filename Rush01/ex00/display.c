#include "main.h"


// might want to create abstraction Function or Macro
// maybe can set as output and turn on right after
void init_default_led(void)
{
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);
    DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6);
}


// for now clears: D1, D2, D3, D4, D5, D9, D10, D11, all of 7segments, D6, D7, D8
void clear_all(uint8_t addr)
{
    uint8_t color[3] = SPI_ALL_OFF;
    PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4));
    PORTD &= ~((1 << PD3) | (1 << PD5) | (1 << PD6));
    i2c_set_pin(addr, OUTPUT_PORT_0_COMMAND, PORT_0_ALL_OFF);
    i2c_set_pin(addr, OUTPUT_PORT_1_COMMAND, PORT_1_ALL_OFF);
    spi_init();
    spi_set_led_all(0xFF, color);
    spi_close();
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10)); //stop timer1
    cli();
}

// all 7segments (dot ?), D1, D2, D3, D4
// TODO: manage the switches and light D9, D10, D11
// for the sw1, sw2, sw3 polling ?
void init_display(uint8_t addr)
{
    uint16_t i = 0;
     PORTB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);
     i2c_set_pin(addr, OUTPUT_PORT_0_COMMAND, PORT_0_ONLY_DIGITS_ON);
     i2c_set_pin(addr, OUTPUT_PORT_1_COMMAND, PORT_1_ALL_ON);

     while (i < 3000)
     {
         initial_check_switch(addr);
         _delay_ms(1);
         i++;
     }
     i = 0;
     spi_init(); // needed for clear_all()
     clear_all(addr);
     while (i < 1000)
     {
         initial_check_switch(addr);
         _delay_ms(1);
         i++;
     }
}


// might be over complicated because MODES_COUNT = 12 so we don't use the D4
void display_mode_binary(uint8_t mode)
{
    uint8_t mask = mode;
    if (mask & (1 << 3))
    {
        mask &= ~(1 << PB3);
        mask |= (1 << PB4);
    }
    PORTB = mask;
}

void set_rgb_led(uint8_t color[3])
{
    PORTD &= ~(D5_RED | D5_GREEN | D5_BLUE);
    uint8_t port = 0;
    if (color[0] != 0)
        port |= D5_RED;
    if (color[1] != 0)
        port |= D5_GREEN;
    if (color[2] != 0)
        port |= D5_BLUE;
    PORTD |= port;
}
