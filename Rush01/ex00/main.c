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

// Format:  "28/02/2023 18:03:17"
// Size 19
// 0123456789/: 
uint8_t check_input(char *buffer)
{
    if (ft_strlen(buffer) != INPUT_FORMAT_SIZE)
        return (1);
    //
    if (!ft_is_allowed_chars(buffer, ALLOWED_CHARS))
        return (1);
    return (0);
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
        if (check_input(buffer))
            putstr_uart(WRONG_FORMAT);
        else
            putstr_uart(TIME_SET);
        return; // end of input need to check for format .. then set i = 0 and maybe empty buffer
    }
    else if (c == BACKSPACE)
    {
        if (i > 0)
        {
            putstr_uart(REMOVE_CHAR);
            i--;
        }
    }
    else if (i < INPUT_BUFFER_SIZE)
    {
        buffer[i] = c;
        i++;
    }
    write_uart(c);
}

// might want to create abstraction Function or Macro
// maybe can set as output and turn on right after
void init_default_led(void)
{
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);
    DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6);
}



// all 7segments (dot ?), D1, D2, D3, D4
// TODO: manage the switches and light D9, D10, D11
// for the sw1, sw2, sw3 polling ?
void init_display(uint8_t addr)
{
     PORTB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);
     i2c_set_pin(addr, OUTPUT_PORT_0_COMMAND, PORT_0_ALL_DIGIT_ON);
     i2c_set_pin(addr, OUTPUT_PORT_1_COMMAND, PORT_1_ALL_ON);

     _delay_ms(3000);
     // turn off --> clear
     
     /* _delay_ms(1000); */

}

// for now clears: D1, D2, D3, D4, D5, D9, D10, D11, all of 7segments, D6, D7, D8
void clear_all(uint8_t addr)
{
    uint8_t color[3] = {0x00, 0x00, 0x00};
    PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4));
    PORTD &= ~((1 << PD3) | (1 << PD5) | (1 << PD6));
    i2c_set_pin(addr, OUTPUT_PORT_0_COMMAND, PORT_0_ALL_OFF);
    i2c_set_pin(addr, OUTPUT_PORT_1_COMMAND, PORT_1_ALL_OFF);
    spi_set_led_all(0xFF, color);

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

    if (switch1Pressed || switch2Pressed)
        _delay_ms(250);
    return (mode);
}

int main(void) 
{
    uint8_t mode = 0;
    uint8_t addr = I2C_EXPANDER_ADDR;
    uint8_t (*modes[])(uint8_t) = {mode0, mode1, mode2};
    /* char buffer[INPUT_BUFFER_SIZE]; */
    i2c_init();
    spi_init();
    /* init_timer1(2); */
    sei();
    init_default_led();
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

    // initial display;
    clear_all(addr);
    init_display(addr);
    
    // TEST for numbers
    uint8_t tab[12] = {DISPLAY_0, DISPLAY_1, DISPLAY_2, DISPLAY_3, DISPLAY_4, DISPLAY_5, DISPLAY_6, DISPLAY_7, DISPLAY_8, DISPLAY_9, DISPLAY_DASH, DISPLAY_DOT};
    i2c_set_pin(addr, OUTPUT_PORT_0_COMMAND, PORT_0_ALL_DIGIT_ON);
    while (1)
    {
        modes[mode](0);
        i2c_print_digits(addr, tab[10], tab[4], tab[2], tab[10]);
        mode = get_input_switch(mode);
        display_mode_binary(mode);
    }
    return (0);

}
