#include "main.h"




// if setup not needed
uint8_t setup_empty(uint8_t setup)
{
    return (setup);
}

uint8_t setup_adc(uint8_t setup)
{
    adc_init();
    return (setup);
}

// uint8_t setup_mode3(uint8_t setup)
// {
//     ADMUX |= (1 << REFS0) | (1 << REFS1); // 1.1V REF, channel#8 is temperature
//     ADMUX |= (1 << MUX3);
//     ADCSRA |= (1 << ADEN);     // enable the ADC 

//     // ADC Prescaler selection (128)
// 	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
//     return (setup);
// }

uint8_t setup_mode4(uint8_t setup)
{
    init_timer1(1);
    spi_init();
    return (setup);
}

uint8_t mode4(uint8_t setup)
{
    uint8_t tab[12] = {DISPLAY_0, DISPLAY_1, DISPLAY_2, DISPLAY_3, DISPLAY_4, DISPLAY_5, DISPLAY_6, DISPLAY_7, DISPLAY_8, DISPLAY_9, DISPLAY_DASH, DISPLAY_DOT};
    uint8_t colors[3][3] = { SPI_ALL_RED, SPI_ALL_GREEN, SPI_ALL_BLUE };
    static uint8_t color_index = 0;
    if (setup)   
        color_index = 0;
    if (TIFR1 & (1 << OCF1A))
    {
        TIFR1 |= (1 << OCF1A);
        color_index = (color_index + 1) % 3;
    }
    spi_set_led_all(SPI_BRIGHTNESS, colors[color_index]);
    set_rgb_led(colors[color_index]);
    i2c_print_digits(I2C_EXPANDER_ADDR, tab[10], tab[4], tab[2], tab[10]); // test
    return (0);
}


uint8_t mode_selector(uint8_t mode, uint8_t newMode)
{
    uint8_t (*modes[])(uint8_t) = {display_RV1, display_LDR, display_NTC, display_temp, mode4, mode6, mode7, mode8, mode9, mode10, mode11};
    uint8_t (*setup[])(uint8_t) = {setup_adc, setup_adc, setup_adc, setup_adc, setup_mode4, setup_timers, setup_timers, setup_timers, setup_empty, setup_empty, setup_empty};
    if (newMode != mode)
    {
        clear_all(I2C_EXPANDER_ADDR); // need tests
        setup[newMode](0);
        modes[newMode](1); // if 1 is sent means mode was just changed (for static variables)
    }
    else
        modes[newMode](0);
    return (newMode);
}

int main(void) 
{
    uint8_t mode = 0;
    uint8_t newMode = 0;
    uint8_t time[TIME_ARRAY_SIZE] = {0};
    char buffer[INPUT_BUFFER_SIZE];
    i2c_init();
    init_default_led();
    init_uart();
    calibrate_Aht20();

    // init all pins as output
    i2c_set_pin(I2C_EXPANDER_ADDR, CONFIG_PORT_0_COMMAND, ALL_OUTPUT_SWITCH_INPUT);
    i2c_set_pin(I2C_EXPANDER_ADDR, CONFIG_PORT_1_COMMAND, ALL_OUTPUT);
    
    // initial display;
    clear_all(I2C_EXPANDER_ADDR);
    init_display(I2C_EXPANDER_ADDR);
        
    i2c_set_pin(I2C_EXPANDER_ADDR, OUTPUT_PORT_0_COMMAND, PORT_0_ONLY_DIGITS_ON); 
    while (1)
    {
        newMode = get_input_switch(mode);
        mode = mode_selector(mode, newMode);
        if (get_input_uart(buffer))
        {
            convert_input(buffer, time);
            if (format_is_valid(time))
            {
                putstr_uart(TIME_SET);
                rtc_set_time(time);
            }
            else
                putstr_uart(TIME_NOT_ALLOWED);
        }
        display_mode_binary(mode);
    }
    return (0);

}
