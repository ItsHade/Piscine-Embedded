#include "main.h"
#include "main.h"


volatile uint8_t g_mode = 0;
//TODO: clear/reset function
// g_mode to binary on LED D1 to D4

// modes function will be in different files
uint8_t mode0(uint8_t setup)
{
    uint8_t color[3] = {0xFF, 0x00, 0x00};
    spi_set_led(LED_D6, 0xFF, color);
    return (setup); // might not use setup, return setup cause compiler error
}


uint8_t mode1(uint8_t setup)
{
    uint8_t color[3] = {0x00, 0xFF, 0x00};
    spi_set_led(LED_D7, 0xFF, color);
    return (setup); // might not use setup, return setup cause compiler error
}

uint8_t mode2(uint8_t setup)
{
    uint8_t color[3] = {0x00, 0x00, 0xFF};
    spi_set_led(LED_D8, 0xFF, color);
    return (setup); // might not use setup, return setup cause compiler error
}

FT_ISR(USART_RX_vect)
{
    static char buffer[INPUT_BUFFER_SIZE];
    static uint8_t i = 0;
    char c = 0;
     
    c = read_uart();
    if (c == END_OF_INPUT)
    {
        buffer[i] = '\0';
        return; // end of input need to check for format .. then set i = 0 and maybe empty buffer
    }
    else if (c == BACKSPACE)
    {
        if (i > 0)
        {
            write_uart('\b');
            write_uart(' ');
            write_uart('\b');
            i--;
        }
    }
    else if (i < INPUT_BUFFER_SIZE)
    {
        buffer[i] = c;
        i++;
        
    }
    write_uart(c);
    (void)buffer; // compilation
}

// probably can be way cleaner
void get_input_switch(void)
{
    static uint8_t switch1Pressed = 0;
    static uint8_t switch2Pressed = 0;
    if (!(PIND & (1 << PD2)) && switch1Pressed == 0)
    {
        switch1Pressed = 1;
        // next mode
        g_mode++;
        if (g_mode >= MODES_COUNT)
            g_mode = 0;
    }   
    else
        switch1Pressed = 0;

    if (!(PIND & (1 << PD4)) && switch2Pressed == 0)
    {
        switch2Pressed = 1;
        // previous mode
        if (g_mode == 0)
            g_mode = MODES_COUNT;
        g_mode--;
    }   
    else
        switch2Pressed = 0;

    if (switch1Pressed || switch2Pressed)
        _delay_ms(250);
}

int main(void) 
{
    uint8_t addr = I2C_EXPANDER_ADDR;
    uint8_t (*modes[])(uint8_t) = {mode0, mode1, mode2};
    /* char buffer[INPUT_BUFFER_SIZE]; */
    i2c_init();
    spi_init();
    /* init_timer1(2); */
    sei();
    init_uart();
    _delay_ms(10);

    //SPI TEST
    /* uint8_t color[3] = {0x00, 0xFF, 0x00}; */
    /* spi_set_led_all(0xFF, color); */
    /* _delay_ms(500); */
    /* spi_set_led(LED_D6, 0xFF, color); */

    // init all pins as output
    i2c_set_pin(addr, CONFIG_PORT_0_COMMAND, ALL_OUTPUT);
    i2c_set_pin(addr, CONFIG_PORT_1_COMMAND, ALL_OUTPUT);

    // TEST for numbers
    uint8_t tab[12] = {DISPLAY_0, DISPLAY_1, DISPLAY_2, DISPLAY_3, DISPLAY_4, DISPLAY_5, DISPLAY_6, DISPLAY_7, DISPLAY_8, DISPLAY_9, DISPLAY_DASH, DISPLAY_DOT};
    i2c_set_pin(addr, OUTPUT_PORT_0_COMMAND, PORT_0_ALL_DIGIT_ON);
    while (1)
    {
        modes[g_mode](0);
        i2c_print_digits(addr, tab[10], tab[4], tab[2], tab[10]);
        get_input_switch();
    }
    return (0);

}
