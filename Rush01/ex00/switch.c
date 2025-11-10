#include "main.h"


void initial_check_switch(uint8_t addr)
{
    uint8_t port0 = PORT_0_ONLY_DIGITS_ON;

    if (IS_PRESSED(PD2))
        port0 &= ~(LED_D9);

    if (IS_PRESSED(PD4))
        port0 &= ~(LED_D10);

    if (!(i2c_get_pin(addr, INPUT_PORT_0_COMMAND) & SW3))
        port0 &= ~(LED_D11);
    
    i2c_set_pin(addr, OUTPUT_PORT_0_COMMAND, port0);
}

uint8_t get_input_switch(uint8_t mode)
{
    static uint8_t switch1Pressed = 0;
    static uint8_t switch2Pressed = 0;
    if (IS_PRESSED(PD2) && !switch1Pressed)
    {
        switch1Pressed = 1;
        // next mode
        mode = (mode + 1) % MODES_COUNT;
    }   
    else if (!IS_PRESSED(PD2))
        switch1Pressed = 0;

    if (IS_PRESSED(PD4) && !switch2Pressed)
    {
        switch2Pressed = 1;
        // previous mode
        mode = (mode == 0) ? (MODES_COUNT - 1) : (mode - 1);
    }   
    else if (!IS_PRESSED(PD4))
        switch2Pressed = 0;

    if ((switch1Pressed && !IS_PRESSED(PD2)) || (switch2Pressed && !IS_PRESSED(PD4)))
        _delay_ms(250);
    return (mode);
}
