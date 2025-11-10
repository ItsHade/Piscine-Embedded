#include "main.h"

int main(void)
{
    uint8_t index = 0;
    uint8_t colors[SPI_LED_COUNT][3] = {SPI_COLOR_RED, SPI_COLOR_GREEN, SPI_COLOR_BLUE};
    uint8_t leds[SPI_LED_COUNT] = {LED_D6, LED_D7, LED_D8};
    spi_init();
    while (1)
    {
        if (index >= SPI_LED_COUNT)
            index = 0;
        spi_set_led_all(SPI_BRIGHTNESS_MAX, (uint8_t [3][3]){SPI_COLOR_OFF, SPI_COLOR_OFF, SPI_COLOR_OFF});
        _delay_ms(83); // rounded from 250 / 3
        spi_set_led(leds[index], SPI_BRIGHTNESS_MAX, colors[index]);
        index++;
        _delay_ms(250);
    }
    spi_close();
}

