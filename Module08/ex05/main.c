#include "main.h"


void set_rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t led)
{
    // {LED_D6, LED_D7, LED_D8}
    uint8_t color[3] = {r, g, b};
    static uint8_t colors[3][3] = {SPI_COLOR_OFF, SPI_COLOR_OFF, SPI_COLOR_OFF}; 

    if (led == LED_D6 || led == LED_ALL)
        ft_memcpy(colors[0], color, sizeof(color));
    if (led == LED_D7 || led == LED_ALL)
        ft_memcpy(colors[1], color, sizeof(color));
    if (led == LED_D8 || led == LED_ALL)
        ft_memcpy(colors[2], color, sizeof(color));

    spi_set_led_all(SPI_BRIGHTNESS_MAX, colors);
}

uint8_t check_switch2(uint8_t led)
{
    static uint8_t switch2Pressed = 0;
    if (IS_PRESSED(PD4) && !switch2Pressed)
    {
        switch2Pressed = 1;
        // next led
        led = (led + 1) % LED_COUNT;
        #ifdef DEBUG
            putstr_uart("Currently managing led: ");
            putnbr_uart(led);
            putstr_uart(NEW_LINE);
        #endif
        
    }   
    else if (!IS_PRESSED(PD4))
        switch2Pressed = 0;

    if (switch2Pressed)
        _delay_ms(250);
    return (led);
}

uint16_t check_switch1(uint8_t mode)
{
    static uint8_t switch1Pressed = 0;
    if (IS_PRESSED(PD2) && !switch1Pressed)
    {
        switch1Pressed = 1;
        // next led
        mode = (mode + 1) % MODE_COUNT;
        #ifdef DEBUG
        putstr_uart("Switch to color mode: ");
        putnbr_uart(mode);
        putstr_uart(NEW_LINE);
        #endif
    }   
    else if (!IS_PRESSED(PD2))
        switch1Pressed = 0;

    if (switch1Pressed)
        _delay_ms(250);
    return (mode);
}

uint8_t convert_to_rgb(uint16_t read)
{
    uint16_t value = 0;
    // number from 0 to 1023 --> number from 0 to 255
    value = read / 4; // 1024 / 256 = 4
    return ((uint8_t)value);
}

int main(void)
{
    uint8_t led = LED_D6;
    uint8_t mode = MODE_R; // r g b
    uint16_t read = 0;
    uint8_t newMode = 0;
    uint8_t newLed = 0;
    uint8_t value = 0;
    static uint8_t colors[3][3] = {SPI_COLOR_OFF, SPI_COLOR_OFF, SPI_COLOR_OFF};
    spi_init();
    init_adc();
    init_uart();
    
    spi_set_led_all(SPI_BRIGHTNESS_MAX, colors);
    
    while (1)
    {
        newMode = check_switch1(mode);
        if (newMode != mode)
        {
            read = read_adc();
            value = convert_to_rgb(read);
            colors[led][mode] = value;

            spi_set_led_all(SPI_BRIGHTNESS_MAX, colors);
            mode = newMode;
        }
        newLed = check_switch2(led);
        if (newLed != led)
        {
            mode = MODE_R;
            led = newLed;
        }
    }
    spi_close();
}

